#include "CPacketProcResult.h"
#include "ext.h"

//> public >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BOOL CPacketProcResult::Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase)
{
	if (!CPacketProcPhase::Init(pNWSess, pPrevPhase))
		return FALSE;
	m_eGamePhase = GAME_PHASE_RESULT;

	// 接続済みユーザの状態を切り替える
	int cnt=0;
	int nSearchIndex = 0;
	for(ptype_session pSess=pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		pSess->frame_count = 0;
		// 切り替え
		pSess->obj_state = OBJ_STATE_RESULT_CONFIRMING;
	}
	return TRUE;
}
//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//> 受信パケット処理
BOOL CPacketProcResult::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;

	// 認証済み以外処理しない
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	switch ( data[PACKET_HEADER_INDEX] )
	{
	case PK_CMD_CONFIRM:
		ret = SetConfirmed(sess);
		break;
	default:
		break;
	}

	return ret;
}
//< 受信パケット処理

//> 切断処理
BOOL CPacketProcResult::DisconnectSession(ptype_session sess)
{
	BOOL ret = CPacketProcPhase::DisconnectSession(sess);
	sess->entity = 0;
	sess->frame_count = 0;
	sess->game_ready = 0;
	sess->live_count = 0;
	return ret;
}
//< 切断処理

// 結果確認パケット処理
BOOL CPacketProcResult::SetConfirmed(ptype_session sess)
{
//	WCHAR msglog[1024];
//	SafePrintf(msglog, 1023, L"[%d]LoadComplete", sess->obj_no);
//	AddMessageLog(msglog);

	if (sess->connect_state != CONN_STATE_AUTHED)
		return FALSE;

	sess->obj_state = OBJ_STATE_RESULT_CONFIRMED;
	BOOL bAllConfirmed = TRUE;

	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		if (pSess->obj_state != OBJ_STATE_RESULT_CONFIRMED)
		{
			bAllConfirmed = FALSE;
			break;
		}
	}
	if (bAllConfirmed)
		m_eGamePhase = GAME_PHASE_ROOM;
	return TRUE;
}
