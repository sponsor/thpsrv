#include "CPacketProcMain.h"
#include "main.h"
#include "ext.h"

// 初期化
BOOL CPacketProcMain::Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase)
{
	if (!CPacketProcPhase::Init(pNWSess, pPrevPhase))
		return FALSE;
	
	m_pSaveScrInfo = NULL;

	return TRUE;
}


//> 受信パケット処理
BOOL CPacketProcMain::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;
	// 認証済み以外処理しない
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	switch ( data[PACKET_HEADER_INDEX] )
	{
	case PK_CMD_MAIN_MV:
		ret = SetMove(sess, data);
		break;
	case PK_CMD_MAIN_FLIP:
		ret = SetFlip(sess, data);
		break;
	case PK_CMD_MAIN_SHOT:
		g_pCriticalSection->EnterCriticalSection_Object(L'1');
		ret = SetShot(sess, data);
		g_pCriticalSection->LeaveCriticalSection_Object();
		break;
	case PK_CMD_MAIN_PASS:
		ret = TurnPass(sess);
		break;
	case PK_ACK:
		ret = SetAck(sess);
		break;
	case PK_CMD_MAIN_ITEM:
		ret = OrderItem(sess, data);
		break;
	case PK_CMD_MAIN_SHOTPOWER:
		ret = SetShotPowerStart(sess, data);
		break;
	case PK_CMD_MAIN_TRIGGER:
		ret = SetTrigger(sess, data);
		break;
	case PK_CMD_MAIN_TURN_PASS:
		ret = SetTurnPass(sess, data);
		break;
	case PK_USER_MAIN_TRIGGER_END:
		ret = RcvTriggerEnd(sess, data);
		break;
	default:
		break;
	}

	return ret;
}
//< 受信パケット処理

//> 切断処理
BOOL CPacketProcMain::DisconnectSession(ptype_session sess)
{
	if (p_pNWSess)
		g_pSyncMain->OnDisconnectSession(sess);

	BOOL ret = CPacketProcPhase::DisconnectSession(sess);
	return ret;
}
//< 切断処理

BOOL CPacketProcMain::SetMove(ptype_session sess, BYTE* data)
{
	if (g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_ACT)
		return FALSE;
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)
		return FALSE;

	BOOL	ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	// PK_CMD_ROOM_MV
	// size			: 2	0
	// header		: 1	2
	// mv_x			: 1	3	(short)
	// footer		: 2	5

	// 移動値(-1,0,1)
	short mvecx = 0;
	memcpy(&mvecx, &data[PACKET_ROOM_MV_INDEX], sizeof(short));

	sess->vx = mvecx;
	sess->vy = 0;

	if (sess->vx != 0)
	{
		E_TYPE_USER_DIRECTION old_dir = sess->dir;
		sess->dir = (sess->vx>0)?USER_DIRECTION_RIGHT:USER_DIRECTION_LEFT;
		// 角度の反転が必要か
		if (old_dir != sess->dir)
			sess->angle = (sess->angle+180)%360;
#if 1
/*
		if (g_pLogFile)
		{
			WCHAR msglog[80];
			SafePrintf(msglog, 80, L"ChrMove#%d(px:%d,py%d,vx%d,vy%d)",sess->obj_no, sess->ax, sess->ay, sess->vx, sess->vy);
			AddMessageLog(msglog);
		}
*/
	}
	else if (g_pLogFile)	// 移動値設定確認
	{
		WCHAR msglog[80];
		SafePrintf(msglog, 80, L"ChrStop#%d(px:%d,py%d,vx%d,vy%d)",sess->sess_index, sess->ax, sess->ay, sess->vx, sess->vy);
		AddMessageLog(msglog);
	}
#else
	}
#endif

	// 移動不可状態
	if (sess->chara_state[CHARA_STATE_NOMOVE_INDEX])
		sess->vx = 0;

	// 位置の送信
	packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
	if (packetSize)
	{
		if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
			ret = AddPacketTeamUser(sess->team_no, pkt, packetSize);
		else
			ret = AddPacketNoBlindUser(sess, pkt, packetSize);
	}
	return ret;
}

BOOL CPacketProcMain::SetFlip(ptype_session sess, BYTE* data)
{
	if (g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_ACT)
		return FALSE;
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)
		return FALSE;

	BOOL	ret = FALSE;
	BYTE	pkt[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	// PK_CMD_ROOM_MV
	// size			: 2	0
	// header		: 1	2
	// mv_x			: 1	3	(E_TYPE_USER_DIRECTION)
	// footer		: 2	4

	// 向き
	E_TYPE_USER_DIRECTION dir = USER_DIRECTION_LEFT;
	memcpy(&dir, &data[PACKET_ROOM_MV_INDEX], sizeof(E_TYPE_USER_DIRECTION));

	E_TYPE_USER_DIRECTION old_dir = sess->dir;
	sess->dir = dir;
	sess->vx = 0;
	sess->vy = 0;
	sess->MV_c = max(0, sess->MV_c-1);

	if (old_dir != sess->dir)
		sess->angle = (sess->angle+180)%360;

	// 位置の送信
	packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
	if (packetSize)
	{
		if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
			ret = AddPacketTeamUser(sess->team_no, pkt, packetSize);
		else
			ret = AddPacketNoBlindUser(sess, pkt, packetSize);
	}
	return ret;
}

BOOL CPacketProcMain::UpdatePos(ptype_session sess, BYTE* data)
{
	if (g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_ACT)
		return FALSE;
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)
		return FALSE;

	BOOL	ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	// PK_CMD_ROOM_MV
	// size			: 2	0
	// header		: 1	2
	// mv_x			: 1	3	(short)
	// footer		: 2	5

	// 移動値(-1,0,1)
	short mvecx = 0;
	memcpy(&mvecx, &data[PACKET_ROOM_MV_INDEX], sizeof(short));

	sess->vx = mvecx;
	sess->vy = 0;

	if (sess->vx != 0)
	{
		E_TYPE_USER_DIRECTION old_dir = sess->dir;
		sess->dir = (sess->vx>0)?USER_DIRECTION_RIGHT:USER_DIRECTION_LEFT;
		// 角度の反転が必要か
		if (old_dir != sess->dir)
			sess->angle = (sess->angle+180)%360;
#if 1
	}
	else if (g_pLogFile)	// 移動値設定確認
	{
		WCHAR msglog[80];
		SafePrintf(msglog, 80, L"ChrStop#%d(px:%d,py%d,vx%d,vy%d)",sess->sess_index, sess->ax, sess->ay, sess->vx, sess->vy);
		AddMessageLog(msglog);
	}
#else
	}
#endif

	// 移動不可状態
	if (sess->chara_state[CHARA_STATE_NOMOVE_INDEX])
		sess->vx = 0;

	// 位置の送信
	packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
	if (packetSize)
	{
		if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
			ret = AddPacketTeamUser(sess->team_no, pkt, packetSize);
		else
			ret = AddPacketNoBlindUser(sess, pkt, packetSize);
	}
	return ret;
}

// angle				: 角度
// power			: 初速
// blt_type			: 弾の種類
BOOL CPacketProcMain::SetShot(ptype_session sess, BYTE* data)
{
	AddMessageLog(L"PK_CMD_MAIN_SHOT");
	if ( g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_TRIGGER)
		return FALSE;
	if ( sess->obj_state != OBJ_STATE_MAIN_ACTIVE)
		return FALSE;

	BOOL	ret = FALSE;
	// 角度
	memcpy(&m_sSaveShotAngle, &data[3], sizeof(short));
	// 初速
	m_nSaveShotPower = min(data[5],MAX_SHOT_POWER);
	// 弾の種類
	m_nSaveBltType = data[6];
	// 処理
	m_nSaveProcType = data[7];
	// インジケーター角度
	memcpy(&m_sSaveIndicatorAngle, &data[8], sizeof(short));
	
	// インジケーターパワー
	m_nSaveIndicatorPower = data[10];
	// キャラの種類
	m_nSaveCharaType = sess->chara_type;
	//キャラオブジェクト番号
	m_nSaveCharaObjNo = sess->obj_no;

	m_pSaveScrInfo = (TCHARA_SCR_INFO*)sess->scrinfo;

	// パワーアップを使用状態に設定
	if (sess->chara_state[CHARA_STATE_POWER_INDEX])
	{
		sess->chara_state[CHARA_STATE_POWER_INDEX] = CHARA_STATE_POWERUP_USE;
		BYTE		pkt[MAX_PACKET_SIZE];
		INT packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_POWER_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
	}

	ret |= ShootingBullet(sess, 0);

	return ret;
}

// 発射処理
BOOL CPacketProcMain::ShootingBullet(ptype_session sess, int nFrame)
{
	BOOL ret = FALSE;
	g_pCriticalSection->EnterCriticalSection_Object(L'2');

	switch (m_nSaveProcType)
	{
	case BLT_PROC_TYPE_SCR_CHARA:
		{
			if (common::scr::CallShootingFunc(g_pLuah, sess, m_nSaveProcType, m_nSaveBltType, (int)m_sSaveShotAngle, m_nSaveShotPower, m_pSaveScrInfo, m_nSaveCharaObjNo, nFrame, (int)m_sSaveIndicatorAngle, m_nSaveIndicatorPower, g_pCriticalSection))
				g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOT);	// 次フェーズ
			else if (!nFrame)
				g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOOTING);
		}
		break;
	case BLT_PROC_TYPE_SCR_SPELL:
		{
			// フレーム0なら弾発射によるディレイ値増加
			if (!nFrame)
			{
				sess->EXP_c = 0;
				WCHAR log[32];
				SafePrintf(log, 32,L"shooting frame:%d", sess->frame_count);
				AddMessageLog(log);
			}
			if (common::scr::CallShootingFunc(g_pLuah, sess, m_nSaveProcType, m_nSaveBltType, (int)m_sSaveShotAngle, m_nSaveShotPower, m_pSaveScrInfo, m_nSaveCharaObjNo, nFrame, (int)m_sSaveIndicatorAngle, m_nSaveIndicatorPower, g_pCriticalSection))
				g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOT);	// 次フェーズ
			else if (!nFrame)
				g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOOTING);
		}
		break;
	case BLT_PROC_TYPE_ITEM:
		{
			POINT pnt;
			// 体の中心より高めで発射させる
			short sHeadAngle = 90;
			if (sess->dir != USER_DIRECTION_LEFT)
				sHeadAngle = 270;
			double dRad = D3DXToRadian( (sess->angle+sHeadAngle)%360);

			double dBodyOffsetX = cos(dRad) * (CHARA_BODY_RANGE*1.5);
			double dBodyOffsetY = sin(dRad) * (CHARA_BODY_RANGE*1.5);
			// ラジアンから各方向を取り出す
			dRad = D3DXToRadian(m_sSaveShotAngle);
			double dx = cos(dRad);
			double dy = sin(dRad);
			pnt.x = (LONG)(dx * (float)m_nSaveShotPower) * BLT_VEC_FACT_N;
			pnt.y = (LONG)(dy * (float)m_nSaveShotPower) * BLT_VEC_FACT_N;
			// フレーム0なら弾発射によるディレイ値増加、使用アイテムクリア
			if (!nFrame)
			{
				BYTE		pkt[MAX_PACKET_SIZE];	// パケットデータ
				INT		packetSize = 0;
				// 使用アイテムクリアパケット
				sess->chara_state[m_nSaveBltType] = 0;
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, m_nSaveBltType, pkt);
				ret |= AddPacketAllUser(NULL, pkt, packetSize);
			}

			int nFirstRange = (CHARA_BODY_RANGE+BLT_DEFAULT_HITRANGE)+1;
			int nFirstX = (int)(sess->ax+(dx*nFirstRange)-dBodyOffsetX); //;(pScrInfo->rec_tex_chr.right/2));
			int nFirstY = (int)(sess->ay+(dy*nFirstRange)-dBodyOffsetY);//*(pScrInfo->rec_tex_chr.bottom/2));

			type_blt blt;
			ZeroMemory(&blt, sizeof(type_blt));
			blt.chr_obj_no = (short)m_nSaveCharaObjNo;
			blt.adx = BLT_DEFAULT_ADDVEC_X;								// 加速値
			blt.ady = BLT_DEFAULT_ADDVEC_Y;
			blt.chara_type = (BYTE)m_nSaveCharaType;
			blt.bullet_type = (BYTE)m_nSaveBltType;
			blt.obj_type = (E_OBJ_TYPE)(OBJ_TYPE_ITEM|OBJ_TYPE_BLT_SOLID);
			blt.proc_type = BLT_PROC_TYPE_ITEM;
			blt.ax = (short)nFirstX;
			blt.ay =(short) nFirstY;
			blt.vx = (short)pnt.x;
			blt.vy = (short)pnt.y;
			blt.hit_range = (BYTE)BLT_DEFAULT_HITRANGE;
			blt.extdata1 = 0;
			blt.extdata2 = 0;
			blt.obj_state = OBJ_STATE_MAIN_ACTIVE;
			g_pSyncMain->AddObject( &blt );
			g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOT);
		}
		break;
	}
	g_pCriticalSection->LeaveCriticalSection_Object();

	return ret;
}

BOOL CPacketProcMain::SetAck(ptype_session sess)
{
	WCHAR acklog[MAX_USER_NAME+16+1];
	WCHAR name[MAX_USER_NAME+1];
	common::session::GetSessionName(sess, name);
	SafePrintf(acklog, MAX_USER_NAME+16, L"sync.%s", name);
	AddMessageLog(acklog);
	sess->game_ready = TRUE;
	return FALSE;
}

BOOL CPacketProcMain::TurnPass(ptype_session sess)
{
	// アクティブキャラのみ受け入れる
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)	return FALSE;
// CSyncMain::NotifyTurnEndに移動
//	// 経過時間によるディレイ値増加
//	int nTimeOfDelay = sess->frame_count?((int)( ((float)sess->frame_count / (float)MAX_PASSAGE_TIME)*(float)MAX_PASSAGE_TIME_DELAY_VALUE)):0;
//	sess->delay += nTimeOfDelay;
	// フェーズ変更
	g_pSyncMain->SetPhase(GAME_MAIN_PHASE_CHECK);
	return FALSE;
}

// アイテム使用要求
BOOL CPacketProcMain::OrderItem(ptype_session sess, BYTE* data)
{
	BOOL ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	// アクティブキャラ以外
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)	return FALSE;
	// 使用アイテムインデックス
	int nItemIndex = data[3];
	// アイテムフラグ
	DWORD dwItemFlg = sess->items[nItemIndex];
	// フラグ有効範囲確認
	if (GAME_ITEM_ENABLE_FLG & dwItemFlg)
	{
		WCHAR logmsg[32];
		SafePrintf(&logmsg[0], 32, L"OrderItem[%d:%x]", nItemIndex, dwItemFlg);
		AddMessageLog(logmsg);

		BOOL bSteal = FALSE;
		ptype_session pStealSess = g_pSyncMain->GetStealSession();
		// スティール使用中キャラが居る場合、スティール発動
		if (pStealSess && (pStealSess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] & GAME_ITEM_STEAL_USING))
		{
			bSteal = TRUE;
			ret = StealItem(pStealSess, dwItemFlg);
		}

		// アイテム情報クリア
		sess->items[nItemIndex] = 0x0;
		packetSize = PacketMaker::MakePacketData_MainInfoItemSelect(nItemIndex, dwItemFlg, pktdata, bSteal);
		if (packetSize)
			ret |= AddPacket(sess, pktdata, packetSize);
	
		if (!bSteal)
			ret |= UseItem(sess, dwItemFlg);
	}
	
	return ret;
}

// アイテムを盗む
BOOL CPacketProcMain::StealItem(ptype_session sess, DWORD item_flg)
{
	BOOL ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	// 空きスロットを探す
	int nSlot = 0;
	for (nSlot=0;nSlot<GAME_ITEM_STOCK_MAX_COUNT;nSlot++)
	{
		if (!sess->items[nSlot])	// 空きスロット
			break;
	}

	// 空きが無い場合
	if (nSlot >= GAME_ITEM_STOCK_MAX_COUNT)
		return (BOOL)g_pSyncMain->UpdateCharaStatus(sess->obj_no,0,0,0,GAME_ADD_ITEM_ENOUGH_EXP);
	
	// アイテム追加
	sess->items[nSlot] = item_flg;
	// パケット送信
	packetSize = PacketMaker::MakePacketData_MainInfoAddItem(sess->obj_no,nSlot, item_flg, pkt, TRUE);
	ret = AddPacket(sess, pkt, packetSize);
	
	// スティール状態を解除
	g_pSyncMain->SetStealSession(NULL);
	sess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] = GAME_ITEM_STEAL_OFF;
	packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_STEAL_INDEX, pkt);
	ret |= AddPacketAllUser(NULL, pkt, packetSize);
	return ret;
}

// アイテム使用
BOOL CPacketProcMain::UseItem(ptype_session sess, DWORD item_flg)
{
	BOOL ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	switch (item_flg)
	{
	case GAME_ITEM_MOVE_UP:											// 移動値増加
		sess->delay += /*(((TCHARA_SCR_INFO*)sess->scrinfo)->delay-GAME_CHARA_START_DELAY_RND_MIN)+*/GAME_ITEM_MOVE_UP_DELAY;
		sess->MV_m += GAME_ITEM_MOVE_UP_VALUE;
		sess->MV_c += GAME_ITEM_MOVE_UP_VALUE;
		WCHAR logmsg[32];
		SafePrintf(&logmsg[0], 32, L"MvUp[%d:%d]", sess->MV_m, sess->MV_c);
		AddMessageLog(logmsg);
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
		ret = AddPacket(sess, pkt, packetSize);
		break;
	case GAME_ITEM_DOUBLE_SHOT:									// 連射
		sess->delay += GAME_ITEM_DOUBLE_DELAY;
		sess->chara_state[CHARA_STATE_DOUBLE_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_DOUBLE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_STEALTH:											// ステルス
		sess->delay += GAME_ITEM_STEALTH_DELAY;
		sess->chara_state[CHARA_STATE_STEALTH_INDEX] = GAME_ITEM_STEALTH_TURN_COUNT;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_STEALTH_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		// 20101105
//		g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TURNEND);	// ターン終了
		break;
	case GAME_ITEM_REVERSE:										// 逆さアイテム発射前状態
		sess->delay += GAME_ITEM_REVERSE_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_REVERSE_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_REVERSE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_BLIND:											// 暗くなる弾発射前状態
		sess->delay += GAME_ITEM_BLIND_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_BLIND_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_BLIND_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_SHIELD:											// シールド
		sess->delay += /*(((TCHARA_SCR_INFO*)sess->scrinfo)->delay-GAME_CHARA_START_DELAY_RND_MIN)+*/GAME_ITEM_SHIELD_DELAY;
		sess->chara_state[CHARA_STATE_SHIELD_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_SHIELD_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_POWER:													// パワーアップ
		sess->delay += GAME_ITEM_POWER_DELAY;
		sess->chara_state[CHARA_STATE_POWER_INDEX] = CHARA_STATE_POWERUP_ON;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_POWER_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_NOANGLE:													// 角度変更不可
		sess->delay += GAME_ITEM_NOANGLE_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_NOANGLE_INDEX] = GAME_ITEM_NOANGLE_TURN_COUNT;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_NOANGLE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_NOMOVE:													// 移動不可
		sess->delay += GAME_ITEM_NOMOVE_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_NOMOVE_INDEX] = GAME_ITEM_NOMOVE_TURN_COUNT;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_NOMOVE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_REPAIR:														// 回復
		sess->delay += /*(((TCHARA_SCR_INFO*)sess->scrinfo)->delay-GAME_CHARA_START_DELAY_RND_MIN)+*/GAME_ITEM_REPAIR_DELAY;
		UpdateHPStatus(sess, (int)(sess->HP_m * GAME_ITEM_REPAIR_RATE));
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_REPAIR_BULLET:										// 回復弾
		sess->delay += GAME_ITEM_REPAIR_BULLET_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_REPAIRBLT_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_REPAIRBLT_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_TELEPORT:													// 移動弾
		sess->delay += GAME_ITEM_TELEPORT_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_TELEPORT_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_TELEPORT_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_REPAIR_BIG:												// 回復(大幅)
		sess->delay += GAME_ITEM_REPAIR_BIG_DELAY;
		UpdateHPStatus(sess, (int)(sess->HP_m * GAME_ITEM_REPAIR_BIG_RATE));
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		// 移動中の場合、移動値を0にして位置の送信
		if (sess->vx != 0)
		{
			sess->vx = 0;
			packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
			if (packetSize)
			{
				if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
					ret = AddPacketTeamUser(sess->team_no, pkt, packetSize);
				else
					ret = AddPacketNoBlindUser(sess, pkt, packetSize);
			}
		}
		g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TURNEND);	// ターン終了
		break;
	case GAME_ITEM_REPAIR_TEAM:											// 回復(チーム)
		{
			sess->delay += GAME_ITEM_REPAIR_TEAM_DELAY;
			int nSearchIndex = 0;
			for(ptype_session pTeamSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
				pTeamSess;
				pTeamSess=p_pNWSess->GetSessionNext(&nSearchIndex))
			{
				if (pTeamSess->connect_state != CONN_STATE_AUTHED
				|| !pTeamSess->entity															// 存在しないセッション
				|| pTeamSess->obj_state == OBJ_STATE_MAIN_DEAD			// 死んでる
				|| pTeamSess->obj_state == OBJ_STATE_MAIN_DROP			// 死んでる
				|| pTeamSess->team_no != sess->team_no)							// 別チーム
					continue;
				UpdateHPStatus(pTeamSess, (int)(pTeamSess->HP_m * GAME_ITEM_REPAIR_TEAM_RATE));
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(pTeamSess, pkt);
				ret |= AddPacketAllUser(NULL, pkt, packetSize);
			}
			g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TURNEND);	// ターン終了
			break;
		}
	case GAME_ITEM_DRAIN:													// 吸収弾
		sess->delay += GAME_ITEM_DRAIN_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_DRAIN_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_DRAIN_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_FETCH:
		sess->delay += GAME_ITEM_FETCH_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_FETCH_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_FETCH_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_EXCHANGE:
		sess->delay += GAME_ITEM_EXCHANGE_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_EXCHANGE_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_EXCHANGE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_WIND_CHANGE:											// 風向き変更
		sess->delay += (short)GAME_ITEM_WIND_CHANGE_DELAY;
		g_pSyncMain->SetWind(-g_pSyncMain->GetWind(), -g_pSyncMain->GetWindDirection());
//		WCHAR logmsg[32];
		SafePrintf(&logmsg[0], 32, L"WindChange[%d](%d)", g_pSyncMain->GetWind(),g_pSyncMain->GetWindDirection());
		AddMessageLog(logmsg);
		break;
	case GAME_ITEM_STEAL:
		sess->delay += (short)GAME_ITEM_STEAL_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] = GAME_ITEM_STEAL_SET;
		// SyncMainに設定
		g_pSyncMain->SetStealSession(sess);
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_STEAL_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		AddMessageLog(L"SetSteal");
		break;
		break;
	case GAME_ITEM_20:
		break;
	}

	return ret;
}

BOOL CPacketProcMain::SetTrigger(ptype_session sess,BYTE* data)
{
	BOOL ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	// フェーズ確認	20101223
	if (g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_ACT)	return FALSE;

	g_pCriticalSection->EnterCriticalSection_Session(L'7');
	// 認証済みアクティブキャラのみ受け入れる
	if (sess->connect_state != CONN_STATE_AUTHED)
	{
		g_pCriticalSection->LeaveCriticalSection_Session();
		return FALSE;
	}

	if ( (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)							// アクティブなセッションか
	|| (g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_ACT)				// アクティブなフェーズか
	|| (!g_pSyncMain->IsShotPowerWorking())											//	ショットパワーチャージ中か
	|| (g_pSyncMain->GetPhaseTimeCount() > GAME_MAIN_PHASE_TIME_SHOTPOWER)	// 時間外の要求
	)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoRejShot(sess->obj_no, pkt);
		if (packetSize)
			ret = AddPacket(sess, pkt, packetSize);
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	}

	int nIndex = 3;
	// キャラ番号
	short nCharaIndex;
	memcpy(&nCharaIndex, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// スクリプト/アイテムのタイプ
	int nProcType = data[nIndex];
	nIndex++;
	// 演出タイプ
	int nBltType = data[nIndex];
	nIndex++;
	// 角度
	short sShotAngle = 0;
	memcpy(&sShotAngle, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// パワー
	short sShotPower = 0;
	memcpy(&sShotPower, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// indicator角度
	short sShotIndicatorAngle = 0;
	memcpy(&sShotIndicatorAngle, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// indicatorパワー
	short sShotIndicatorPower = 0;
	memcpy(&sShotIndicatorPower, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);

	// トリガー拒否
	BOOL bRejTrigger = FALSE;
	// 処理タイプ
	switch (nProcType)
	{
	case BLT_PROC_TYPE_ITEM:
		if (!sess->chara_state[nBltType])
			bRejTrigger = TRUE;
		break;
	case BLT_PROC_TYPE_SCR_CHARA:
		if (nBltType > ((TCHARA_SCR_INFO*)sess->scrinfo)->blt_sel_count && nBltType < 0)
			bRejTrigger = TRUE;
		else if (sess->vy != 0)
			bRejTrigger = TRUE;
		break;
	case BLT_PROC_TYPE_SCR_SPELL:
		if (nBltType == MAX_CHARA_BULLET_TYPE && sess->EXP_c < ((TCHARA_SCR_INFO*)sess->scrinfo)->sc_info.max_exp)
			bRejTrigger = TRUE;
		else if (sess->vy != 0)
			bRejTrigger = TRUE;
		break;
	case BLT_PROC_TYPE_SCR_STAGE:
	default:
		bRejTrigger = TRUE;
		break;
	}

	if (bRejTrigger)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoRejShot(sess->obj_no, pkt);
		if (packetSize)
			ret = AddPacket(sess, pkt, packetSize);
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	}

	m_nSaveCharaObjNo = nCharaIndex;
	m_nSaveBltType = nBltType;
	m_nSaveProcType = nProcType;
	m_sSaveShotAngle = sShotAngle;
	m_nSaveShotPower = sShotPower;
	m_sSaveIndicatorAngle = sShotIndicatorAngle;
	m_nSaveIndicatorPower = sShotIndicatorPower;
	m_pSaveScrInfo = (TCHARA_SCR_INFO*)sess->scrinfo;

	// 接続済みユーザの状態を切り替える
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		if (!pSess->entity)	continue;

		// 待ち
		pSess->game_ready = 0;
	}

	// 移動中の場合、移動値を0にして位置の送信
	if (sess->vx != 0)
	{
		sess->vx = 0;
		packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
		if (packetSize)
		{
			if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
				ret = AddPacketTeamUser(sess->team_no, pkt, packetSize);
			else
				ret = AddPacketNoBlindUser(sess, pkt, packetSize);
		}
	}

	packetSize = MakeTriggerPacket(sess, pkt);
	if (packetSize)
	{
		if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
			ret = AddPacketTeamNoBlindUser(sess->team_no, pkt, packetSize);
		else
			ret = AddPacketNoBlindUser(sess, pkt, packetSize);
	}
	// フェーズ変更
	g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TRIGGER);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return ret;
}

bool CPacketProcMain::ShootingCommand(ptype_session sess, int nProcType, int nCharaID, int nBltType,int nAngle,int nPower, int nChrObjNo, int nFrame, int indicator_angle, int indicator_short)
{
	std::map < int, TCHARA_SCR_INFO >::iterator itfind = g_mapCharaScrInfo.find(nCharaID);
	if (itfind == g_mapCharaScrInfo.end())
	{
		AddMessageLog(L"!Shooting ScrID error");
		g_pSyncMain->SetPhase(GAME_MAIN_PHASE_TURNEND);
		return true;	// 発射終了
	}

	bool ret = common::scr::CallShootingFunc(g_pLuah, sess, nProcType, nBltType, nAngle, nPower, &((*itfind).second), nChrObjNo, nFrame, indicator_angle, indicator_short, g_pCriticalSection)?true:false;
	if (ret)
	{
		sess->delay += (*itfind).second.blt_info->blt_delay;
		g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOT);	// 次フェーズ
	}
	else if (!nFrame)
		g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOOTING);

	return ret;
}

INT CPacketProcMain::MakeTriggerPacket(ptype_session sess, BYTE* data)
{
	INT ret = 0;
	ret = PacketMaker::MakePacketData_MainInfoTrigger(sess->obj_no, m_nSaveProcType, m_nSaveBltType, (int)m_sSaveShotAngle, m_nSaveShotPower, (int)m_sSaveIndicatorAngle, m_nSaveIndicatorPower, data);
	return ret;
}


// 連射するか確認
BOOL CPacketProcMain::IsDoubleShot(ptype_session sess)
{
	BOOL ret = TRUE;

	// 連射に状態設定されているか
	if ( !sess || !sess->chara_state[CHARA_STATE_DOUBLE_INDEX])
		return FALSE;
	
	// 別キャラが引数の場合FALSEを返す
	if (sess->obj_no != m_nSaveCharaObjNo)
		return FALSE;

	switch (m_nSaveProcType)
	{
	case BLT_PROC_TYPE_SCR_CHARA:
	case BLT_PROC_TYPE_ITEM:
		break;
	case BLT_PROC_TYPE_SCR_SPELL:	// スペルは連射不可
	default:
		ret = FALSE;
		break;
	}

	return ret;
}

// パケット：自ターンパス
BOOL CPacketProcMain::SetTurnPass(ptype_session sess,BYTE* data)
{
	BOOL ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	g_pCriticalSection->EnterCriticalSection_Session(L'8');
	// 未認証受け入れない
	if (sess->connect_state != CONN_STATE_AUTHED)
	{
		g_pCriticalSection->LeaveCriticalSection_Session();
		return FALSE;
	}
	// 非アクティブ、落下中は受け入れない
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE || sess->vy != 0)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoRejTurnPass(sess, pkt);
		if (packetSize)
			ret = AddPacket(sess, pkt, packetSize);
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	}

	// 移動中の場合、移動値を0にして位置の送信
	if (sess->vx != 0)
	{
		sess->vx = 0;
		packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
		if (packetSize)
		{
			if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
				ret = AddPacketTeamUser(sess->team_no, pkt, packetSize);
			else
				ret = AddPacketNoBlindUser(sess, pkt, packetSize);
		}
	}
	// パスした場合は時間経過を0にする
	sess->frame_count = 0;
	
	g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TURNEND);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return ret;
}

BOOL CPacketProcMain::RcvTriggerEnd(ptype_session sess,BYTE* data)
{
	if (g_pSyncMain && g_pSyncMain->GetPhase() == GAME_MAIN_PHASE_TRIGGER)
	{
		g_pCriticalSection->EnterCriticalSection_Session(L'0');

		WCHAR log[MAX_USER_NAME+32+1];
		WCHAR name[MAX_USER_NAME+1];
		common::session::GetSessionName(sess, name);
		SafePrintf(log, MAX_USER_NAME+32, L"RcvTriggerEnd.%s", name);
		AddMessageLog(log);

		sess->game_ready = 1;
		g_pCriticalSection->LeaveCriticalSection_Session();
	}

	return TRUE;
}
	
BOOL CPacketProcMain::SetShotPowerStart(ptype_session sess, BYTE* data)
{
	BOOL ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	// 認証済みアクティブキャラのみ受け入れる
	if (sess->connect_state != CONN_STATE_AUTHED)	return FALSE;
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoRejShot(sess->obj_no, pkt);
		if (packetSize)
			ret = AddPacket(sess, pkt, packetSize);
		return ret;
	}

	int nIndex = 3;
	// キャラ番号
	short nCharaIndex;
	memcpy(&nCharaIndex, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// スクリプト/アイテムのタイプ
	int nProcType = data[nIndex];
	nIndex++;
	// 演出タイプ
	int nBltType = data[nIndex];
	nIndex++;
	// 角度
	short sShotAngle = 0;
	memcpy(&sShotAngle, &data[nIndex], sizeof(short));
	nIndex+= sizeof(short);

	BOOL bRejTrigger = FALSE;

	if (sess->vy != 0)
		bRejTrigger = TRUE;
	else
	{
		// 処理タイプ
		switch (nProcType)
		{
		case BLT_PROC_TYPE_ITEM:
			if (!sess->chara_state[nBltType])
				bRejTrigger = TRUE;
			break;
		case BLT_PROC_TYPE_SCR_CHARA:
			if (nBltType > ((TCHARA_SCR_INFO*)sess->scrinfo)->blt_sel_count && nBltType < 0)
				bRejTrigger = TRUE;
			else if (sess->vy != 0)
				bRejTrigger = TRUE;
			break;
		case BLT_PROC_TYPE_SCR_SPELL:
			if (nBltType == MAX_CHARA_BULLET_TYPE && sess->EXP_c < ((TCHARA_SCR_INFO*)sess->scrinfo)->sc_info.max_exp)
				bRejTrigger = TRUE;
			else if (sess->vy != 0)
				bRejTrigger = TRUE;
			break;
		case BLT_PROC_TYPE_SCR_STAGE:
		default:
			bRejTrigger = TRUE;
			break;
		}
	}

	if (bRejTrigger)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoRejShot(sess->obj_no, pkt);
		if (packetSize)
			ret = AddPacket(sess, pkt, packetSize);
		return ret;
	}

	// 移動中の場合、移動値を0にして位置の送信
	if (sess->vx != 0)
	{
		sess->vx = 0;
		packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
		if (packetSize)
		{
			if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
				ret = AddPacketTeamUser(sess->team_no, pkt, packetSize);
			else
				ret = AddPacketNoBlindUser(sess, pkt, packetSize);
		}
	}
	
	// 保存
	m_nSaveCharaObjNo = nCharaIndex;
	m_nSaveBltType = nBltType;
	m_nSaveProcType = nProcType;
	m_sSaveShotAngle = sShotAngle;
	m_nSaveShotPower = 0;
	m_pSaveScrInfo = (TCHARA_SCR_INFO*)sess->scrinfo;

	// フェーズ変更
	g_pSyncMain->SetShotPowerStart(sess, nBltType);

	return ret;
}
