#include "CPacketProcRoom.h"
#include "ext.h"
#include "../common/itemcost.h"
#include <queue>
#include <algorithm>

typedef struct {
	DWORD	flg;
	int	cost;
}TCostTable;
const TCostTable c_tblCost[GAME_ITEM_COUNT] =
	{
		{GAME_ITEM_01,GAME_ITEM_01_COST},			{GAME_ITEM_02,GAME_ITEM_02_COST},
		{GAME_ITEM_03,GAME_ITEM_03_COST},			{GAME_ITEM_04,GAME_ITEM_04_COST},
		{GAME_ITEM_05,GAME_ITEM_05_COST},			{GAME_ITEM_06,GAME_ITEM_06_COST},
		{GAME_ITEM_07,GAME_ITEM_07_COST},			{GAME_ITEM_08,GAME_ITEM_08_COST},
		{GAME_ITEM_09,GAME_ITEM_09_COST},			{GAME_ITEM_10,GAME_ITEM_10_COST},
		{GAME_ITEM_11,GAME_ITEM_11_COST},			{GAME_ITEM_12,GAME_ITEM_12_COST},
		{GAME_ITEM_13,GAME_ITEM_13_COST},			{GAME_ITEM_14,GAME_ITEM_14_COST},
		{GAME_ITEM_15,GAME_ITEM_15_COST},			{GAME_ITEM_16,GAME_ITEM_16_COST},
		{GAME_ITEM_17,GAME_ITEM_17_COST},			{GAME_ITEM_18,GAME_ITEM_18_COST},
		{GAME_ITEM_19,GAME_ITEM_19_COST},
	};

// 新しいユーザーを部屋に追加
BOOL CPacketProcRoom::AddNewUser(ptype_session sess)
{
	if (sess->connect_state != CONN_STATE_AUTHED)
		return FALSE;

	int sessindex = sess->sess_index;
	// 入室設定
	sess->obj_state = OBJ_STATE_ROOM_READY;
	sess->cost = g_nMaxCost;

	// マスター権限なし
	if (!m_pMasterSession)
	{
		sess->master = TRUE;		// 接続セッションをマスターに設定
		m_pMasterSession = sess;
	}

	// 新規ユーザに入室済みのユーザ情報を送信
	BOOL bTellPacket = TellNewUserToEntryUserList(sess);
	// 入室済みのユーザに新規ユーザ情報を周知
	BOOL bFamPacket = FamiliariseNewUser(sess);
	// チーム数情報を送信
	BOOL bTeamCountPacket = TellTeamCount(sess, FALSE);
	// ルール情報を送信
	BOOL bRulePacket = SetRule(sess, m_bytRuleFlg);
	// ステージ選択情報送信
	BOOL bStageSelectPacket = SetStageSelect(sess, (BYTE)m_nStageIndex);
	// 制限ターン数送信
	BOOL bTurnLimit = SetTurnLimit(sess, m_nTurnLimit);
	// 制限時間情報送信
	BOOL bActTimeLimit = SetActTimeLimit(sess, m_nActTimeLimit);

	// サーバのリストボックスにユーザ名追加
	WCHAR	username[MAX_USER_NAME+1];
	common::session::GetSessionName(sess, username);
	// 追加
	InsertUserList(sessindex, username);

	return (bTellPacket|bFamPacket|bTeamCountPacket|bRulePacket|bStageSelectPacket|bTurnLimit|bActTimeLimit);
}

//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

BOOL CPacketProcRoom::ReEnter(ptype_session sess)
{
	WCHAR log[64];
	SafePrintf(log, 64, L"ReEnter:#%d", sess->sess_index);
	AddMessageLog(log);

	BOOL ret = FALSE;
	// 入室設定
	sess->obj_state = OBJ_STATE_ROOM_READY;

	// マスター権限確認
	if (!m_pMasterSession)
	{
		int nSearchIndex = 0;
		for(ptype_session pRoomSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pRoomSess;
			pRoomSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pRoomSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (pRoomSess->master)
			{
				if (m_pMasterSession)
					pRoomSess->master = 0;
				else
				{
					pRoomSess->master = 1;
					m_pMasterSession = pRoomSess;
				}
			}
		}
	}
	else if (m_pMasterSession->master == 0)
		m_pMasterSession->master = 1;

	// 位置の初期化
//	sess->ax = WIN_WIDTH/2;
//	sess->ay = ROOM_CHARA_BASE_MAX_MOVE_H/2;
	sess->ax = 0;
	sess->ay = 0;
	sess->vx = 0;
	sess->vy = 0;

	WORD packetSize;
	BYTE pktdata[MAX_PACKET_SIZE];
	// 手持ちアイテムのコスト確認
	int nTotalCost = 0;
	for (int i=0;i<GAME_ITEM_STOCK_MAX_COUNT;i++)
	{
		// アイテムを持っているか
		if (sess->items[i] & GAME_ITEM_ENABLE_FLG)
		{
			// アイテム情報をテーブルから探す
			for (int nTableIndex = 0;nTableIndex < GAME_ITEM_COUNT;nTableIndex++)
			{
				if (c_tblCost[nTableIndex].flg == sess->items[i])
				{
					// 手持ちがコストオーバー
					if ((nTotalCost+c_tblCost[nTableIndex].cost) > g_nMaxCost)
					{
						sess->items[i] = 0x0;
						packetSize = PacketMaker::MakePacketData_RoomInfoItemSelect(i, 0, (WORD)(g_nMaxCost-nTotalCost), pktdata);
						if (packetSize)
							ret = AddPacket(sess, pktdata, packetSize);
					}
					else
					{
						nTotalCost += c_tblCost[nTableIndex].cost;
					}
					break;
				}
			}
		}
	}

	if ((int)sess->cost != (g_nMaxCost-nTotalCost))
	{
		sess->cost = (WORD)max(0,(g_nMaxCost-nTotalCost));
		packetSize = PacketMaker::MakePacketData_RoomInfoItemSelect(0, sess->items[0], sess->cost, pktdata);
		if (packetSize)
			ret = AddPacket(sess, pktdata, packetSize);
	}

	packetSize = PacketMaker::MakePacketData_RoomInfoReEnter(sess, pktdata);
	if (packetSize)
		ret |= AddPacketAllUser(NULL, pktdata, packetSize);

	// 入室済みのユーザ情報を送信
	ret |= TellNewUserToEntryUserList(sess);
	// チーム数情報を送信
	BOOL bTeamCountPacket = TellTeamCount(sess, FALSE);
	// ルール情報を送信
	BOOL bRulePacket = SetRule(sess, m_bytRuleFlg);
	// ステージ選択情報送信
	BOOL bStageSelectPacket = SetStageSelect(sess, (BYTE)m_nStageIndex);
	// 制限ターン数送信
	BOOL bTurnLimit = SetTurnLimit(sess, m_nTurnLimit);
	// 制限時間情報送信
	BOOL bActTimeLimit = SetActTimeLimit(sess, m_nActTimeLimit);

	return ret;
}

//> チーム数を知らせる
BOOL CPacketProcRoom::TellTeamCount(ptype_session sess, BOOL bAll, ptype_session ignore_sess)
{
	BYTE		pktdata[MAX_PACKET_SIZE];		// パケットデータ
	INT		packetSize = 0;
	BYTE		pktdataReady[MAX_PACKET_SIZE];		// パケットデータ
	INT		packetSizeReady = 0;

	// パケット作成
	packetSize = PacketMaker::MakePacketData_RoomInfoTeamCount(m_nTeamCount, pktdata);
	if (bAll)
	{
		// 全員の準備OKを解除
		int nUserCount = p_pNWSess->GetConnectUserCount();
		t_sessionInfo* sesstbl = p_pNWSess->GetSessionTable();
		for(int i=0;i<g_nMaxLoginNum;i++)
		{
			ptype_session pRoomSess = &sesstbl[i].s;
			if ( (pRoomSess->connect_state == CONN_STATE_AUTHED)
				&& (pRoomSess->obj_state & OBJ_STATE_ROOM)
				)
			{
				nUserCount--;
				if (pRoomSess == ignore_sess) continue;
				if (pRoomSess == sess)	continue;
				if (pRoomSess->game_ready)
				{
					pRoomSess->game_ready = 0;
					packetSizeReady = PacketMaker::MakePacketData_RoomInfoGameReady(pRoomSess->sess_index, FALSE, pktdataReady);
					if (!AddPacketAllUser(NULL, pktdataReady, packetSizeReady))
						return FALSE;
				}
				if (!nUserCount)	break;
			}
		}
		if (!AddPacketAllUser(ignore_sess, pktdata, packetSize))
			return FALSE;
	}
	else
	{
		// 準備OK状態なら解除しておく
		if (sess->game_ready)
		{
			sess->game_ready = 0;
			packetSizeReady = PacketMaker::MakePacketData_RoomInfoGameReady(sess->sess_index, FALSE, pktdataReady);
			if (!AddPacketAllUser(ignore_sess, pktdataReady, packetSizeReady))
				return FALSE;
		}
		if (!AddPacket(sess, pktdata, packetSize))
			return FALSE;
	}
	return TRUE;
}
//< チーム数を知らせる

//> 新規ユーザを周知する
BOOL CPacketProcRoom::FamiliariseNewUser(ptype_session sess)
{
	BYTE		pktdata[MAX_PACKET_SIZE];		// パケットデータ
	INT		packetSize = 0;
	
	packetSize = PacketMaker::MakePacketData_RoomInfoIn(sess, pktdata);

	// 入室者(自分を含める)にユーザ情報を送信するパケットを作成
	if (!AddPacketAllUser(NULL, pktdata, packetSize))
	{
		return FALSE;
	}

	return TRUE;
}
//< 新規ユーザを周知する

//> 新規ユーザに参加済み全ユーザ情報を送る
BOOL CPacketProcRoom::TellNewUserToEntryUserList(ptype_session sess)
{
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT packetSize = 0;
	BOOL bSend=FALSE;

//	ptype_session pObjSess = p_pNWSess->GetSessionFromUserNo(obj_no);
	int nUserCount = p_pNWSess->GetConnectUserCount();
	// 一人目のユーザなら相手が居ない
	if (nUserCount <= 1)
		return FALSE;
	// ルーム情報ヘッダ作成		// nUserCountから自分を引く
	packetSize += PacketMaker::MakePacketData_RoomInfoInHeader((BYTE)nUserCount-1, pktdata);

	//> 入室者に部屋に居るの全ユーザーの情報を送るパケット作成
	int nSearchIndex = 0;
	for(ptype_session pRoomSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pRoomSess;
		pRoomSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		if (pRoomSess == sess)	continue;
		if ( (pRoomSess->connect_state == CONN_STATE_AUTHED)
			&& (pRoomSess->obj_state | OBJ_STATE_ROOM)
			)
		{
			// ユーザ情報を追加
			packetSize += PacketMaker::MakePacketData_RoomInfoInChara(pRoomSess, &pktdata[packetSize]);
			bSend = TRUE;
		}
	}
	// パケット要送信
	if (bSend)
	{
		// フッタセット
		packetSize += PacketMaker::MakePacketData_SetFooter(packetSize, pktdata);
		if (!AddPacket(sess, pktdata, packetSize))
			return FALSE;
	}

	return bSend;
}
//< 新規ユーザに参加済み全ユーザ情報を送る

//> 受信パケット処理
BOOL CPacketProcRoom::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;

	// 認証済み以外処理しない
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	switch ( data[PACKET_HEADER_INDEX] )
	{
	case PK_CMD_CONFIRM:
		ret = ReEnter(sess);
		break;
	case PK_CHK_FILE_HASH:
	case PK_RET_HASH:
#if SCR_HASH_CHECK == 0
	case PK_USER_AUTH:
#endif
		ret = AddNewUser(sess);
		break;
	case PK_CMD_ROOM_CHARA_SEL:
		ret = SetCharacter(sess, data);
		break;
	case PK_CMD_ROOM_READY:
		ret = SetGameReady(sess, data);
		break;
	case PK_CMD_ROOM_MV:
		ret = SetMove(sess, data);
		break;
	case PK_CMD_ROOM_ITEM_SEL:
		ret = SetItem(sess, data);
		break;
	case PK_CMD_ROOM_TEAM_COUNT:
		ret = SetTeamCount(sess, data[3]);
		break;
	case PK_CMD_ROOM_RULE:
		ret = SetRule(NULL, data[3]);
		break;
	case PK_CMD_ROOM_STAGE_SEL:
		ret = SetStageSelect(NULL, data[3]);
		break;
	case PK_CMD_ROOM_RULE_TURN_LIMIT:
	{
		short turn = 0;
		memcpy(&turn, (void*)&data[3], sizeof(short));
		ret = SetTurnLimit(NULL, turn);
		break;
	}
	case PK_CMD_ROOM_RULE_ACT_TIME_LIMIT:
		ret = SetActTimeLimit(NULL, data[4]);
		break;
	case PK_REQ_TEAM_RAND:
		ret = SetTeamRandom(sess, data[3]);
		break;
	default:		break;
	}
	return ret;
}
//< 受信パケット処理

//> 切断処理
BOOL CPacketProcRoom::DisconnectSession(ptype_session sess)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;

	// 認証済みユーザー以外は処理しない
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	sess->entity = 0;
	sess->frame_count = 0;
	sess->game_ready = 0;
	sess->live_count = 0;
	sess->team_no = 0;

	// 切断したセッションがマスターだった場合
	if (sess->master)
	{
		ptype_session pNewMasterSess = NULL;
		sess->master = 0;
		// 最初に見つかったセッションにマスター権限を移す
		//> 新しいマスターを探す
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (pSess == sess)	continue;
			pNewMasterSess = pSess;
			pNewMasterSess->game_ready = 0;	// 準備状態を解除
			pNewMasterSess->master = 1;	// 準備状態を解除
			break;
		}

		if (pNewMasterSess)
			packetSize = PacketMaker::MakePacketData_RoomInfoMaster(pNewMasterSess->sess_index, TRUE, pktdata);
		if (packetSize)
			ret = AddPacketAllUser(sess, pktdata, packetSize);
		
		// 変更
		m_pMasterSession = pNewMasterSess;

		if (ret)
		{
			// チーム数設定を個人戦にする
			m_nTeamCount = 1;
			ret = TellTeamCount(sess, TRUE, sess);
		}
	}

	// セッションが切断することでチーム数設定の最大値を確認
	int nAuthedUserCount = p_pNWSess->CalcAuthedUserCount();
	if (!nAuthedUserCount)	nAuthedUserCount = 1;
//> 20101105 端数でもチーム設定できるようにする
//	int nMaxTeamCount = max(1,(int)((float)(nAuthedUserCount-1) / 2.0f));
//	int nMaxTeamCount = max(1,nAuthedUserCount-1);	// 20101114
	int nMaxTeamCount = min(MAXUSERNUM-1, max(1,nAuthedUserCount-2));
//< 20101105 端数でもチーム設定できるようにする
	if (m_nTeamCount > nMaxTeamCount)
	{
		m_nTeamCount = nMaxTeamCount;
		ret = TellTeamCount(NULL, TRUE, sess);
	}
	return ret;
}
//< 切断処理

//> キャラ選択パケット処理
BOOL CPacketProcRoom::SetCharacter(ptype_session sess, BYTE* data)
{
	BOOL ret = FALSE;
	// PK_CMD_ROOM_CHARA_SELx
	// size			: 2	0
	// header		: 1	2
	// chara_type: 1	3
	// footer		: 2	4
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	// ユーザno
	BYTE bytSessIndex = sess->sess_index;
	BYTE bytCharaNo =  data[PACKET_ROOM_CHARA_SEL_FLG_INDEX];

	/////////////////////////////////////////////////////////////////////////
	/// そのセッションが変準備状態が変更可能かチェックが必要
	/////////////////////////////////////////////////////////////////////////
	// 準備OKなら変更を受け付けない
	if (sess->game_ready)
	{
		AddMessageLog(L"E_ProcPK_CMD_ROOM_CHARA_SEL::既にゲーム準備OKのため変更不可");
		return FALSE;
	}
	else if (m_pMasterSession && m_pMasterSession->game_ready)
	{
		AddMessageLog(L"E_ProcPK_CMD_ROOM_CHARA_SEL::既にゲーム開始中のため変更不可");
		return FALSE;
	}
	if (sess->chara_type == bytCharaNo)
	{
									// クライアントで切り替え時しか送信しないようにするはず
//		return FALSE;		// パケット送信する必要ないけど一応する
	}
	else
		sess->chara_type = bytCharaNo;

	// パケット作成
	packetSize = PacketMaker::MakePacketData_RoomInfoCharaSelect(bytSessIndex, bytCharaNo, pktdata);
	if (packetSize)
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	
	return ret;
}

//> ゲーム準備パケット処理
BOOL CPacketProcRoom::SetGameReady(ptype_session sess, BYTE* data)
{
	BOOL ret = TRUE;
	// PK_CMD_ROOM_READY
	// size			: 2	0
	// header		: 1	2
	// gameready: 1	3
	// footer		: 2	4
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	// ユーザno
	BYTE bytSessIndex = sess->sess_index;
	BYTE bytGameReady =  data[PACKET_ROOM_GAME_READY_FLG_INDEX];

	/////////////////////////////////////////////////////////////////////////
	/// そのセッションが変準備状態が変更可能かチェックが必要
	/////////////////////////////////////////////////////////////////////////
	// マスター権限持ちの場合
	if (sess == m_pMasterSession)
	{
		BOOL bAllReady = TRUE;
		//> マスター以外、全員が準備OKか確認
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->connect_state != CONN_STATE_AUTHED) continue;
			if (pSess == m_pMasterSession)	continue;
			// 観戦
//			if ((int)(pSess->lx >= ROOM_GALLERY_WIDTH) && (pSess->lx < (ROOM_WIDTH-ROOM_GALLERY_WIDTH)))
			{
				if (!pSess->game_ready)
				{
					// 準備NGがいたら終了
					bAllReady = FALSE;
					break;
				}
			}
		}
		// 準備が出来ていなかったら、受け付けない
		if (!bAllReady)
			ret = FALSE;
	}
	else	// マスター権限無しのセッション
	{
		// マスター権限持ちが準備OKなら準備変更を受け付けない
		if (m_pMasterSession
		&& m_pMasterSession->game_ready)
			ret = FALSE;
	}

	if (!g_bOneClient)
	{
		// 準備変更不要
		if (!ret)	return FALSE;
	}
// クライアントで切り替え時しか送信しないようにするはず

	// 既に同じ設定してある
	if (sess->game_ready == bytGameReady)
	{
									// クライアントで切り替え時しか送信しないようにするはず
//		return FALSE;		// パケット送信する必要ないけど一応する
	}
	else
		sess->game_ready = bytGameReady;

	// 準備解除の時、チーム番号解除
	if (!bytGameReady)
		sess->team_no = 0;

	// マスターが準備OKならチーム分け
	if (m_pMasterSession->game_ready)
	{
		if (!SetTeamNo())
		{
			m_pMasterSession->game_ready = 0;
			bytGameReady = FALSE;
		}
		else
		{
			AddMessageLog(L"GAME_LOADING");
			m_eGamePhase = GAME_PHASE_LOAD;
			UpdateWMCopyData();
		}
	}

	packetSize = PacketMaker::MakePacketData_RoomInfoGameReady(bytSessIndex, bytGameReady, pktdata);
	if (packetSize)
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	
	// 移動中で準備OKになったら移動を止める、準備OKの位置でチーム番号設定
	if (bytGameReady)
	{
		sess->obj_state = OBJ_STATE_ROOM_READY_OK;
		if (( sess->vx != 0) || ( sess->vy != 0))
		{
			sess->vx = 0;	sess->vy = 0;	// 移動を止める			
			// 位置の送信
			packetSize = PacketMaker::MakePacketData_RoomInfoRoomCharaMove(sess, pktdata);
			if (packetSize)
				ret = AddPacketAllUser(NULL, pktdata, packetSize);
		}
		if (m_eGamePhase == GAME_PHASE_ROOM)
		{
			if (m_nTeamCount > 1)
			{
				int nSeparateWidth = ROOM_ENTRY_WIDTH / m_nTeamCount;
				int nTeamNo = GALLERY_TEAM_NO;
				if (sess->lx >= ROOM_ENTRY_LEFT && sess->lx < ROOM_ENTRY_RIGHT)
					nTeamNo = min(m_nTeamCount-1, (int)((sess->lx-ROOM_ENTRY_LEFT) / nSeparateWidth));
				sess->team_no = nTeamNo;
			}
			else
			{
				// 個人戦の時の観戦
				if (sess->lx < ROOM_ENTRY_LEFT || sess->lx >= ROOM_ENTRY_RIGHT)
					sess->team_no = GALLERY_TEAM_NO;
			}
		}
	}

	return ret;
}
//< ゲーム準備パケット処理

//> マスター権限パケット処理
BOOL CPacketProcRoom::SetRoomMaster(ptype_session sess, BYTE* data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	int			sess_index = sess->sess_index;
	BOOL	bMaster	= data[PACKET_ROOM_MASTER_FLG_INDEX];
	// PK_CMD_ROOM_READY
	// size			: 2	0
	// header		: 1	2
	// master		: 1	3
	// footer		: 2	4
	
	// 既に同じ設定してある
	if (sess->master == bMaster)
	{
									// クライアントで切り替え時しか送信しないようにするはず
//		return FALSE;		// パケット送信する必要ないけど一応する
	}
	else
		sess->master = bMaster;

	packetSize = PacketMaker::MakePacketData_RoomInfoMaster(sess_index, bMaster, pktdata);
	if (packetSize)
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	
	return ret;
}
//< マスター権限パケット処理

//> 移動情報パケット処理
BOOL CPacketProcRoom::SetMove(ptype_session sess, BYTE* data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;

	if (!sess || sess->connect_state != CONN_STATE_AUTHED)	return FALSE;
	int			sessindex = sess->sess_index;
	// PK_CMD_ROOM_MV
	// size			: 2	0
	// header		: 1	2
	// mv_x			: 1	3
	// mv_y			: 1	4
	// footer		: 2	6

	// 移動値(-1,0,1)
	short mvecx = 0;
	memcpy(&mvecx, &data[PACKET_ROOM_MV_INDEX], sizeof(short));
	short mvecy = 0;
	memcpy(&mvecy, &data[PACKET_ROOM_MV_INDEX+sizeof(short)], sizeof(short));
	/////////////////////////////////////////////////////////////////////////
	/// そのセッションが変準備状態が変更可能かチェックが必要
	/////////////////////////////////////////////////////////////////////////
	// 準備OK状態は移動不可
	if (sess->game_ready)	return FALSE;
	if (m_pMasterSession && m_pMasterSession->game_ready)	return FALSE;

	sess->vx = mvecx;
	sess->vy = mvecy;
	if (mvecx != 0)
		sess->dir = (mvecx>0)?(USER_DIRECTION_RIGHT):(USER_DIRECTION_LEFT);
#if 0
	if (sess->obj_state != OBJ_STATE_ROOM_MOVE)
	{
		WCHAR logmsg[32];
		SafePrintf(&logmsg[0], 32, L"移動の開始[%d][%d]", sess->ax, sess->ay);
		AddMessageLog(logmsg);
	}
#endif
	sess->obj_state = OBJ_STATE_ROOM_MOVE;
	// 位置の送信
	packetSize = PacketMaker::MakePacketData_RoomInfoRoomCharaMove(sess, pktdata);
	if (packetSize)
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	
	return ret;
}
//< 移動情報パケット処理

//> キャラアイテム選択パケット処理
BOOL CPacketProcRoom::SetItem(ptype_session sess, BYTE* data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];
	INT		packetSize = 0;
	int			sessindex = sess->sess_index;

	// PK_CMD_ROOM_ITEM_SEL
	// size			: 2	0
	// header		: 1	2
	// item_index: 1	3
	// item_flg		: 4	4
	// footer		: 2	8
	
	int nItemIndex = data[3];
	DWORD dwItemFlg;
	memcpy(&dwItemFlg, (void*)&data[4], sizeof(DWORD));
	dwItemFlg &= (DWORD)GAME_ITEM_ENABLE_FLG;	// 有効範囲外のフラグを消す

	// インデックス確認
	if (nItemIndex < 0 && nItemIndex >= GAME_ITEM_STOCK_MAX_COUNT)
		return FALSE;

	BOOL bOff = FALSE;
	DWORD dwSearchFlg = dwItemFlg;
	if (!dwItemFlg)
	{
		bOff = TRUE;
		dwSearchFlg = sess->items[nItemIndex];
	}
	// フラグ探す
	int nTableIndex = 0;
	for (;nTableIndex < GAME_ITEM_COUNT;nTableIndex++)
	{
		if (c_tblCost[nTableIndex].flg == dwSearchFlg)
			break;
	}
	if (nTableIndex >= GAME_ITEM_COUNT)
		return FALSE;
	else
	{
		// アイテムセットで残りコストよりアイテムコストが高い場合は拒否
		if (!bOff)
		{
			if ((int)sess->cost < c_tblCost[nTableIndex].cost)
				return FALSE;
		}
	}
	sess->items[nItemIndex] = dwItemFlg;

	// 手持ちアイテムのコスト確認
	int nTotalCost = 0;
	for (int i=0;i<GAME_ITEM_STOCK_MAX_COUNT;i++)
	{
		// アイテムを持っているか
		if (sess->items[i] & GAME_ITEM_ENABLE_FLG)
		{
			// アイテム情報をテーブルから探す
			for (int nTableIndex = 0;nTableIndex < GAME_ITEM_COUNT;nTableIndex++)
			{
				if (c_tblCost[nTableIndex].flg == sess->items[i])
				{
					// 手持ちがコストオーバー
					if ((nTotalCost+c_tblCost[nTableIndex].cost) > g_nMaxCost)
					{
						sess->items[i] = 0x0;
						packetSize = PacketMaker::MakePacketData_RoomInfoItemSelect(i, 0, (WORD)(g_nMaxCost-nTotalCost), pktdata);
						if (packetSize)
							ret = AddPacket(sess, pktdata, packetSize);
					}
					else
					{
						nTotalCost += c_tblCost[nTableIndex].cost;
					}
					break;
				}
			}
		}
	}

	// 情報を送り返す
	if ((int)sess->cost != (g_nMaxCost-nTotalCost))
	{
		sess->cost = (WORD)max(0,(g_nMaxCost-nTotalCost));
		packetSize = PacketMaker::MakePacketData_RoomInfoItemSelect(nItemIndex, dwItemFlg, sess->cost, pktdata);
		if (packetSize)
			ret = AddPacket(sess, pktdata, packetSize);
	}
	
	return ret;
}

//> チーム数変更パケット処理
BOOL CPacketProcRoom::SetTeamCount(ptype_session sess, BYTE data)
{
	BOOL	ret = FALSE;
	// PK_CMD_ROOM_ITEM_SEL
	// size			: 2	0
	// header		: 1	2
	// team_count: 1	3
	// footer		: 2	5

	// マスター以外変更拒否
	if (!sess->master)	return FALSE;
	
//> 20101105 端数でもチーム設定できるようにする
/*
	int nTeamCount = data;
	int nAuthedUserCount = p_pNWSess->CalcAuthedUserCount();
	int nMaxTeamCount = (int)(((float)nAuthedUserCount+1.0f) / 2.0f);
	if (nMaxTeamCount < nTeamCount)
		nTeamCount = nMaxTeamCount;
	else if (nTeamCount < 1)
		nTeamCount = 1;
	else if (nAuthedUserCount % nTeamCount)
		nTeamCount = 1;
*/
	int nTeamCount = data;
	int nMaxTeamCount = min(MAXUSERNUM-1, p_pNWSess->CalcAuthedUserCount()-1);
	if (nMaxTeamCount < nTeamCount)
		nTeamCount = nMaxTeamCount;
	else if (nTeamCount < 1)
		nTeamCount = 1;
//< 20101105 端数でもチーム設定できるようにする

	m_nTeamCount = nTeamCount;	
	ret = TellTeamCount(sess, TRUE);
	return ret;
}
//< チーム数変更パケット処理

//> ルール変更パケット処理
BOOL CPacketProcRoom::SetRule(ptype_session sess, BYTE data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	// PK_CMD_ROOM_ITEM_SEL
	// size			: 2	0
	// header		: 1	2
	// fule_flg		: 1	3
	// footer		: 2	5
	m_bytRuleFlg = data;
	
	// 送信
	packetSize = PacketMaker::MakePacketData_RoomInfoRule(m_bytRuleFlg, pktdata);
	if (packetSize)
	{
		if (sess)
			ret = AddPacket(sess, pktdata, packetSize);
		else
			ret = AddPacketAllUser(NULL, pktdata, packetSize);
	}
	return ret;
}
//< ルール変更パケット処理

//> ステージ選択パケット処理
BOOL CPacketProcRoom::SetStageSelect(ptype_session sess, BYTE data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	// PK_CMD_ROOM_STAGE_SEL
	// size			: 2	0
	// header		: 1	2
	// stage_index: 1	3
	// footer		: 2	5
	int nStageIndex = data;
	
	// 要求インデックス値確認
	if (nStageIndex < 0 || nStageIndex >= (int)g_mapStageScrInfo.size())
		return FALSE;
	
	m_nStageIndex = nStageIndex;

	// 送信
	packetSize = PacketMaker::MakePacketData_RoomInfoStageSelect(m_nStageIndex, pktdata);
	if (packetSize)
	{
		if (sess)
			ret = AddPacket(sess, pktdata, packetSize);
		else
			ret = AddPacketAllUser(NULL, pktdata, packetSize);
	}
	return ret;
}
//< ステージ選択パケット処理

BOOL CPacketProcRoom::SetTeamNo()
{
	int nUserCount = p_pNWSess->GetConnectUserCount();

	// 個人戦
	if (m_nTeamCount <= 1)
	{
		int nEntryUserCount = 0;
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->lx >= ROOM_ENTRY_LEFT && pSess->lx < ROOM_ENTRY_RIGHT)
				nEntryUserCount++;
		}
		// 参戦が一人以下ならFALSE
		if ((nEntryUserCount <= 1 || nEntryUserCount > MAXUSERNUM)&& !g_bOneClient)	return FALSE;
	}
//> 20101105 端数でもチーム設定できるようにする
/*
	// ユーザー数に端数が出る場合FALSE
	if (nUserCount % m_nTeamCount)	return FALSE;

	int nMaxSeat = nUserCount / m_nTeamCount;
	int nUserNum = 0;
	int nTeamWidth = (int)(WIN_WIDTH / m_nTeamCount);
	int nTeamSeats[MAX_TEAM_COUNT];
	ZeroMemory(nTeamSeats, sizeof(int)*MAX_TEAM_COUNT);

	// 接続済みユーザの状態を切り替える
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(nUserCount);
		pSess;
		pSess=p_pNWSess->GetSessionNext(nUserCount))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		
		// 位置によってチーム番号を取得
		int nTeamNo = (int)(pSess->ax / nTeamWidth);
		nTeamSeats[nTeamNo]++;
		if (nTeamSeats[nTeamNo] > nMaxSeat)
			return FALSE;
		pSess->team_no = nTeamNo;
	}
*/
	int nTeamWidth = (int)(ROOM_ENTRY_WIDTH / m_nTeamCount);
	int nTeamSeats[MAX_TEAM_COUNT];
	ZeroMemory(nTeamSeats, sizeof(int)*MAX_TEAM_COUNT);

	// 位置によるチーム番号を設定
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		// 位置によってチーム番号を取得
		int nTeamNo = GALLERY_TEAM_NO;
		if ((int)(pSess->lx >= ROOM_GALLERY_WIDTH) && (pSess->lx < (ROOM_WIDTH-ROOM_GALLERY_WIDTH)))
		{
			nTeamNo = min( (int)((pSess->lx-ROOM_GALLERY_WIDTH) / nTeamWidth), m_nTeamCount-1);
			nTeamSeats[nTeamNo]++;
		}
		pSess->team_no = nTeamNo;
	}
	// 人の居ないチームがある場合、チーム分け失敗
	for (int i=0;i<m_nTeamCount;i++)
	{
		if (!nTeamSeats[i])
			return FALSE;
	}
	
//< 20101105 端数でもチーム設定できるようにする
	return TRUE;
}

// 戻る
BOOL CPacketProcRoom::Reset(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase)
{
	BOOL ret = Init(pNWSess, pPrevPhase, pPrevPhase->GetStageIndex(), pPrevPhase->GetRuleFlg(), pPrevPhase->GetActTimeLimit());

	//> マスター以外、全員が準備OK解除
	t_sessionInfo* pSessTable = p_pNWSess->GetSessionTable();
	for(int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session pSess = &pSessTable[i].s;
		if (p_pNWSess->GetFlag(i) == 0)
		{
			p_pNWSess->InitSession(pSess);
			continue;
		}
		pSess->entity = 0;
//		pSess->entity = 1;
		pSess->obj_state = OBJ_STATE_RESULT_CONFIRMING;
		pSess->game_ready = 0;
	}

	int nAuthedUserCount = p_pNWSess->CalcAuthedUserCount();
//> 20101105 端数でもチーム設定できるようにする
//	// 現在のチーム数設定が最大チーム数より大きいなら1に設定
//	if (m_nTeamCount > nAuthedUserCount/2)
//		m_nTeamCount = 1;
	if (nAuthedUserCount <= 2 || m_nTeamCount > nAuthedUserCount-1)
		m_nTeamCount = 1;
//< 20101105 端数でもチーム設定できるようにする
	return ret;
}

// 制限ターン数パケット処理
BOOL CPacketProcRoom::SetTurnLimit(ptype_session sess, short data)
{
	ptype_session pSendSess = sess;
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;

	if (!pSendSess)
	{
		if (m_nTurnLimit == data)
			return FALSE;
	}

	if (data)	// 設定あり
	{
		// 最低制限ターン数を確認
		int nAuthedUserCount = p_pNWSess->CalcAuthedUserCount();
		if (nAuthedUserCount+1 > data)
		{
			m_nTurnLimit = nAuthedUserCount+1;
			pSendSess = NULL;
		}
		else
			m_nTurnLimit = max(0, min(999,data));
	}
	else
		m_nTurnLimit = 0;

	packetSize = PacketMaker::MakePacketData_RoomInfoTurnLimit(m_nTurnLimit, pktdata);
	if (pSendSess)
		ret = AddPacket(sess,  pktdata, packetSize);
	else
		ret = AddPacketAllUser(NULL, pktdata, packetSize);

	return ret;
}

// 制限時間パケット処理
BOOL CPacketProcRoom::SetActTimeLimit(ptype_session sess, BYTE data)
{
	ptype_session pSendSess = sess;
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;

	if (!pSendSess && m_nActTimeLimit == data)
		return FALSE;

	if (data < GAME_TURN_ACT_COUNT_MIN || data > GAME_TURN_ACT_COUNT_MAX)	// 値の範囲確認
		return FALSE;

	// 値更新
	m_nActTimeLimit = (int)data;
	// パケット送信
	packetSize = PacketMaker::MakePacketData_RoomInfoActTimeLimit(m_nActTimeLimit, pktdata);
	if (pSendSess)
		ret = AddPacket(sess,  pktdata, packetSize);
	else
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	return ret;
}

// チームランダム
BOOL CPacketProcRoom::SetTeamRandom(ptype_session sess, BYTE data)
{
	ptype_session pSendSess = sess;
	BOOL	ret = FALSE;
	BYTE	pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;

	if (!pSendSess && pSendSess->master)
		return FALSE;

	int nTeams = data;
	if (nTeams <= 0)
		return FALSE;
	std::vector <ptype_session> vecPlayer;
	// 参加人数計算
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		if ((int)(pSess->lx >= ROOM_GALLERY_WIDTH) && (pSess->lx < (ROOM_WIDTH-ROOM_GALLERY_WIDTH))){
			vecPlayer.push_back(pSess);
		}
	}
	int nPlayer = (int)vecPlayer.size();
	// ゲーム人数より多い場合はvectorをランダムに並べる
//	if (nPlayer > MAXUSERNUM)
		random_shuffle( vecPlayer.begin(), vecPlayer.end());

	if (nPlayer < nTeams)
		nTeams = nPlayer;

	int nPlayersInTeam = (int)((float)nPlayer / (float)nTeams);
	
	// 端数
	int iFraction = nPlayer % nTeams;

	std::vector<ptype_session>* pVecTeamPlayer = new std::vector<ptype_session>[nTeams];
	
	for (std::vector<ptype_session>::iterator it = vecPlayer.begin();
		it != vecPlayer.end();
		++it)
	{
		int nTeamIndex = 0;
		int nTeamSize = 0;
		do {
			nTeamIndex = genrand_int32() % nTeams;
			nTeamSize = (int)pVecTeamPlayer[nTeamIndex].size();
			if (iFraction && nTeamSize < nPlayersInTeam+1)
			{
				if (nTeamSize >= nPlayersInTeam)
					--iFraction;
				break;
			}
			else if (nTeamSize < nPlayersInTeam)
			{
				break;				
			}
		} while ( 1 );
		pVecTeamPlayer[nTeamIndex].push_back((*it));
	}

	packetSize = PacketMaker::MakePacketData_TeamRandomHeader(pktdata, (BYTE)nTeams);
	WCHAR* wstrTeamNo[] = { L"①",L"②",L"③",L"④",L"⑤",L"⑥",L"⑦",L"⑧",L"⑨" };
	std::wstring wstr;
	bool bFirst = true;
	for (int i=0;i<nTeams;++i)
	{
		WCHAR header[32];
		wsprintf(header, L"チーム%s:", wstrTeamNo[i]);
		wstr = std::wstring(header);
		bFirst = true;
		for (std::vector<ptype_session>::iterator it = pVecTeamPlayer[i].begin();
			it != pVecTeamPlayer[i].end();
			++it)
		{
			WCHAR name[MAX_USER_NAME+1];
			common::session::GetSessionName((*it), name);
			if (bFirst)
			{
				wstr += std::wstring(name);
				bFirst = false;
			}
			else
				wstr += std::wstring(L", ") + std::wstring(name);
		}
		packetSize += PacketMaker::MakePacketData_TeamRandomAddData(&pktdata[packetSize], wstr);
	}
	
	SafeDeleteArray(pVecTeamPlayer);
	// エンドマーカ
	packetSize += SetEndMarker(&pktdata[packetSize]);
	SetWordData(&pktdata[0], packetSize);
	ret = AddPacketAllUser(NULL, pktdata, packetSize);
	return ret;
}
