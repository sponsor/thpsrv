#include "CSyncLoad.h"
#include "ext.h"

CSyncLoad::CSyncLoad() : CSyncProc()
{
	m_bNextPhase = FALSE;
	m_nLoadCompleteCheckIndex = 0;
	m_nLoadingWaitTimeCounter = 0;
	m_nLoadingCheckUserCount = 0;
}

CSyncLoad::~CSyncLoad()
{
	Clear();
	ClearQueue(m_tQueue.next);
}

BOOL CSyncLoad::Init(CNetworkSession* pNetSess)
{
	BOOL ret = CSyncProc::Init(pNetSess);
	m_nLoadCompleteCheckIndex = 0;
	m_nLoadingTimeCounter = 0;
	m_nLoadingWaitTimeCounter = 0;
	m_bNextPhase = FALSE;
	m_nLoadingCheckUserCount = 0;

	return ret;
}

BOOL CSyncLoad::Frame()
{
	if (!m_nLoadingWaitTimeCounter)
		m_nLoadingCheckUserCount = p_pNWSess->CalcAuthedUserCount();

	// 最低限待つ時間
	m_nLoadingWaitTimeCounter++;

	if (g_bOneClient)
	{
		if (m_nLoadingWaitTimeCounter < GAME_LOADING_WAIT_TIME_ONE_CLIENT)
			return FALSE;
	}
	else
	{
		if (m_nLoadingWaitTimeCounter < GAME_LOADING_WAIT_TIME)
			return FALSE;
	}

	g_pCriticalSection->EnterCriticalSection_Session(L'0');

	if (m_nLoadCompleteCheckIndex >= m_nLoadingCheckUserCount)
	{
		m_bNextPhase = TRUE;
		g_pCriticalSection->LeaveCriticalSection_Session();
		return FALSE;
	}
	
	ptype_session sess = p_pNWSess->GetSessionFromIndex(m_nLoadCompleteCheckIndex);

	// スルー
	if (sess->connect_state != CONN_STATE_AUTHED	|| !sess->entity)
	{
		m_nLoadingTimeCounter = 0;
		m_nLoadCompleteCheckIndex++;
		g_pCriticalSection->LeaveCriticalSection_Session();
		return FALSE;
	}

	if (!m_nLoadingTimeCounter++)
	{
		BYTE pkt[MAX_PACKET_SIZE];
		INT packetSize = PacketMaker::MakePacketData_LoadReqLoadComplete(sess, pkt);
		g_pCriticalSection->LeaveCriticalSection_Session();
		return AddPacket(sess, pkt, packetSize);
	}
	
	// ack
	if (sess->obj_state == OBJ_STATE_LOADCOMPLETE)
	{
		m_nLoadingTimeCounter = 0;
		m_nLoadCompleteCheckIndex++;
		g_pCriticalSection->LeaveCriticalSection_Session();
		return FALSE;
	}

	// check TimeOut
	if (m_nLoadingTimeCounter > SYN_TIMEOUT)
	{
		// タイムアウトしたユーザーをキックする
		AddMessageLog(L"LoadingTimeOut\n");
		KickUser(sess->sess_index);
		
		m_nLoadingTimeCounter = 0;
		m_nLoadCompleteCheckIndex++;
		g_pCriticalSection->LeaveCriticalSection_Session();
		return FALSE;
	}

	g_pCriticalSection->LeaveCriticalSection_Session();
	return FALSE;
}
