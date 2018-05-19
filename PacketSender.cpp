#include "main.h"
#include "ext.h"

BOOL sendall(int sock, char* pkt, int* sendsize);

void SetErrorSession(ptype_session *ERRSess, int count)
{
	for(int i=0;i<count;i++)
	{
		if (ERRSess[i])
		{
			ERRSess[i]->sock= 0;//signal_sock_no;
			ERRSess[i]->connect_state = CONN_STATE_DESTROY;
	//		tmp=ERRmap[i];
			ERRSess[i]=NULL;
		}
	}
}
// ���[�U�[�L�����N�^�[�������X���b�h
DWORD __stdcall Thread_PacketSender(LPVOID param)
{
	if (!param) return 0;
	
	CNetworkSession *pNetSess = ((CNetworkSession*) param);

	int i,ErrIndex;
//	int rets=0;
	int tmp_sock;
	int nSendSize=0;
	type_packet *	 packet;
	type_session * c;
	type_session * ErrSess[MAX_SENDPACKET_ONELOOP_QUEUE_SIZE];
//	type_session * tmp;

	CCriticalSection*	pCriticalSection = ((CCriticalSection*) param);
	CPacketQueue* cp_queue = g_pPacketQueue;//pNetSess->GetQueue();

	int que_len;
	HANDLE hEvent = cp_queue->CreateGrowEventHandle();

	for(i=0;i<MAX_SENDPACKET_ONELOOP_QUEUE_SIZE;i++)
		ErrSess[i]=NULL;

	for(;;){
		ErrIndex=0;
//		EnterCriticalSection(&pNetSess->m_CriticalSectionPacket);
//		pthread_mutex_lock(&sendQuelock);

//		HANDLE hEvent = cp_queue->OpenMyEvent();
		// �L���[�̐������܂�܂ő҂�
		que_len=cp_queue->GetCount();
//		que_len = queue_get_length((type_queue const  * const  *)&send_que);
		while(que_len== 0)
		{
			WaitForSingleObject(hEvent, INFINITE);
			que_len=cp_queue->GetCount();
//			pthread_cond_wait(&sendQuecond,&sendQuelock);
		}
		cp_queue->ResetGrowEvent();

		// ��x�ɏ����ł���p�P�b�g��
//		if (que_len > MAX_SENDPACKET_ONELOOP_QUEUE_SIZE-1)
//			que_len = MAX_SENDPACKET_ONELOOP_QUEUE_SIZE;

		// �p�P�b�g�p�̃N���e�B�J���Z�N�V�����҂�
		// (�L���[�𗭂߂Ă���X���b�h�̑��삪�I���̂�҂�
		pCriticalSection->EnterCriticalSection_Packet(L'T');

		if (que_len >= g_nMaxSendPacketOneLoop)
		{
			WCHAR msgqlen[32];
			SafePrintf(msgqlen,32,L"send que count:%d", que_len);
			AddMessageLog(msgqlen);
		}

		i=0;
		while(i<que_len)
		{
			packet = cp_queue->Dequeue();
			i++;

			if (!packet)
				break;
			c = (type_session *)packet->session;
			tmp_sock = c->sock;

			if(!packet->session)				// �p�P�b�g�����NULL
			{
				AddMessageLog(L"�Z�b�V������null.........................");

//				DeletePacket(packet);
//				packet_destroy(packet);
//				if(i<que_len) packet = queue_pull_packet(&send_que);

//				if(i<que_len)
//					packet = cp_queue->Dequeue();
			}
			else if(tmp_sock==-2)				// sigpipe
			{
				AddMessageLog(L"sigpipe����.........................");

//				DeletePacket(packet);
//				packet_destroy(packet);
//				if(i<que_len) packet = queue_pull_packet(&send_que);

//				if(i<que_len)
//					packet = cp_queue->Dequeue();
			}
			else if(packet->size<MIN_PACKET_SIZE)				// �ŏ��p�P�b�g�T�C�Y�ȉ�
			{
				WCHAR errlog[256];
				SafePrintf(errlog, 255, L"Send error �p�P�b�g�̒�����5��菬����...%d\n",packet->data[2]);
				AddMessageLog(errlog);
			}
			else										// �p�P�b�g���M
			{
//				rets = send(packet->cli_sock, packet->data, packet->size,0);
//				if(rets<=0)						// ���M�G���[�`�F�b�N
				nSendSize = packet->size;

				if (!sendall(packet->cli_sock, (char*)packet->data, &nSendSize))
				{
					AddMessageLog(L"send errorsss");
					ErrSess[ErrIndex]=c;
					ErrIndex+=1;
					if (g_nMaxSendPacketOneLoop <= ErrIndex)
					{
						pCriticalSection->EnterCriticalSection_Session(L'`');
						SetErrorSession(&ErrSess[0], ErrIndex);
						pCriticalSection->LeaveCriticalSection_Session();
						ErrIndex = 0;
					}
				}
			}
			DeletePacket(packet);
		} //--while loop
		pCriticalSection->LeaveCriticalSection_Packet();

		if(ErrIndex!=0)
		{
			pCriticalSection->EnterCriticalSection_Session(L'{');
			SetErrorSession(&ErrSess[0], ErrIndex);
			pCriticalSection->LeaveCriticalSection_Session();
		}
	}
	return 0;
}


BOOL sendall(int sock, char* pkt, int* sendsize)
{
	int nBufferSize = *sendsize;
	*sendsize = 0;
	int nSent = 0;
	//> �p�P�b�g���M
	while (SOCKET_ERROR != (nSent = send(sock, &pkt[*sendsize], nBufferSize, 0)))
	{
		// �\�P�b�g������ꂽ
		if (!nSent)	break;
//		if (!nSent)	return FALSE;
		// �p�P�b�g�T�C�Y�I�[�o�[
		nBufferSize -= nSent;
		if (nBufferSize <= 0)
			break;
		*sendsize += nSent;
	}
	//< �p�P�b�g���M

	DWORD dwErrCode = WSAGetLastError();
	// �G���[(��u���b�L���O���[�h�̈ȊO)
	if(dwErrCode && WSAEWOULDBLOCK != dwErrCode)
	{
//#if ADD_WSAERROR_LOG
		WCHAR pc[16];
		SafePrintf(pc, 16, L"%d", dwErrCode);
		AddMessageLog(pc);
//#endif
		return FALSE;
	}
	return TRUE;
}
