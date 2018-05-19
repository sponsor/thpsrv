#include "CPacketProcLoad.h"
#include "ext.h"

//> public >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BOOL CPacketProcLoad::Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase)
{
	if (!CPacketProcPhase::Init(pNWSess, pPrevPhase))
		return FALSE;
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 7;

	if (g_bOneClient || m_pMasterSession->game_ready)
	{
		m_eGamePhase = GAME_PHASE_LOAD;
		if (!CheckTeam(GetTeamCount()))
		{
#ifdef	_DEBUG
			MessageBox(NULL, L"チームわけが一致しません", L"error", MB_OK);
			AddMessageLog(L"チームわけが一致しません");
			return FALSE;
#endif
		}

		// 接続済みユーザの状態を切り替える
		int cnt=0;
		int nTeamNo = 0;
		int nSearchIndex = 0;
		for(ptype_session pSess=pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=pNWSess->GetSessionNext(&nSearchIndex))
		{
			pSess->scrinfo = NULL;
			// 認証済みか
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;

			// 切り替え
			pSess->obj_state = OBJ_STATE_LOADING;
			// 観戦以外
			if (pSess->team_no != GALLERY_TEAM_NO)
			{
				if (m_nTeamCount == 1)
				{
					pSess->team_no = nTeamNo;
					nTeamNo++;
				}
				pSess->entity = 1;	// 存在1
				// キャラランダムの確定
				if (pSess->chara_type == ROOM_CHARA_RANDOM_ID)
				{
					int nRndIndex = genrand_int32()%g_mapCharaScrInfo.size();

					std::map < int, TCHARA_SCR_INFO >::iterator it = g_mapCharaScrInfo.begin();
					for (int i=0;i<nRndIndex;i++)	it++;
					TBASE_SCR_INFO* scrrelinfo = &(*it).second;
					pSess->scrinfo = scrrelinfo;
					pSess->chara_type = scrrelinfo->ID;
				}
				else
				{
					pSess->scrinfo = common::scr::FindCharaScrInfoFromCharaType(pSess->chara_type, &g_mapCharaScrInfo);;
				}
				// スクリプトと関連付け
			}
			else
				pSess->entity = 0;	// 存在0
			// キャラ状態をゼロクリア
			ZeroMemory(pSess->chara_state, sizeof(char)*CHARA_STATE_COUNT);

			// 生存ターン数
			pSess->live_count = 0;
			packetSize = PacketMaker::AddPacketData_Load(packetSize, pSess, pktdata);
			cnt++;
		}
		if (cnt > 0)
		{
			// ロード命令
			PacketMaker::MakePacketHeadData_Load(m_nTeamCount, m_bytRuleFlg, m_nStageIndex, cnt, pktdata);
			packetSize = PacketMaker::AddEndMarker(packetSize, pktdata);
			if (packetSize)
				ret = AddPacketAllUser(NULL, pktdata, packetSize);
		}
	}
	return TRUE;
}
//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//> 受信パケット処理
BOOL CPacketProcLoad::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;

	// 認証済み以外処理しない
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	switch ( data[PACKET_HEADER_INDEX] )
	{
	case PK_CMD_LOAD_COMPLETE:		// ロード完了
		ret = SetLoadComplete(sess);
		break;
	default:
		break;
	}

	return ret;
}
//< 受信パケット処理

//> 切断処理
BOOL CPacketProcLoad::DisconnectSession(ptype_session sess)
{
	BOOL ret = CPacketProcPhase::DisconnectSession(sess);
	sess->obj_state = OBJ_STATE_MAIN_DEAD;
	return ret;
}
//< 切断処理

//> 定員確認
BOOL CPacketProcLoad::CheckTeam(int nTeamCount)
{
	int nUserCount = p_pNWSess->GetConnectUserCount();
	if (nTeamCount <= 1)	return TRUE;
//> 20101105 端数でもチーム設定できるようにする
	int *pTeamSeats;
	pTeamSeats = new int[g_nMaxLoginNum-1];
	ZeroMemory(pTeamSeats, sizeof(int)*(g_nMaxLoginNum-1));
	// 接続済みユーザの状態を切り替える
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		// チーム番号を加算
		if (pSess->team_no != GALLERY_TEAM_NO)	// 観戦者以外
			pTeamSeats[pSess->team_no]++;
	}

	// チームに人が居ない場合ミス
	for (int i=0;i<nTeamCount;i++)
	{
		if (!pTeamSeats[i])
		{
			SafeDeleteArray(pTeamSeats);
			return FALSE;
		}
	}
/*
	// ユーザー数に端数が出る場合FALSE
	if (nUserCount % nTeamCount)	return FALSE;

	int nMaxSeat = nUserCount / nTeamCount;
	int nUserNum = 0;

	// 接続済みユーザの状態を切り替える
	for (int i=0;i<nTeamCount;i++)
	{
		int nSeat = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(nUserCount);
			pSess;
			pSess=p_pNWSess->GetSessionNext(nUserCount))
		{
			// 認証済みか
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (i == pSess->team_no)
			{
				nSeat++;
				if (nSeat > nMaxSeat)
					return FALSE;
			}
		}
	}
*/
	SafeDeleteArray(pTeamSeats);
	return TRUE;
}
//< 定員確認

// ロード完了パケット処理
BOOL CPacketProcLoad::SetLoadComplete(ptype_session sess)
{
	WCHAR msglog[64];
	SafePrintf(msglog, 64, L"[%d]LoadComplete", sess->sess_index);
	AddMessageLog(msglog);

	if (sess->connect_state != CONN_STATE_AUTHED)
		return FALSE;
	sess->obj_state = OBJ_STATE_LOADCOMPLETE;

	return TRUE;
}