#include "main.h"
#include "ext.h"

BOOL recvall(int sock, char* pkt, int* recvsize, int bufsize);
int g_nSessionCount=0;
int g_bRestartFlg=FALSE;

DWORD __stdcall Thread_PacketReceiver(PVOID param)
{
	int		nTcpSock=0;
	const int c_nPacketRecvCount = 8;
	const int c_nPacketStockCount = 64;
	BYTE	arMsg[MAX_PACKET_SIZE*c_nPacketRecvCount];
	BYTE	arMsgto[c_nPacketStockCount][MAX_PACKET_SIZE];
	WORD arSize[c_nPacketStockCount];
	int nPacketProcIndex = 0;
	int nPacketStockIndex = 0;
	BOOL bStock = FALSE;

	int		nHighest_fd;
	int		nConnectSocket = 0;
	int		nPacketCount=0;
//	int		nIndex;
//	int		n;

	WORD	wRecvEnd;
	WORD	wEnd = PEND;
	fd_set	rfds, efds;
	int nDivIndex;
	int nRecvSize = 0;
	int nRecvStockSize = 0;
	BOOL bPacket;
	
	ptype_session	pObjSess;
	ptype_session	pConnectObjSess;

	g_nSessionCount = 0;
	CCriticalSection*	pCriticalSection = ((CCriticalSection*) param);
	CNetworkSession*	pNWSess = g_pNetSess;

	nTcpSock = pNWSess->GetSockListener();

//	int nUserCount = 0;
	int newUser = 0;

	while(TRUE)
	{
		FD_ZERO(&rfds);
		FD_ZERO(&efds);
		nHighest_fd = nTcpSock;
		FD_SET(nTcpSock, &rfds);

		//> �ڑ����[�U�[��FD_SET
		pCriticalSection->EnterCriticalSection_Session(L'u');
		static int nSearchIndex = 0;
		for(pConnectObjSess=pNWSess->GetSessionFirst(&nSearchIndex);
			pConnectObjSess;
			pConnectObjSess=pNWSess->GetSessionNext(&nSearchIndex))
		{
			nConnectSocket = pConnectObjSess->sock;
			FD_SET(nConnectSocket,&rfds);
			FD_SET(nConnectSocket,&efds);//extra receive

			if (nConnectSocket>nHighest_fd)
				nHighest_fd = nConnectSocket;
		}
		pCriticalSection->LeaveCriticalSection_Session();

		// ��M�҂�
		if (select(nHighest_fd+1,&rfds,NULL,&efds,(struct timeval *)0)<0)
		{
			if (errno!=/*EINTR*/0)
				continue;
		}

		if ( (newUser = NewUserSession(&rfds, nTcpSock, pNWSess, &pObjSess)) == -1)
			continue;
		g_nSessionCount += newUser;
#ifdef _DEBUG
		if (newUser)
		{
			WCHAR newuserlog[32];
			SafePrintf(newuserlog, 32, L"NewUser(usercount=%d)", g_nSessionCount);
			AddMessageLog(newuserlog);
		}

#endif
		//> �ڑ��ς݃��[�U�Ƃ̏���
//		int nSearchIndex = 0;
		for(pObjSess=pNWSess->GetSessionFirst(&nSearchIndex);
			pObjSess;
			pObjSess=pNWSess->GetSessionNext(&nSearchIndex))
		{
			// �ؒf�������Z�b�V����
			if (pObjSess->connect_state==CONN_STATE_DESTROY)
			{
				WCHAR log[64];
				WCHAR name[MAX_USER_NAME+1];
				common::session::GetSessionName(pObjSess, name);
				SafePrintf(log, 64, L"disconnect:%s", name);
				AddMessageLog(log);
				DisconnectSession(pObjSess);
//				g_nSessionCount--;
				continue;
			}
			nConnectSocket = pObjSess->sock;

			//> ��M�\�ȃ��[�U�[
			if (FD_ISSET(nConnectSocket,&rfds))
			{
				memset(&arMsg[nRecvStockSize],0,  sizeof(arMsg)-nRecvStockSize);
				//> �p�P�b�g��M
//				if((nRecvSize = recv(nConnectSocket, arMsg, MAX_PACKET_SIZE, 0))<=0)
				if (!recvall(nConnectSocket, (char*)arMsg, &nRecvSize, sizeof(arMsg)-nRecvStockSize))
				{
					// �˔��I�ɃA�N�Z�X���f
					WCHAR log[64];
					WCHAR name[MAX_USER_NAME+1];
					common::session::GetSessionName(pObjSess, name);
					SafePrintf(log, 64, L"abort disconnect in receive:%s", name);
					AddMessageLog(log);
					DisconnectSession(pObjSess);
					continue;
				}
				//< �p�P�b�g��M
				
				//> �p�P�b�g�`�F�b�N
				// �T�C�Y�O��M
				if (nRecvSize == 0)
				{
					AddMessageLog(L"�p�P�b�g�T�C�Y0��M\n");
					continue;
				}

				// ���v��M�T�C�Y��0�Ȃ�T�C�Y�ݒ�
				bStock = (nRecvStockSize>0);
				if (!bStock)
					memcpy(&arSize[nPacketStockIndex],&arMsg[0],2);	// �p�P�b�g�T�C�Y�擾

				nPacketCount=0;					//packet count reset

				// �p�P�b�g�X�g�b�N���ĂȂ�
				if (!bStock)
				{
					// 1�p�P�b�g�̂ݎ�M
					if( nRecvSize==arSize[nPacketStockIndex])
					{
						memset(arMsgto[nPacketStockIndex],0,MAX_PACKET_SIZE);
						memcpy(arMsgto[nPacketStockIndex],&arMsg[0],arSize[nPacketStockIndex]);
						memcpy(&wRecvEnd,&arMsg[arSize[nPacketStockIndex] -2],2);
						if(wRecvEnd != wEnd)
						{
							WCHAR msglog[64];
							SafePrintf(msglog, 64, L"�p�P�b�g�����ɃG���[...size=%d\n", (int)arSize[0]);
							AddMessageLog(msglog);
							continue;
						}
						nRecvStockSize = 0;	// �X�g�b�N�Ȃ�
						nPacketCount=1;
						nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
					}
					else	// �����p�P�b�g��M��1�p�P�b�g�ȉ���M
					{
						//> �p�P�b�g����
						nPacketCount=0;
						nDivIndex=0;
						nRecvStockSize = nRecvSize;
						for(;;)
						{
							if(nRecvStockSize <= 0)	break;
							if(arSize[nPacketStockIndex] > nRecvStockSize)	// ��M�o�b�t�@���p�P�b�g���傫��
							{
								// ��M�o�b�t�@��擪�Ɉړ�����K�v������
								memset(arMsgto[nPacketStockIndex],0,MAX_PACKET_SIZE);
								memcpy(arMsgto[nPacketStockIndex], &arMsg[nDivIndex], nRecvStockSize);
								break;
							}
							memset(arMsgto[nPacketStockIndex],0,MAX_PACKET_SIZE);
							memcpy(&arMsgto[nPacketStockIndex][0],&arMsg[nDivIndex],arSize[nPacketStockIndex]);
							memcpy(&wRecvEnd,&arMsgto[nPacketStockIndex][ (int)(arSize[nPacketStockIndex] -2)],2);
							if(wRecvEnd != wEnd)
							{
								WCHAR msglog[MAX_MSG_BUFFER+1];
								SafePrintf(msglog, MAX_MSG_BUFFER+1, L"�p�P�b�g�����ɃG���[(�������G���[)...size=%d/index=%d/stock=%d", arSize[nPacketStockIndex],nDivIndex,nRecvStockSize);
								nRecvStockSize = 0;
								break;
							}
							nDivIndex += arSize[nPacketStockIndex];
							nRecvStockSize -= arSize[nPacketStockIndex];
							nPacketCount++;
							nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
							// ���p�P�b�g�T�C�Y
							memcpy(&arSize[nPacketStockIndex],&arMsg[nDivIndex],2);
						}
						//< �p�P�b�g����
					}
				}
				else	// �p�P�b�g�X�g�b�N��
				{
					// �X�g�b�N+��M�T�C�Y��1�p�P�b�g��
					if( (nRecvStockSize+nRecvSize)==arSize[nPacketStockIndex])
					{
						memcpy(&arMsgto[nPacketStockIndex][nRecvStockSize],arMsg,arSize[nPacketStockIndex]-nRecvStockSize);
						memcpy(&wRecvEnd,&arMsgto[nPacketStockIndex][arSize[nPacketStockIndex] -2],2);
						if(wRecvEnd != wEnd)
						{
							WCHAR msglog[64];
							SafePrintf(msglog, 64, L"�p�P�b�g�����ɃG���[...size=%d\n", (int)arSize[0]);
							AddMessageLog(msglog);
							continue;
						}
						nRecvStockSize = 0;	// �X�g�b�N�Ȃ�
						nPacketCount=1;
						nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
					}
					else	// �p�P�b�g����
					{
						nPacketCount=0;
						nDivIndex=0;
						int nRecvStockIndex = nRecvStockSize;
						nRecvStockSize = nRecvSize;
						for (;;)
						{
							if(nRecvStockSize <= 0)	break;
							if ((arSize[nPacketStockIndex]-nRecvStockIndex) > (nRecvStockSize))	// ��M�o�b�t�@���p�P�b�g���傫��
							{
								// ��M�o�b�t�@��擪�Ɉړ�����K�v������
								memcpy(&arMsgto[nPacketStockIndex][nRecvStockIndex], &arMsg[nDivIndex], nRecvStockSize);
								nRecvStockSize += nRecvStockIndex;
								break;
							}
//							memset(arMsgto[nPacketStockIndex],0,MAX_PACKET_SIZE);
							memcpy(&arMsgto[nPacketStockIndex][nRecvStockIndex],&arMsg[nDivIndex],arSize[nPacketStockIndex]-nRecvStockIndex);
							memcpy(&wRecvEnd,&arMsgto[nPacketStockIndex][ (int)(arSize[nPacketStockIndex] -2)],2);
							if(wRecvEnd != wEnd)
							{
								WCHAR msglog[MAX_MSG_BUFFER+1];
								SafePrintf(msglog, MAX_MSG_BUFFER+1, L"�p�P�b�g�����ɃG���[(�������G���[)...size=%d/index=%d/stock=%d", arSize[nPacketStockIndex],nDivIndex,nRecvStockSize);
								nRecvStockSize = 0;
								break;
							}
							nDivIndex += arSize[nPacketStockIndex];
							nRecvStockIndex = 0;
							nRecvStockSize -= arSize[nPacketStockIndex];
							nPacketCount++;
							nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
							// ���p�P�b�g�T�C�Y
							memcpy(&arSize[nPacketStockIndex],&arMsg[nDivIndex],2);
						}
					}
				}
				//< �p�P�b�g�`�F�b�N

				bPacket = FALSE;
				pCriticalSection->EnterCriticalSection_Session(L'i');
				// �p�P�b�g����
				for(int n=0;n<nPacketCount;n++)
				{
					bPacket |= PacketProc(pNWSess, (BYTE*)arMsgto[nPacketProcIndex], pObjSess, arSize[nPacketProcIndex]);
					nPacketProcIndex = (nPacketProcIndex+1)%c_nPacketStockCount;
				}
				pCriticalSection->LeaveCriticalSection_Session();

				if (bPacket)
				{
					pCriticalSection->EnterCriticalSection_Packet(L'R');
					PacketEnqueue();
					pCriticalSection->LeaveCriticalSection_Packet();
				}
			}//if (FD_ISSET(csocket,&rfds)) end �ڑ���������̃p�P�b�g��M�I��

			// �G���[
			/*g_nSessionCount += */ErrorUserSession(nConnectSocket, &efds, nRecvSize, pNWSess, pObjSess);
		
			// �ڑ����[�U����l�����Ȃ��ꍇ
			if (!g_pNetSess->CalcAuthedUserCount())
			{
				g_pPPRoom->Init(g_pNetSess, g_pPPPhase, g_pPPPhase->GetStageIndex(), g_bytIniRuleFlg, g_nActTimeLimit);
				g_pPPPhase = g_pPPRoom;
				UpdateWMCopyData();
			}
		}
	}// endless loop
	return 0;
}

BOOL recvall(int sock, char* pkt, int* recvsize, int bufsize)
{
	int nBufferSize = bufsize;
	*recvsize=0;
	int nRecvd = 0;
	//> �p�P�b�g��M
	while (SOCKET_ERROR != (nRecvd = recv(sock, &pkt[*recvsize], nBufferSize, 0)))
	{
		// �\�P�b�g������ꂽ
		if (!nRecvd)
			return FALSE;
//		// ��M0
//		if (!nRecvd)
//			break;
		// ��M�c�肪����
		*recvsize += nRecvd;
		nBufferSize -= nRecvd;
	}
	DWORD dwErrCode = WSAGetLastError();
	// �G���[
	if(dwErrCode && WSAEWOULDBLOCK != dwErrCode)
	{
#if ADD_WSAERROR_LOG
		if (g_bLogFile)
		{
			WCHAR pc[16];
			SafePrintf(pc, 16, L"LastErr:%d", dwErrCode);
			AddMessageLog(pc);
		}
#endif
		return FALSE;
	}
	//< �p�P�b�g��M
	return TRUE;
}

BOOL PacketProc(CNetworkSession *pNWSess, BYTE* msg, ptype_session sess, INT msgsize)
{
	BOOL bAuthPacket = g_pPPAuth->PacketProc(msg, sess);
	BOOL bChatPacket = g_pPPChat->PacketProc(msg, sess);
	BOOL bPhasePacket = g_pPPPhase->PacketProc(msg, sess);	// Room,Load,Main,Return

	E_STATE_GAME_PHASE eGamePhase = g_pPPPhase->GetGamePhase();
	if (g_eGamePhase != eGamePhase)
	{
		g_eGamePhase = eGamePhase;
		switch (eGamePhase)
		{
		case GAME_PHASE_ROOM:
			bPhasePacket |= g_pPPRoom->Init(g_pNetSess, g_pPPPhase, g_nDefaultStageID, g_bytIniRuleFlg, g_nActTimeLimit);
			bPhasePacket |= g_pPPRoom->PacketProc(msg, sess);
			g_pPPPhase = g_pPPRoom;
			break;
		case GAME_PHASE_LOAD:
			bPhasePacket |= g_pPPLoad->Init(g_pNetSess, g_pPPPhase);
			bPhasePacket |= g_pPPLoad->PacketProc(msg, sess);
			g_pSyncLoad->Init(g_pNetSess);
			g_pPPPhase = g_pPPLoad;
			break;
		case GAME_PHASE_MAIN:
			bPhasePacket |= g_pPPMain->Init(g_pNetSess, g_pPPPhase);
			bPhasePacket |= g_pPPMain->PacketProc(msg, sess);
			g_pPPPhase = g_pPPMain;
			break;
		case GAME_PHASE_RESULT:
			bPhasePacket |= g_pPPResult->Init(g_pNetSess, g_pPPPhase);
			bPhasePacket |= g_pPPResult->PacketProc(msg, sess);
			break;
		case GAME_PHASE_RETURN:
			break;
		}
	}

	if (g_bLogFile)
	{
		int hd = msg[PACKET_HEADER_INDEX];
		WCHAR log[32];
		SafePrintf(log, 32, L"rcvhd:%d", hd);
		AddMessageLog(log);
	}

	return (bAuthPacket|bChatPacket|bPhasePacket);
}

void PacketEnqueue()
{
	g_pPacketQueue->Enqueue(g_pPPAuth->DequeuePacket());
	g_pPacketQueue->Enqueue(g_pPPChat->DequeuePacket());
	g_pPacketQueue->Enqueue(g_pPPRoom->DequeuePacket());
	g_pPacketQueue->Enqueue(g_pPPLoad->DequeuePacket());
	g_pPacketQueue->Enqueue(g_pPPMain->DequeuePacket());
}

// �ؒf����
void DisconnectSession(ptype_session sess)
{
	// �Z�b�V�����N���A�ς݊m�F
	if (!g_pNetSess->GetFlag(sess->sess_index))
		return;
	g_pCriticalSection->EnterCriticalSection_Session(L'o');
//	int sess_no = sess->obj_no;

	BOOL bAuthPacket = g_pPPAuth->DisconnectSession(sess);
	BOOL bChatPacket = g_pPPChat->DisconnectSession(sess);
	BOOL bPhasePacket = g_pPPPhase->DisconnectSession(sess);	// Room,Load,Main,Return

	// ���[�U���X�g����폜
	DeleteUserList(sess);
	g_pNetSess->ClearSession(sess);

	g_pCriticalSection->LeaveCriticalSection_Session();

	g_pCriticalSection->EnterCriticalSection_Packet(L'R');
	
	// ���M�p�P�b�g���肩
	if (bAuthPacket)
		g_pPacketQueue->Enqueue(g_pPPAuth->DequeuePacket());
	if (bChatPacket)
		g_pPacketQueue->Enqueue(g_pPPChat->DequeuePacket());
	if (bPhasePacket)
		g_pPacketQueue->Enqueue(g_pPPPhase->DequeuePacket());	// Room,Load,Main,Return

	g_nSessionCount--;

	WCHAR disc[40];
	SafePrintf(disc, 40, L"DiscSess(Count=%d)", g_nSessionCount);
	AddMessageLog(disc);
	g_pCriticalSection->LeaveCriticalSection_Packet();
	if (g_nSessionCount == 0)
	{
#ifdef _DEBUG
		Sleep(100);
#else
		MessageBox(g_hDlg, L"�T�[�o�����Z�b�g���܂�",L"�ڑ��l��0",MB_OK); 
#endif
		g_bRestartFlg = TRUE;
		SendMessage(g_hWnd, WM_NULL, NULL, NULL);
	}

	UpdateWMCopyData();
}

int ErrorUserSession(int nConnectSocket, fd_set *efds, int nRecvSize, CNetworkSession *pNWSess, ptype_session sess)
{
	// �G���[
	if (FD_ISSET(nConnectSocket,efds))
	{
		AddMessageLog(L"disconnect in efds");
//		return -1;

		if(nRecvSize==1)
			AddMessageLog(L"DISCONNECT DATA SAVE OK----------");
		else
			AddMessageLog(L"DISCONNECT DATA SAVE FAIL--------");

		DisconnectSession(sess);
		return -1;
	}//end for linked list search loop

	return 0;
}

int NewUserSession(fd_set *rfds, int nTcpSock, CNetworkSession *pNWSess, pptype_session ppSess)
{
	int ret = 0;
	//> �V�K�̃��[�U�̎�M����������
	//  �Z�b�V�����쐬
	if (FD_ISSET(nTcpSock, rfds))
	{
		int				    nTempSock;
		struct sockaddr_in 	tAddr;
		int					nAddr_len = sizeof(tAddr);

		// �N���C�A���g����̐ڑ��v�����󂯓���C�ʐM�H���m�ۂ���
		if ((nTempSock = accept(nTcpSock,(struct sockaddr *)&tAddr,&nAddr_len))<0)
		{
	        if(errno == WSAEINTR)
	            ret = -1;              /* retry accept : back to while() */
	        else
				AddMessageLog(L"error with accept");
			ret = -1;
		}
		
		*ppSess = NULL;
		//> ���[�U�[��񓯊�
		g_pCriticalSection->EnterCriticalSection_Session(L'p');
		if (pNWSess->CalcAuthedUserCount() < g_nMaxLoginNum)
		{
			// ����ݐ���
			*ppSess = pNWSess->CreateSession(nTempSock, ntohl(tAddr.sin_addr.s_addr), ntohs(tAddr.sin_port));
		}
		//< ���[�U�[��񓯊�
		g_pCriticalSection->LeaveCriticalSection_Session();
		// ���s�Ȃ�ؒf
		if (!(*ppSess))
		{
			AddMessageLog(L"error with session create");
			shutdown(nTempSock, 2);
			closesocket(nTempSock);
			ret = -1;
		}
		ret++;
	}
	
	return ret;
}

