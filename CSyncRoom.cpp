#include "CSyncRoom.h"
#include "ext.h"

CSyncRoom::CSyncRoom() : CSyncProc()
{
}

CSyncRoom::~CSyncRoom()
{
	Clear();
	ClearQueue(m_tQueue.next);
}

BOOL CSyncRoom::Init(CNetworkSession* pNetSess)
{
	return CSyncProc::Init(pNetSess);
}

BOOL CSyncRoom::Frame()
{
	g_pCriticalSection->EnterCriticalSection_Session(L'W');

	BOOL ret = FALSE;
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		switch (pSess->obj_state)
		{
		case OBJ_STATE_EMPTY:
			if (pSess->connect_state == CONN_STATE_EMPTY)
			{
				if (pSess->game_ready)
					pSess->frame_count++;
				if (pSess->frame_count >= LOGIN_TIMEOUT)
				{
					WCHAR msg[128];
					SafePrintf(msg, 128, L"���[�U�[�F�ؒ�����:�^�C���A�E�g - OBJNo[%d]",  pSess->sess_index);
					// �F�،��ʃp�P�b�g�쐬
					BYTE pkt[MAX_PACKET_SIZE];
					INT packetSize = PacketMaker::MakePacketData_Authentication(pSess, pkt, AUTH_RESULT_TIMEOUT);
					ret |= AddPacket(pSess, pkt, packetSize);
					pSess->frame_count = 0;
				}
			}
			break;
		case OBJ_STATE_ROOM_MOVE:
			// �ړ��l�ݒ�m�F
			if (pSess->vx == 0
			 && pSess->vy == 0)
			{
#if 0
				WCHAR logmsg[MAX_LOG_BUFFER+1];
				SafePrintf(&logmsg[0], MAX_LOG_BUFFER, L"�ړ��̒�~[%d][%d]", pSess->ax, pSess->ay);
				AddMessageLog(logmsg);
#endif
				pSess->obj_state = OBJ_STATE_ROOM_READY;
				break;
			}
			else
			{
				if (pSess->vx != 0)
				{	// 0 �` WIN_WIDTH
					pSess->lx = max(0L, (int)min(pSess->lx+(pSess->vx*ROOM_MV_VEC_X), WIN_WIDTH));
					// �����ύX
					pSess->dir = (pSess->vx < 0) ? USER_DIRECTION_LEFT:USER_DIRECTION_RIGHT;
				}
				if (pSess->vy != 0)
				{	// 0 �` ROOM_CHARA_BASE_MAX_MOVE_H
					pSess->ly = max(0L, (int)min(ROOM_CHARA_BASE_MAX_MOVE_H, pSess->ly+(pSess->vy*ROOM_MV_VEC_Y) ) );
				}
			}
			break;
		default:
			break;
		}
	}
	g_pCriticalSection->LeaveCriticalSection_Session();
	return ret;

}