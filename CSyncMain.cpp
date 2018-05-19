#include "CSyncMain.h"
#include "main.h"
#include "ext.h"

#ifdef _DEBUG
#define SC_NOLIMIT	1			// スペカ撃ち放題
#endif
#define START_INTERVAL	1	// 1ターン目のインターバル

BOOL SessionNo_Asc( ptype_session left, ptype_session right ) {
	return left->frame_count < right->frame_count;
}

CSyncMain::CSyncMain() : CSyncProc()
{
	m_mapObjects.clear();
	m_vecObjectNo.clear();
	p_pActiveSession = NULL;
	p_pStealSess = NULL;
	m_nPhaseReturnIndex = 0;
	m_nObjectNoCounter = 0;
	m_pMainStage = new CMainStage();
	m_vecCharacters.clear();
	SetPhase(GAME_MAIN_PHASE_NONE);
	m_nWind = 0;
	m_nWindDirection = 0;
	m_bReqShotFlg = FALSE;
	m_pStageScrInfo = NULL;
	m_nTurnCount = 0;
	m_nWindDirValancer = 0;
	m_nPhaseSyncIndex = 0;
	m_nTurnLimit = 0;
	m_bDoubleShotFlg = FALSE;
	m_bKillGame = FALSE;
	ZeroMemory(m_nLivingTeamCountTable, sizeof(int)*MAXUSERNUM);
}

CSyncMain::~CSyncMain()
{
	Clear();
	SafeDelete(m_pStageScrInfo);
	ClearQueue(m_tQueue.next);
}

void CSyncMain::ClearGameReady()
{
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();it != m_vecCharacters.end();it++)
		(*it)->game_ready = 0;
}

BOOL CSyncMain::Init(CNetworkSession* pNetSess, CFiler* pFiler, CPacketProcPhase* pPPPhase)
{
	ZeroMemory(&m_tQueue, sizeof(type_queue));
	Clear();

	ZeroMemory(m_nLivingTeamCountTable, sizeof(int)*MAXUSERNUM);

	TSTAGE_SCR_INFO* pStageScrInfo = common::scr::FindStageScrInfoFromStageIndex(pPPPhase->GetStageIndex(), &g_mapStageScrInfo);
	if (!pStageScrInfo)
	{
		MessageBox(g_hWnd, L"不正なステージが選択されました。", L"error", MB_OK);
		return FALSE;
	}

	SafeDelete(m_pStageScrInfo);
	m_pStageScrInfo = new TSTAGE_SCR_INFO();
	memcpy(m_pStageScrInfo, pStageScrInfo, sizeof(TSTAGE_SCR_INFO));

	BOOL ret = FALSE;
	ret = m_pMainStage->Init(pFiler, &pStageScrInfo->stage.path[0], &pStageScrInfo->stage.size);

	if (!ret)
	{
		SafeDelete(m_pStageScrInfo);
		MessageBox(g_hWnd, L"ステージロード失敗しました。\ndataフォルダの構成を確認してください", L"error", MB_OK);
		return FALSE;
	}

	m_nTurnLimit = pPPPhase->GetTurnLimit();
	m_bytRule = pPPPhase->GetRuleFlg();
	// 生存者テーブルをセット
	int nTeamCount = pPPPhase->GetTeamCount();
	int nEntityCount = pNetSess->CalcEntityUserCount();
//> 20101105
	// チームバトル
//	if (nTeamCount > 1)
//	{
//		for (int i=0;i<nTeamCount;i++)
//			m_nLivingTeamCountTable[i] = (int)(nEntityCount/nTeamCount);
//	}
	ZeroMemory(m_nLivingTeamCountTable, sizeof(int)*MAXUSERNUM);
//< 20101105

	g_pCriticalSection->EnterCriticalSection_Session(L'-');
	t_sessionInfo* ptbl = pNetSess->GetSessionTable();
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session pSess = &ptbl[i].s;
		// 存在確認
		pSess->obj_no = (short)MAXUSERNUM;
		if (!pSess->entity)	continue;
		AddObject((type_obj*)pSess);
		pSess->game_ready = 0;

		// 生存確認してテーブル更新
		if (!(pSess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY|OBJ_STATE_MAIN_GALLERY) ))
		{
			if (nTeamCount > 1)
				m_nLivingTeamCountTable[pSess->team_no]++;
		}
	}
/*
	int nSearchIndex = 0;
	for(ptype_session pSess=pNetSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=pNetSess->GetSessionNext(&nSearchIndex))
	{
		// 存在確認
		pSess->obj_no = MAXUSERNUM;
		if (!pSess->entity)	continue;
		AddObject((type_obj*)pSess);
		pSess->game_ready = 0;

		// 生存確認してテーブル更新
		if (!(pSess->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG))
		{
			if (nTeamCount > 1)
				m_nLivingTeamCountTable[pSess->team_no]++;
		}
	}
*/
	g_pCriticalSection->LeaveCriticalSection_Session();

	// ルール：風向きありなし確認、風方向ランダム
	m_nWind = 0;
#ifndef NO_WIND
	if (m_bytRule & GAME_RULE_WIND_ENABLE)
		m_nWind = (genrand_int32()%(RND_WIND_VALUE)) + MIN_WIND_VALUE;
	
	if (m_bytRule & GAME_RULE_WIND_ENABLE)
		m_nWindDirection = (genrand_int32()%(MAX_WIND_DIR_VALUE-MIN_WIND_DIR_VALUE)) + MIN_WIND_DIR_VALUE;
#endif
	SetPhase(GAME_MAIN_PHASE_TURNEND);
	m_eGameNextPhase = GAME_MAIN_PHASE_RETURN;
	m_nTurnCount = 0;
	m_bDoubleShotFlg = FALSE;
	m_bReqShotFlg = FALSE;
	m_nPhaseReturnIndex = 0;
	m_bStartIntervalEnd = FALSE;

	ret = CSyncProc::Init(pNetSess);
	if (ret)
		ret = LoadStage();
	if (!ret)
		SafeDelete(m_pStageScrInfo);

	m_bKillGame = FALSE;

	if (ret)
	{
		AddMessageLog(L"GAME_START");
	}
	return ret;
}

BOOL CSyncMain::LoadStage()
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 5;
	int nUserNum = 0;

	std::vector<int> vecGroundXPos;
	if (!m_pMainStage->GetGroundsXPos(&vecGroundXPos))
	{
		AddMessageLog(L"地形データが不正なステージ画像です");
		MessageBox(NULL, L"地形データが不正なステージ画像です", L"error", MB_OK);
		return FALSE;
	}

	g_pCriticalSection->EnterCriticalSection_Session(L'^');
	// 接続済みユーザの状態を切り替える
	t_sessionInfo* ptbl = p_pNWSess->GetSessionTable();
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session pSess = &ptbl[i].s;
//		if (!pSess->entity) continue;
//		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		// 切り替え
		if (pSess->team_no == GALLERY_TEAM_NO)
		{
			pSess->obj_state = OBJ_STATE_MAIN_GALLERY;
			pSess->entity = 0;
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		}
		else
		{
			if (pSess->connect_state == CONN_STATE_AUTHED)
				pSess->obj_state = OBJ_STATE_MAIN_WAIT;
			else if (!pSess->scrinfo)
				continue;
			// ステージに置く
			PutCharacter(&vecGroundXPos, pSess);
		}

		nUserNum++;
		// パケット追加
		packetSize = PacketMaker::AddPacketData_MainStart(packetSize, pSess, pktdata);

		if (pSess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_MAIN_GALLERY_FLG)) continue;

		//LuaFuncParam luaParams,luaResults;
		//// ユーザ番号と位置
		//luaParams.Number(pSess->scrinfo->scr_index).Number(pSess->obj_no).Number(pSess->ax).Number(pSess->ay);
		//if (!common::scr::CallLuaFunc(g_pLuah, "onLoad_Chara", &luaResults, 0, &luaParams, g_pCriticalSection))
		//	return FALSE;
	}

	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session pSess = &ptbl[i].s;
		LuaFuncParam luaParams,luaResults;
		if (pSess->connect_state != CONN_STATE_AUTHED) continue;
		if (pSess->obj_state != OBJ_STATE_MAIN_WAIT) continue;
		// ユーザ番号と位置
		luaParams.Number(pSess->scrinfo->scr_index).Number(pSess->obj_no).Number(pSess->ax).Number(pSess->ay);
		if (!common::scr::CallLuaFunc(g_pLuah, "onLoad_Chara", &luaResults, 0, &luaParams, g_pCriticalSection))
			return FALSE;
	}
/*
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
//		if (!pSess->entity) continue;
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		// 切り替え
		if (pSess->team_no == GALLERY_TEAM_NO)
		{
			pSess->obj_state = OBJ_STATE_MAIN_GALLERY;
		}
		else
		{
			pSess->obj_state = OBJ_STATE_MAIN_WAIT;

			// ステージに置く
			PutCharacter(&vecGroundXPos, pSess);
		}
		nUserNum++;

		// パケット追加
		packetSize = PacketMaker::AddPacketData_MainStart(packetSize, pSess, pktdata);
	}
	// キャラロードイベントを呼ぶループ
	nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		if (!(pSess->entity) || pSess->obj_state == OBJ_STATE_MAIN_DROP) continue;

		LuaFuncParam luaParams,luaResults;
		// ユーザ番号と位置
		luaParams.Number(pSess->scrinfo->scr_index).Number(pSess->obj_no).Number(pSess->ax).Number(pSess->ay);
		if (!common::scr::CallLuaFunc(g_pLuah, "onLoad_Chara", &luaResults, 0, &luaParams, g_pCriticalSection))
			return FALSE;
	}
*/

	if (m_pStageScrInfo)
	{
		LuaFuncParam luaParams,luaResults;
		luaParams.Number(m_pStageScrInfo->scr_index);
		if (!common::scr::CallLuaFunc(g_pLuah, "onLoad_Stage", &luaResults, 0, &luaParams, g_pCriticalSection))
			return FALSE;
	}
	UpdateWMCopyData();	// 20121026
	g_pCriticalSection->LeaveCriticalSection_Session();

	// パケット作成
	if (nUserNum)
	{
		PacketMaker::MakePacketHeadData_MainStart(nUserNum, pktdata);
		packetSize = PacketMaker::AddEndMarker(packetSize, pktdata);
		// 全ユーザに送信
		if (packetSize)
			ret = AddPacketAllUser(NULL, pktdata, packetSize);
	}

	if (g_bOneClient)
	{
		BOOL ret = (nUserNum <= 1);
		if (!ret)
			AddMessageLog(L"ONE_CLIENTオプションが不一致");
		return ret;
	}
	return (nUserNum>1);
}

// キャラクタをおく
void CSyncMain::PutCharacter(std::vector<int>* vecGrounds, ptype_session sess, BOOL PutOnly)
{
	D3DXVECTOR2	vecPos;
	D3DXVECTOR2	vecPut;
	D3DXVECTOR2	vecVec = D3DXVECTOR2(0,0);
	D3DXVECTOR2	vecGround = D3DXVECTOR2(0,0);

	int nRndWidth = vecGrounds->size();
	vecPos.x = (float)( (*vecGrounds)[genrand_int32()%nRndWidth]);
	vecPos.y = (float)(genrand_int32()%g_pSyncMain->GetStageHeight());
	// 地面を探す
	while (!g_pSyncMain->m_pMainStage->FindGround(&vecPut, &vecGround, &vecPos, CHARA_BODY_RANGE))
	{
		vecPos.x = (float)(*vecGrounds)[genrand_int32()%nRndWidth];
		vecPos.y = (float)(genrand_int32()%g_pSyncMain->GetStageHeight());
	}
	sess->ax = (int)vecPut.x;
	sess->ay = (int)vecPut.y;
	
#if 0
	if (!PutOnly)
	{
		sess->ax = 150;
		sess->ay = 50;
	}
#endif
	vecPos = vecPut;
//	sess->dir = (E_TYPE_USER_DIRECTION)(rand()%(int)USER_DIRECTION_MAX);
	sess->dir = (E_TYPE_USER_DIRECTION)(genrand_int32()%(int)USER_DIRECTION_MAX);

	sess->angle = 0;
	vecVec.x = -1;
	if (sess->dir == USER_DIRECTION_RIGHT)
	{
		vecVec.x = 1;
		sess->angle = 180;
	}
	D3DXVECTOR2 vec = vecPos + vecVec; 
	// 地面を探す
	if ( g_pSyncMain->m_pMainStage->FindGround(&vecPut, &vecGround,  &vec, CHARA_BODY_RANGE) )
	{
		vec = vecPut - vecPos;
		sess->angle = (GetAngle(vec.x, vec.y)+REVERSE_ANGLE)%360;
	}

	if (!PutOnly)
	{
		// キャラの初期ステータスなど設定
		TCHARA_SCR_INFO* pCharaScrInfo = common::scr::FindCharaScrInfoFromCharaType(sess->chara_type, &g_mapCharaScrInfo);
		sess->HP_c = sess->HP_m = pCharaScrInfo->max_hp;
		sess->MV_c = sess->MV_m = pCharaScrInfo->move;
		sess->turn_count = 0;
		int nMax = max(pCharaScrInfo->delay,MIN_GAME_CHARA_DELAY_VALUE);
		int nMin = min(pCharaScrInfo->delay,GAME_CHARA_START_DELAY_RND_MIN);
		sess->delay = (short)((double)((short)(genrand_int32()%max(1,(nMax-nMin)))) * 1.9);
		sess->EXP_c = pCharaScrInfo->sc_info.exp;
	}
}

void CSyncMain::SetPhase(E_STATE_GAME_MAIN_PHASE phase)
{
	m_eGameMainPhase = phase;
	switch (phase)
	{
	case GAME_MAIN_PHASE_ACT:
		AddMessageLog(L"GAME_MAIN_PHASE_ACT");
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = g_pPPMain->GetActTimeLimit()*RUN_FRAMES;
//		m_nPhaseTime = GAME_MAIN_PHASE_TIME_ACT;
		m_eGameNextPhase = GAME_MAIN_PHASE_TRIGGER;
		m_bShotPowerWorking = FALSE;
		break;
	case GAME_MAIN_PHASE_TRIGGER:
		AddMessageLog(L"GAME_MAIN_PHASE_TRIGGER");
		m_bReqShotFlg = FALSE;
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = GAME_MAIN_PHASE_TIME_TRIGGER;
		m_eGameNextPhase = GAME_MAIN_PHASE_SHOOTING;
		break;
	case GAME_MAIN_PHASE_DOUBLE:
		AddMessageLog(L"GAME_MAIN_PHASE_DOUBLE");
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = GAME_MAIN_PHASE_TIME_DOUBLE;
		m_eGameNextPhase = GAME_MAIN_PHASE_SHOT;
		break;
	case GAME_MAIN_PHASE_SHOOTING:
		AddMessageLog(L"GAME_MAIN_PHASE_SHOOTING");
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = GAME_MAIN_PHASE_TIME_SHOOTING;
		m_eGameNextPhase = GAME_MAIN_PHASE_SHOT;
		break;
	case GAME_MAIN_PHASE_SHOT:
		AddMessageLog(L"GAME_MAIN_PHASE_SHOT");
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = GAME_MAIN_PHASE_TIME_SHOT;
		m_eGameNextPhase = GAME_MAIN_PHASE_CHECK;
		break;
	case GAME_MAIN_PHASE_CHECK:
		AddMessageLog(L"GAME_MAIN_PHASE_CHECK");
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = GAME_MAIN_PHASE_TIME_CHECK;
		break;
	case GAME_MAIN_PHASE_SYNC:
		AddMessageLog(L"GAME_MAIN_PHASE_SYNC");
		m_nPhaseTimeCounter = 0;
		m_nPhaseSyncIndex = 0;
		m_nPhaseTime = GAME_MAIN_PHASE_TIME_SYNC;
		break;
	case GAME_MAIN_PHASE_TURNEND:
		AddMessageLog(L"GAME_MAIN_PHASE_TURNEND");
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = GAME_MAIN_PHASE_TIME_TURNEND;
		m_eGameNextPhase = GAME_MAIN_PHASE_RETURN;
		break;
	case GAME_MAIN_PHASE_RETURN:
		AddMessageLog(L"GAME_MAIN_PHASE_RETURN");
		m_nPhaseReturnIndex = 0;
		m_nPhaseReturnTimeTotal = 0;
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = (int)GAME_MAIN_PHASE_TIME_RETURN;
		m_eGameNextPhase = GAME_MAIN_PHASE_ACT;
		ClearGameReady();
		break;
	case GAME_MAIN_PHASE_NONE:
	default:
		m_nPhaseTimeCounter = 0;
		m_nPhaseTime = 0;
		break;
	}
}

BOOL CSyncMain::Frame()
{
	BOOL bGameEnd = FALSE;
	BOOL bCharaPacket = FALSE;
	BOOL bObjectPacket = FALSE;
	BOOL bSomeoneLiving = TRUE;
	BOOL bNextPhase = FALSE;

#if START_INTERVAL
	if (!g_bOneClient && !m_bStartIntervalEnd)
	{
		m_nPhaseTimeCounter++;
		if (m_nPhaseTimeCounter > max(FPS*5, (int)((FPS*1.5)*m_vecCharacters.size())))
		{
			AddMessageLog(L"End:StartInterval");
			m_nPhaseTimeCounter = 0;
			m_bStartIntervalEnd = TRUE;
		}
		return (m_bKillGame|bGameEnd);
	}
#endif

	switch (m_eGameMainPhase)
	{
	case GAME_MAIN_PHASE_ACT:
		bNextPhase = TRUE;
		g_pCriticalSection->EnterCriticalSection_Session(L'\\');

		m_nPhaseTimeCounter++;
		if (m_nPhaseTimeCounter < m_nPhaseTime)	// 時間内
		{
			if (p_pActiveSession
			&& p_pActiveSession->entity
			&& (p_pActiveSession->connect_state == CONN_STATE_AUTHED)
			&& (p_pActiveSession->obj_state & OBJ_STATE_GAME)
			&& !(p_pActiveSession->obj_state & (OBJ_STATE_MAIN_NOACT_FLG|OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_MAIN_GALLERY_FLG))
			)
			{
				bNextPhase = FALSE;
				if (!m_bShotPowerWorking)
					p_pActiveSession->frame_count++;
			}
		}
		// 次フェーズ
		if (bNextPhase)
		{
			// 移動中だった場合は移動を止める
			if (p_pActiveSession && p_pActiveSession->vx != 0)
			{
				p_pActiveSession->vx = 0;
				BYTE pkt[MAX_PACKET_SIZE];
				INT packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(p_pActiveSession, pkt);
				if (packetSize)
				{
					if (p_pActiveSession->chara_state[CHARA_STATE_STEALTH_INDEX])
						AddPacketTeamUser(p_pActiveSession->team_no, pkt, packetSize);
					else
						AddPacketNoBlindUser(p_pActiveSession, pkt, packetSize);
				}
			}
			// パワーメータの増加時間がいっぱいなら
			if (p_pActiveSession && m_bShotPowerWorking)
			{
				// 20140810　ショットパワーMAX関連の問題
//				g_pPPMain->SetSaveShotPower(MAX_SHOT_POWER);
				BYTE		pkt[MAX_PACKET_SIZE];
				INT packetSize = g_pPPMain->MakeTriggerPacket(p_pActiveSession, pkt);
				if (packetSize)
				{
					if (p_pActiveSession->chara_state[CHARA_STATE_STEALTH_INDEX])
						AddPacketTeamNoBlindUser(p_pActiveSession->team_no, pkt, packetSize);
					else
						AddPacketNoBlindUser(p_pActiveSession, pkt, packetSize);
				}
				// フェーズ変更
				g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TRIGGER);
			}
			else
			{
				AddMessageLog(L"!ActiveCharaTimeOut");
				SetCheckPhase(GAME_MAIN_PHASE_TURNEND);
//				m_eGameNextPhase = GAME_MAIN_PHASE_TURNEND;
//				SetPhase(GAME_MAIN_PHASE_CHECK);
			}
		}
		else
		{
			FrameCharacters();
			g_pCriticalSection->EnterCriticalSection_Object(L'3');
			FrameObjects();
			g_pCriticalSection->LeaveCriticalSection_Object();
			bGameEnd = IsGameEnd();
		}
		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case GAME_MAIN_PHASE_TRIGGER:
		m_nPhaseTimeCounter++;
		g_pCriticalSection->EnterCriticalSection_Session(L'!');
		// 弾発射キャラがだめになっていないか確認、なっていたらターン終了に以降
		if (p_pActiveSession
		&& (p_pActiveSession->connect_state == CONN_STATE_AUTHED)
		&& (p_pActiveSession->obj_state & OBJ_STATE_GAME)
		&& !(p_pActiveSession->obj_state & (OBJ_STATE_MAIN_NOACT_FLG|OBJ_STATE_MAIN_NOLIVE_FLG))
		)
			FrameTrigger();
		else
			SetPhase(GAME_MAIN_PHASE_TURNEND);

		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case GAME_MAIN_PHASE_SHOOTING:
		g_pCriticalSection->EnterCriticalSection_Session(L'"');
		g_pCriticalSection->EnterCriticalSection_Object(L'4');
		// 弾発射キャラがだめになっていないか確認、なっていたらターン終了に以降
		if (p_pActiveSession
		&& (p_pActiveSession->connect_state == CONN_STATE_AUTHED)
		&& (p_pActiveSession->obj_state & OBJ_STATE_GAME)
		&& !(p_pActiveSession->obj_state & (OBJ_STATE_MAIN_NOACT_FLG|OBJ_STATE_MAIN_NOLIVE_FLG))
		)
		{
			m_nPhaseTimeCounter++;
			g_pPPMain->ShootingBullet(p_pActiveSession, m_nPhaseTimeCounter);

			if (m_nPhaseTimeCounter > m_nPhaseTime	// 時間外
			&& m_eGameMainPhase != GAME_MAIN_PHASE_SHOT)
				SetPhase(GAME_MAIN_PHASE_SHOT);
		}
		else
		{
			WCHAR log[64];
			if (!p_pActiveSession)
				SafePrintf(log, 64, L"!SHOOTING NoActiveSession");
			else
				SafePrintf(log, 64, L"!SHOOTING InvalidActiveSession:%d(state=%x)", p_pActiveSession->obj_no, (DWORD)p_pActiveSession->obj_state);
			AddMessageLog(log);
			SetCheckPhase(GAME_MAIN_PHASE_TURNEND);
		}

		// キャラの移動
		FrameCharacters();
		// 弾の移動
		FrameObjects();
		g_pCriticalSection->LeaveCriticalSection_Object();
		bGameEnd = IsGameEnd();
		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case GAME_MAIN_PHASE_SHOT:
		g_pCriticalSection->EnterCriticalSection_Session(L'#');
		// キャラの移動
		FrameCharacters();
		g_pCriticalSection->EnterCriticalSection_Object(L'5');
		// 弾の移動
		if (!FrameObjects())
		{
			// アクティブな弾が無くなった場合、二発目を撃つか確認
			if ( !m_bDoubleShotFlg && g_pPPMain->IsDoubleShot(p_pActiveSession) )
			{
				SetPhase(GAME_MAIN_PHASE_CHECK);
				m_eGameNextPhase = GAME_MAIN_PHASE_DOUBLE;
			}
			else
			{
				SetPhase(GAME_MAIN_PHASE_CHECK);
				m_eGameNextPhase = GAME_MAIN_PHASE_TURNEND;
			}
		}
		g_pCriticalSection->LeaveCriticalSection_Object();

		bGameEnd = IsGameEnd();
		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case GAME_MAIN_PHASE_DOUBLE:
		if (p_pActiveSession && p_pActiveSession->chara_state[CHARA_STATE_DOUBLE_INDEX])
		{
			BYTE pkt[MAX_PACKET_SIZE];
			INT packetSize = 0;
			m_bDoubleShotFlg = TRUE;
			// パワーアップ状態をクリア
			if (p_pActiveSession->chara_state[CHARA_STATE_POWER_INDEX] == CHARA_STATE_POWERUP_USE)
			{
				p_pActiveSession->chara_state[CHARA_STATE_POWER_INDEX] = CHARA_STATE_POWERUP_OFF;
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(p_pActiveSession, CHARA_STATE_POWER_INDEX, pkt);
				AddPacketAllUser(NULL, pkt, packetSize);
			}
			// 撃つ前に二連発の状態をクリア
			p_pActiveSession->chara_state[CHARA_STATE_DOUBLE_INDEX] = 0x0;
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(p_pActiveSession, CHARA_STATE_DOUBLE_INDEX, pkt);
			AddPacketAllUser(NULL, pkt, packetSize);
//			g_pPPMain->TriggerStart(p_pActiveSession);
			g_pPPMain->ShootingBullet(p_pActiveSession, 0);		// 二発目を発射
		}
		else
			SetPhase(GAME_MAIN_PHASE_TURNEND);
		break;
	case GAME_MAIN_PHASE_CHECK:
		g_pCriticalSection->EnterCriticalSection_Session(L'$');
		g_pCriticalSection->EnterCriticalSection_Object(L'6');

		bNextPhase = !FrameCharacters();
		bNextPhase = (!FrameObjects() && bNextPhase);
		if (bNextPhase)
			SetPhase(m_eGameNextPhase);

		bGameEnd = IsGameEnd();
		g_pCriticalSection->LeaveCriticalSection_Object();
		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case GAME_MAIN_PHASE_SYNC:
		g_pCriticalSection->EnterCriticalSection_Session(L'%');
		bNextPhase = FrameSync();
		if (bNextPhase)
			SetPhase(m_eGameNextPhase);
		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case GAME_MAIN_PHASE_TURNEND:
		g_pCriticalSection->EnterCriticalSection_Object(L'7');
		g_pCriticalSection->EnterCriticalSection_Session(L'&');
		FrameTurnEnd();
		bGameEnd = IsGameEnd();
		g_pCriticalSection->LeaveCriticalSection_Session();
		g_pCriticalSection->LeaveCriticalSection_Object();
		break;
	case GAME_MAIN_PHASE_RETURN:
		g_pCriticalSection->EnterCriticalSection_Session(L'\'');
		bCharaPacket = FrameReturn();
		bGameEnd = IsGameEnd();
		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case GAME_MAIN_PHASE_NONE:
	default:
		break;
	}

	// メインゲーム終了
	if (bGameEnd || m_bKillGame)
	{
		GameEnd();
	}
	return bCharaPacket|bObjectPacket|m_bPacket|bGameEnd;
}

// 終了
void CSyncMain::GameEnd()
{
	g_pCriticalSection->EnterCriticalSection_Session(L'(');
	AddMessageLog(L"GameEnd");
	// 順位付け
	SetRankOrder();

	t_sessionInfo* pSessInfo = g_pNetSess->GetSessionTable();
	// パケット送信
	std::vector<ptype_session> vecEntityCharacters;
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session pSess = &pSessInfo[i].s;
		if (pSess->entity)
			vecEntityCharacters.push_back(pSess);
	}
	
	std::sort(vecEntityCharacters.begin(), vecEntityCharacters.end(), SessionNo_Asc);

	for (std::vector<ptype_session>::iterator it = vecEntityCharacters.begin();
		it != vecEntityCharacters.end();
		it++)
	{
		WCHAR wsRankLog[64];
		WCHAR name[MAX_USER_NAME+1];
		common::session::GetSessionName((*it), name);
		SafePrintf(wsRankLog, 64, L"Rank[%d]:%s", (*it)->frame_count+1, name);
		AddMessageLog(wsRankLog);
	}
	
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = PacketMaker::MakePacketData_MainInfoGameEnd(&vecEntityCharacters, pkt);
	AddPacketAllUser(NULL, pkt, packetSize);
	
	// 結果へ
	g_pPPPhase->SetGamePhase(GAME_PHASE_ROOM);
	g_pPPPhase = g_pPPRoom;
	g_pPPRoom->Reset(p_pNWSess, g_pPPMain);
	g_eGamePhase = GAME_PHASE_ROOM;
	UpdateWMCopyData();
	g_pCriticalSection->LeaveCriticalSection_Session();
}

void CSyncMain::Clear()
{
	m_vecObjectNo.clear();

	for (std::map<int, type_obj*>::iterator it = m_mapObjects.begin();
		it != m_mapObjects.end();
		it++)
	{
		if ((*it).second->obj_type & (OBJ_TYPE_BLT|OBJ_TYPE_ITEM))
		{
			type_blt* blt = (type_blt*)(*it).second;
			SafeDelete(blt);
		}
		else
		{
			type_obj* obj = (type_obj*)(*it).second;
			SafeDelete(obj);
		}
	}
	m_mapObjects.clear();
	if (g_pCriticalSection)
		g_pCriticalSection->EnterCriticalSection_Object(L'8');
	m_vecCharacters.clear();
	if (g_pCriticalSection)
		g_pCriticalSection->LeaveCriticalSection_Object();
	m_pMainStage->Clear();
	m_nObjectNoCounter = 0;
	p_pActiveSession = NULL;
	p_pStealSess = NULL;
	SetPhase(GAME_MAIN_PHASE_NONE);
}

std::map<int, type_obj*>::iterator
	CSyncMain::EraseMapObject(std::map<int, type_obj*>::iterator it)
{
	if ((*it).second->obj_type & (OBJ_TYPE_BLT|OBJ_TYPE_ITEM))
	{
		type_blt* blt = (type_blt*)(*it).second;
		SafeDelete(blt);
	}
	else
	{
		type_obj* obj = (type_obj*)(*it).second;
		SafeDelete(obj);
	}
	return m_mapObjects.erase(it);
}

void CSyncMain::UpdateObjectNo()
{
	m_vecObjectNo.clear();
	// 各オブジェクトの移動
	for (std::map<int, type_obj* >::iterator it = m_mapObjects.begin();
		it != m_mapObjects.end();
		it++)
		m_vecObjectNo.push_back( (int)(*it).first );
}

//> Luaでも使う
int CSyncMain::AddObject(type_obj* obj)
{
	int ret = -1;
	switch (obj->obj_type)
	{
	case OBJ_TYPE_CHARA:
		ret = AddCharacter((type_session*)obj);
		obj->obj_no = ret;
		break;
	default:
		ret = AddBullet((type_blt*)obj);
		break;
	}
	UpdateObjectNo();

	return ret;
}

int CSyncMain::AddBullet(ptype_blt blt)
{
	// 初期速度には加速度の半分を足しておく
	blt->vx += (short)(blt->adx*0.5);
	blt->vy += (short)(blt->ady*0.5);
	
	blt->bx = blt->ax*BLT_POS_FACT_N;
	blt->by = blt->ay*BLT_POS_FACT_N;
	blt->scrinfo = NULL;
	switch (blt->proc_type)
	{
	case BLT_PROC_TYPE_SCR_CHARA:
		// キャラスクリプトと関連付け
		blt->scrinfo = common::scr::FindCharaScrInfoFromCharaType(blt->chara_type, &g_mapCharaScrInfo);
		blt->hit_range = ((TCHARA_SCR_INFO*)blt->scrinfo)->blt_info[blt->bullet_type].hit_range;
		break;
	case BLT_PROC_TYPE_SCR_STAGE:
		// ステージスクリプトと関連付け
		blt->scrinfo = m_pStageScrInfo;
		blt->hit_range = m_pStageScrInfo->blt_info[blt->bullet_type].hit_range;
		break;
	case BLT_PROC_TYPE_SCR_SPELL:
		// キャラスクリプトと関連付け
		blt->scrinfo = common::scr::FindCharaScrInfoFromCharaType(blt->chara_type, &g_mapCharaScrInfo);
		blt->hit_range = ((TCHARA_SCR_INFO*)blt->scrinfo)->sc_info.hit_range;
		break;
	default:
		break;
	}
	
	ptype_blt newblt = new type_blt();
	memcpy(newblt, blt, sizeof(type_blt));
/// 20120920
//	if (m_mapObjects.empty())
//		m_nObjectNoCounter = 0;
	newblt->obj_no = (short)m_nObjectNoCounter;
	m_mapObjects.insert ( std::pair<int, type_obj*>(m_nObjectNoCounter , (type_obj*)newblt) );
	m_nObjectNoCounter = (++m_nObjectNoCounter)%32767;

	WCHAR log[64];
	SafePrintf(log, 64, L"AddBullet(no:%d/type:%d)", newblt->obj_no, newblt->bullet_type);
	AddMessageLog(log);

	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;
	packetSize = PacketMaker::MakePacketData_MainInfoBulletShot(newblt, pktdata);
	if (AddPacketAllUser(NULL, pktdata, packetSize))
		return newblt->obj_no;

	return -1;
}

bool CSyncMain::RemoveObject(int nIndex, E_OBJ_RM_TYPE rm_type)
{
	bool ret = false;
	AddMessageLog(L"RemoveObject");
	
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(nIndex);
	if (itfind == m_mapObjects.end())
	{
		AddMessageLog(L"map find error:RemoveObject()");
		return false;
	}
	type_obj* obj = (type_obj*)(*itfind).second;

	if (obj)
	{
		if (obj->proc_flg & PROC_FLG_OBJ_REMOVE)
			return false;
		obj->proc_flg |= PROC_FLG_OBJ_REMOVE;

		BYTE pktObjRemove[MAX_PACKET_SIZE];
		INT packetSizeObjRemove = PacketMaker::MakePacketData_MainInfoRemoveObject(rm_type, obj, pktObjRemove);
		if (packetSizeObjRemove)
			AddPacketAllUser(NULL, pktObjRemove, packetSizeObjRemove);

//		m_mapObjects.erase(itfind);
//		DEBUG_DELETE(itfind->second);
	}
//	UpdateObjectNo();
	return true;
}

bool CSyncMain::IsRemovedObject(int nIndex)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(nIndex);
	if (itfind == m_mapObjects.end())
	{
		AddMessageLog(L"map find error:IsRemovedObject()");
		return false;
	}
	type_obj* obj = (type_obj*)(*itfind).second;

	if (obj)
	{
		if (obj->proc_flg & PROC_FLG_OBJ_REMOVE)
			return true;
	}
	return false;
}

bool CSyncMain::UpdateObjectState(int obj_no, E_TYPE_OBJ_STATE state)
{
	BYTE pkt[MAX_PACKET_SIZE];

	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())
	{
		DXTRACE_MSG(L"map find error\nRemoveObject()");
		return false;
	}
	type_obj* obj = (type_obj*)(*itfind).second;
	// 削除フラグ確認
	if (obj->proc_flg & PROC_FLG_OBJ_REMOVE)
		return true;
	// 死んでいたら状態変更は行わない
	if (obj->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG)
		return true;

	if (obj)
	{
		// 状態設定
		obj->obj_state = state;
		obj->proc_flg |= PROC_FLG_OBJ_UPDATE_STATE;
		if (obj->obj_type & OBJ_TYPE_BLT)
		{
//> 20110404
		INT packetSize = PacketMaker::MakePacketData_MainInfoUpdateObjectState(obj_no, state, obj->frame_count, pkt);
		if (packetSize)
			AddPacketAllUser(NULL, pkt, packetSize);
//< 20110404
			ptype_blt blt = (type_blt*)((*itfind).second);
			// ステージが作成した弾か
			if (blt->obj_no == (short)STAGE_OBJ_NO)
			{
				LuaFuncParam luaParams, luaResults;
				// 存在しているなら情報取得
				// script,弾タイプ,弾ObjNo,弾位置x,y/移動x,y/extdata/obj_state
				luaParams.Number(m_pStageScrInfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number((DWORD)blt->obj_state&OBJ_STATE_MAIN_MASK);
				if (!common::scr::CallLuaFunc(g_pLuah, "onUpdateState_StageBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
					return false;
			}
			else
			{
				LuaFuncParam luaParams;
				LuaFuncParam luaResults;
				// script,弾タイプ,弾ObjNo,弾を作ったキャラのObjNo,弾位置x,y/移動x,y/extdata
				if (blt->bullet_type != DEF_BLT_TYPE_SPELL)
				{
					luaParams.Number(blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->chr_obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number((DWORD)blt->obj_state&OBJ_STATE_MAIN_MASK);
					if (!common::scr::CallLuaFunc(g_pLuah, "onUpdateState_CharaBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
						return false;
				}
				else
				{
					luaParams.Number(blt->scrinfo->scr_index).Number(blt->obj_no).Number(blt->chr_obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number((DWORD)blt->obj_state&OBJ_STATE_MAIN_MASK);
					if (!common::scr::CallLuaFunc(g_pLuah, "onUpdateState_CharaSpell", &luaResults, 0, &luaParams, g_pCriticalSection))
						return false;
				}
			}
//> 20110404
//		INT packetSize = PacketMaker::MakePacketData_MainInfoUpdateObjectState(obj_no, state, obj->frame_count, pkt);
//		if (packetSize)
//			AddPacketAllUser(NULL, pkt, packetSize);
//< 20110404
		}
	}
	return true;
}
bool CSyncMain::UpdateBulletPositoin(int obj_no, double px, double py, double vx, double vy, double adx, double ady)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())
	{
		DXTRACE_MSG(L"map find error\nRemoveObject()");
		return false;
	}
	type_obj* obj = (type_obj*)(*itfind).second;
	if (obj->proc_flg & PROC_FLG_OBJ_REMOVE)
		return true;
	if (obj)
	{
		// 状態設定
		type_blt* blt = (type_blt*)obj;
		blt->ax = (short)px;
		blt->ay = (short)py;
		blt->bx = (short)(blt->ax*BLT_POS_FACT_N);
		blt->by = (short)(blt->ay*BLT_POS_FACT_N);
		blt->vx = (short)vx;
		blt->vy = (short)vy;
		blt->adx = (char)adx;
		blt->ady = (char)ady;
		blt->proc_flg |= PROC_FLG_OBJ_UPDATE_POS;
		
		BYTE pkt[MAX_PACKET_SIZE];
		PacketMaker::MakePacketHeadData(PK_USER_MAININFO, PK_USER_MAININFO_BULLET_MV, 1, pkt);
		INT packetSize = PacketMaker::AddPacketData_MainInfoMoveBullet(5, blt, pkt);
		packetSize = PacketMaker::AddEndMarker(packetSize, pkt);
		if (packetSize)
			AddPacketAllUser(NULL, pkt, packetSize);
	}
	return false;
}
bool CSyncMain::UpdateBulletVector(int obj_no, double vx, double vy, double adx, double ady)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())
	{
		DXTRACE_MSG(L"map find error\nRemoveObject()");
		return false;
	}
	type_obj* obj = (type_obj*)(*itfind).second;
	if (obj->proc_flg & PROC_FLG_OBJ_REMOVE)
		return true;
	if (obj)
	{
		// 状態設定
		type_blt* blt = (type_blt*)obj;
		blt->vx = (short)vx;
		blt->vy = (short)vy;
		blt->adx = (char)adx;
		blt->ady = (char)ady;
		blt->proc_flg |= PROC_FLG_OBJ_UPDATE_VEC;

		BYTE pkt[MAX_PACKET_SIZE];
		PacketMaker::MakePacketHeadData(PK_USER_MAININFO, PK_USER_MAININFO_BULLET_VEC, 1, pkt);
		INT packetSize = PacketMaker::AddPacketData_MainInfoMoveBullet(5, blt, pkt);
		packetSize = PacketMaker::AddEndMarker(packetSize, pkt);
		if (packetSize)
			AddPacketAllUser(NULL, pkt, packetSize);
	}
	return true;
}

bool CSyncMain::BombObject(int scr_id, int blt_type, int blt_chr_no, int blt_no, int pos_x, int pos_y, int erase)
{
	D3DXVECTOR2 pos = D3DXVECTOR2((float)pos_x, (float)pos_y);
	TCHARA_SCR_INFO* pCharaScrInfo = NULL;
	BOOL* tblHit;
	tblHit = new BOOL[g_nMaxLoginNum];
	int* tblPower;
	tblPower = new int[g_nMaxLoginNum];
	ZeroMemory(tblHit, sizeof(BOOL)*g_nMaxLoginNum);
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;
	int range = 0;

	// 爆発範囲を得る、爆発イベントを通知する
	// ステージが作成した弾か
	if (blt_chr_no == DEF_STAGE_OBJ_NO)
		range = m_pStageScrInfo->blt_info[blt_type].bomb_range;
	else
	{
		pCharaScrInfo = common::scr::FindCharaScrInfoFromCharaType(scr_id, &g_mapCharaScrInfo);
		if (!pCharaScrInfo)
		{
			AddMessageLog(L"map find error\nBombObject()");
			SafeDeleteArray(tblHit);
			SafeDeleteArray(tblPower);
			return false;
		}
		if (blt_type != DEF_BLT_TYPE_SPELL)
			range = pCharaScrInfo->blt_info[blt_type].bomb_range;
		else
			range = pCharaScrInfo->sc_info.bomb_range;
	}

	// 弾の作り手
	ptype_session blt_sess = g_pNetSess->GetSessionFromUserNo(blt_chr_no);

	//> 爆発する前に範囲内のキャラを確認
	// (シールド持ちのキャラが居るか確認)
	// 爆発の範囲
	float fRR = (float)(range*range);

	g_pCriticalSection->EnterCriticalSection_Session(L')');

	// キャラと爆発範囲の当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		ptype_session sess = (*it);
		// 死したやつは飛ばす
//		if (sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY))
//			continue;
		D3DXVECTOR2 vecLen = D3DXVECTOR2((float)sess->ax-pos.x, (float)sess->ay-pos.y);
		float fLength = D3DXVec2LengthSq(&vecLen);
		if (fLength <= fRR)
		{
			// キャラに当たる
			// シールド状態確認
			if (sess->chara_state[CHARA_STATE_SHIELD_INDEX])
			{
				// シールドを解除
				sess->chara_state[CHARA_STATE_SHIELD_INDEX] = 0;
				// 弾を削除
				RemoveObject(blt_no, OBJ_RM_TYPE_SHIELD);
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_SHIELD_INDEX, pkt);
				AddPacketAllUser(NULL, pkt, packetSize);
				g_pCriticalSection->LeaveCriticalSection_Session();
				SafeDeleteArray(tblHit);
				SafeDeleteArray(tblPower);
				return true;
			}
			// 近いほど強くなる
			float fPower = (fRR>fLength)?(fRR-fLength)/fRR:0.0f;
			// パワーアップ状態確認
			if (blt_sess && blt_sess->chara_state[CHARA_STATE_POWER_INDEX] == CHARA_STATE_POWERUP_USE)
				fPower *= CHARA_STATE_POWERUP_FACTOR;

			tblPower[sess->sess_index] = ((int)(fPower*100.0f));	// 100%に変換
			tblHit[sess->sess_index] = TRUE;
		}
	}
	//< 爆発する前に範囲内のキャラを確認

	// 弾の情報を取得
	ptype_blt blt = NULL;
	std::map<int, type_obj*>::iterator itblt = m_mapObjects.find(blt_no);
	if (itblt != m_mapObjects.end())
	{
		if ( (*itblt).second->obj_type & OBJ_TYPE_BLT)
			blt = (ptype_blt)(*itblt).second;							// 弾
	}

	// 爆発半径ありで削除指定ありならステージ削除
	int nRetErasePixels = 0;
	if (range && erase)
	{
		//> ステージ削除
		g_pCriticalSection->EnterCriticalSection_StageTexture(L'1');
		nRetErasePixels = m_pMainStage->EraseStage(&pos, range);	// ステージ削除するピクセル数が返ってくる
		g_pCriticalSection->LeaveCriticalSection_StageTexture();
		if (nRetErasePixels)
		{
			// ステージスクリプトへステージ削除イベント
			LuaFuncParam luaParams,luaResults;
			
			// キャラが発射した弾の場合、ステージ削除イベントを起こす
//> 20110420 ステージが作成した弾の場合、onErase_Stageが呼び出されなかったのを修正
//			if (blt_chr_no != STAGE_OBJ_NO)
//< 20110420 ステージが作成した弾の場合、onErase_Stageが呼び出されなかったのを修正
			{
				// ステージスクリプト番号,弾のタイプ,当たった弾の位置x,y/爆発位置x,y/範囲の近さによる威力値/extdata
				luaParams.Number(m_pStageScrInfo->scr_index).Number(blt_type).Number(blt_chr_no).Number(blt_no).Number(pos_x).Number(pos_y).Number(nRetErasePixels);
				if (!common::scr::CallLuaFunc(g_pLuah, "onErase_Stage", &luaResults, 0, &luaParams, g_pCriticalSection))
				{
					g_pCriticalSection->LeaveCriticalSection_Session();
					SafeDeleteArray(tblHit);
					SafeDeleteArray(tblPower);
					return false;
				}
			}
		}
		//< ステージ削除
	}

	// 爆発イベントを通知する
	if (blt_chr_no == DEF_STAGE_OBJ_NO)
	{
		// Luaに弾爆発イベント通知
		LuaFuncParam luaParams,luaResults;
		// script,弾タイプ,弾ObjNo,弾を作ったキャラのObjNo,弾位置x,y/移動x,y/extdata
		if (blt)
			luaParams.Number(blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number(pos_x).Number(pos_y).Number(nRetErasePixels);
		else
			luaParams.Number(m_pStageScrInfo->scr_index).Number(blt_type).Number(-1).Number(pos_x).Number(pos_y).Nil().Nil().Nil().Nil().Number(pos_x).Number(pos_y).Number(nRetErasePixels);
		
		if (!common::scr::CallLuaFunc(g_pLuah, "onBomb_StageBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
		{
			g_pCriticalSection->LeaveCriticalSection_Session();
			SafeDeleteArray(tblHit);
			SafeDeleteArray(tblPower);
			return false;
		}
	}
	else
	{
		// Luaに弾爆発イベント通知
		LuaFuncParam luaParams,luaResults;
		if (blt_type != DEF_BLT_TYPE_SPELL)
		{
			// script,弾タイプ,弾ObjNo,弾を作ったキャラのObjNo,弾位置x,y/移動x,y/extdata
			if (blt)
				luaParams.Number(blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->chr_obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number(pos_x).Number(pos_y).Number(nRetErasePixels);
			else
				luaParams.Number(pCharaScrInfo->scr_index).Number(blt_type).Number(-1).Number(blt_chr_no).Number(pos_x).Number(pos_y).Nil().Nil().Nil().Nil().Number(pos_x).Number(pos_y).Number(nRetErasePixels);
			if (!common::scr::CallLuaFunc(g_pLuah, "onBomb_CharaBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				SafeDeleteArray(tblHit);
				SafeDeleteArray(tblPower);
				return false;
			}
		}
		else
		{
			// script,弾ObjNo,弾を作ったキャラのObjNo,弾位置x,y/移動x,y/extdata
			if (blt)
				luaParams.Number(blt->scrinfo->scr_index).Number(blt->obj_no).Number(blt->chr_obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number(pos_x).Number(pos_y).Number(nRetErasePixels);
			else
				luaParams.Number(pCharaScrInfo->scr_index).Number(-1).Number(blt_chr_no).Number(pos_x).Number(pos_y).Nil().Nil().Nil().Nil().Number(pos_x).Number(pos_y).Number(nRetErasePixels);
			if (!common::scr::CallLuaFunc(g_pLuah, "onBomb_CharaSpell", &luaResults, 0, &luaParams, g_pCriticalSection))
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				SafeDeleteArray(tblHit);
				SafeDeleteArray(tblPower);
				return false;
			}
		}
	}
	// パケット作成
	packetSize = PacketMaker::MakePacketHeader_MainInfoBombObject(scr_id, blt_type,blt_chr_no, blt_no, pos_x, pos_y, erase, pkt);

	// 爆発範囲内のキャラにイベント通知
	t_sessionInfo* tblSess = g_pNetSess->GetSessionTable();
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		if (!tblHit[i])	continue;
		ptype_session sess = &tblSess[i].s;
		float fPower = tblPower[i]*0.01f;	// 小数点に変換
		// キャラのスクリプト取得
		if (blt_sess)
		{
			// Luaに爆発範囲内のキャラを知らせる
			LuaFuncParam luaParams,luaResults;
			if (blt_type != DEF_BLT_TYPE_SPELL)
			{
				// script,弾タイプ,当たったキャラのObjNo,弾を作ったキャラのObjNo,当たった弾のObjNo,当たった弾の位置x,y/爆発位置x,y/範囲の近さによる威力値
				luaParams.Number(blt->scrinfo->scr_index).Number(blt_type).Number(sess->obj_no).Number(blt_sess->obj_no).Number(blt_no).Number(pos_x).Number(pos_y).Number(fPower);
				if (!common::scr::CallLuaFunc(g_pLuah, "onHitChara_CharaBulletBomb", &luaResults, 0, &luaParams, g_pCriticalSection))
				{
					g_pCriticalSection->LeaveCriticalSection_Session();
					SafeDeleteArray(tblHit);
					SafeDeleteArray(tblPower);
					return false;
				}
			}
			else
			{
				// script,当たったキャラのObjNo,弾を作ったキャラのObjNo,当たった弾のObjNo,当たった弾の位置x,y/爆発位置x,y/範囲の近さによる威力値
				luaParams.Number(blt->scrinfo->scr_index).Number(sess->obj_no).Number(blt_sess->obj_no).Number(blt_no).Number(pos_x).Number(pos_y).Number(fPower);
				if (!common::scr::CallLuaFunc(g_pLuah, "onHitChara_CharaSpellBomb", &luaResults, 0, &luaParams, g_pCriticalSection))
				{
					g_pCriticalSection->LeaveCriticalSection_Session();
					SafeDeleteArray(tblHit);
					SafeDeleteArray(tblPower);
					return false;
				}
			}
		}
		else
		{
			// Luaに爆発範囲内のキャラを知らせる
			LuaFuncParam luaParams,luaResults;
			// script,弾タイプ,当たったキャラのObjNo,当たった弾の位置x,y/爆発位置x,y/範囲の近さによる威力値
			luaParams.Number(m_pStageScrInfo->scr_index).Number(blt_type).Number(sess->obj_no).Number(blt_no).Number(pos_x).Number(pos_y).Number(fPower);
			if (!common::scr::CallLuaFunc(g_pLuah,"onHitChara_StageBulletBomb", &luaResults, 0, &luaParams, g_pCriticalSection))
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				SafeDeleteArray(tblHit);
				SafeDeleteArray(tblPower);
				return false;
			}
		}
		// パケット追加
		packetSize = PacketMaker::AddPacketData_MainInfoBombObject(packetSize, sess->obj_no, tblPower[i], pkt);
	}
	g_pCriticalSection->LeaveCriticalSection_Session();

	if (packetSize)
	{
		packetSize = PacketMaker::AddEndMarker(packetSize, pkt);
		AddPacketAllUser(NULL, pkt, packetSize);
	}
	SafeDeleteArray(tblHit);
	SafeDeleteArray(tblPower);

	return true;
}

BOOL CSyncMain::PasteTextureOnStage(int scr_id, int sx,int sy, int tx,int ty,int tw,int th)
{
	g_pCriticalSection->EnterCriticalSection_StageTexture(L'2');

	WCHAR path[_MAX_PATH+1];
	RECT rcPasteImage = {tx, ty, tw, th};

	if (g_bLogFile)
	{
		WCHAR log[64];
		SafePrintf(log, 64, L"Paste(src{%d,%d,%d,%d},dst{%d,%d}", tx,ty,tw,th, sx,sy);
		AddMessageLog(log);
	}

	switch (scr_id)
	{
	case DEF_STAGE_ID:
		SafePrintf(path, _MAX_PATH, m_pStageScrInfo->stage.path);
		break;
	case DEF_SYSTEM_ID:
		SafePrintf(path, _MAX_PATH, IMG_GUI_SKIN);
		break;
	default:
		{
			// キャラのスクリプト取得
			TCHARA_SCR_INFO* pCharaScrInfo = common::scr::FindCharaScrInfoFromCharaType(scr_id, &g_mapCharaScrInfo);
			SafePrintf(path, _MAX_PATH, pCharaScrInfo->tex_path);
		}
		break;
	}
	// 画像貼り付け
	BOOL ret = m_pMainStage->PasteImage(g_pFiler, path,&rcPasteImage, sx, sy);
	g_pCriticalSection->LeaveCriticalSection_StageTexture();

	// 画像貼り付けパケット送信
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;
	packetSize = PacketMaker::MakePacketData_MainInfoPasteImage(scr_id, sx,sy, tx,ty,tw,th, pkt);
	return AddPacketAllUser(NULL, pkt, packetSize);
	return ret;
}

bool CSyncMain::SetBulletOptionData(int obj_no, int index, DWORD data)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())
	{
		DXTRACE_MSG(L"map find error\nRemoveObject()");
		return false;
	}
	type_blt* blt = (type_blt*)(*itfind).second;
	/*
	// 削除フラグ確認
	if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)
		return true;
	*/
	if (index >= OBJ_OPTION_COUNT)
	{
		MessageBox(NULL, L"SetBulletOptionDataのindex値を超えています", L"lua error", MB_OK); 
		return false;
	}

	blt->option[index] = data;
	blt->proc_flg |= PROC_FLG_OBJ_SET_EXDATA;
	return true;
}

unsigned int CSyncMain::GetBulletOptionData(int obj_no, int index)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())
	{
		DXTRACE_MSG(L"map find error\nRemoveObject()");
		return false;
	}
	type_blt* blt = (type_blt*)(*itfind).second;
	
	if (index >= OBJ_OPTION_COUNT)
	{
		MessageBox(NULL, L"GetBulletOptionDataのindex値を超えています", L"lua error", MB_OK); 
		return false;
	}

	return blt->option[index];
}

bool CSyncMain::SetBulletExtData1(int obj_no, DWORD extdata1)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())
	{
		DXTRACE_MSG(L"map find error\nRemoveObject()");
		return false;
	}
	type_blt* blt = (type_blt*)(*itfind).second;
	// 削除フラグ確認
	if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)
		return true;
	blt->extdata1 = extdata1;
	blt->proc_flg |= PROC_FLG_OBJ_SET_EXDATA;
	return true;	
}

bool CSyncMain::SetBulletExtData2(int obj_no, DWORD extdata2)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())
	{
		DXTRACE_MSG(L"map find error\nRemoveObject()");
		return false;
	}
	type_blt* blt = (type_blt*)(*itfind).second;
	// 削除フラグ確認
	if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)
		return true;
	blt->extdata2 = extdata2;
	blt->proc_flg |= PROC_FLG_OBJ_SET_EXDATA;
	return true;
}

unsigned int CSyncMain::GetCharaOptionData(int obj_no, int index)
{
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return 0;

	if (index >= OBJ_OPTION_COUNT)
	{
		MessageBox(NULL, L"GetCharaOptionDataのindex値を超えています", L"lua error", MB_OK); 
		return false;
	}
	return sess->option[index];
}

unsigned int CSyncMain::GetCharaExtData1(int obj_no)
{
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return 0;
	return sess->extdata1;
}

unsigned int CSyncMain::GetCharaExtData2(int obj_no)
{
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return 0;
	return sess->extdata2;
}

bool CSyncMain::SetCharaOptionData(int obj_no, int index, DWORD data)
{
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return false;

	if (index >= OBJ_OPTION_COUNT)
	{
		MessageBox(NULL, L"GetCharaOptionDataのindex値を超えています", L"lua error", MB_OK); 
		return false;
	}
	sess->option[index] = data;
	return true;
}

bool CSyncMain::SetCharaExtData1(int obj_no, DWORD extdata1)
{
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return false;

	sess->extdata1 = extdata1;
	return true;
}

bool CSyncMain::SetCharaExtData2(int obj_no, DWORD extdata2)
{
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return false;
	sess->extdata2 = extdata2;
	return true;
}

// ゲーム中にキャラの所持アイテム追加
bool CSyncMain::AddCharaItem(int obj_no, DWORD item_flg)
{
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return false;

	// アイテムフラグチェック
	if (!(item_flg&GAME_ITEM_ENABLE_FLG))
		return false;

	// 空きスロットを探す
	int nSlot = 0;
	for (nSlot=0;nSlot<GAME_ITEM_STOCK_MAX_COUNT;nSlot++)
	{
		if (!sess->items[nSlot])	// 空きスロット
			break;
	}

	// 空きが無い場合
	if (nSlot >= GAME_ITEM_STOCK_MAX_COUNT)
		return UpdateCharaStatus(obj_no,0,0,0,GAME_ADD_ITEM_ENOUGH_EXP);

	// アイテム追加
	sess->items[nSlot] = item_flg;
	// パケット送信
	packetSize = PacketMaker::MakePacketData_MainInfoAddItem(obj_no,nSlot, item_flg, pkt, FALSE);
	AddPacket(sess, pkt, packetSize);

	return true;
}

bool CSyncMain::SetCharacterState(int obj_no, int chr_stt, int val)
{
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return false;
	if (chr_stt < 0 || chr_stt >= CHARA_STATE_COUNT) return false;
	if (val < 0) return false;

	sess->chara_state[chr_stt] = val;
	packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, chr_stt, pkt);
	AddPacketAllUser(NULL, pkt, packetSize);
	return true;
}

int CSyncMain::GetCharacterState(int obj_no, int chr_stt)
{
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return -1;
	if (chr_stt < 0 || chr_stt >= CHARA_STATE_COUNT) return -1;

	return (int)sess->chara_state[chr_stt];
}

unsigned long CSyncMain::GetCharacterItemInfo(int obj_no, int item_index)
{
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return 0;
	if (item_index < 0 || item_index >= GAME_ITEM_STOCK_MAX_COUNT) return 0;

	return (unsigned long)sess->items[item_index];
}

///< Luaでも使う

//> Frame
// アクティブターンなキャラのフレーム処理
BOOL CSyncMain::FrameActiveCharacter()
{
	// アクティブキャラが居ない場合終了
	if (!p_pActiveSession) return TRUE;
	if (p_pActiveSession->connect_state != CONN_STATE_AUTHED) return TRUE;
	if (!(p_pActiveSession->obj_state & OBJ_STATE_GAME)) return TRUE;
	if (p_pActiveSession->obj_state & (OBJ_STATE_MAIN_NOACT_FLG|OBJ_STATE_MAIN_NOLIVE_FLG))	return TRUE;
	// アクティブキャラの移動
	MoveCharacter(p_pActiveSession);
	// アクティブ継続
	return FALSE;
}

// キャラのフレーム処理
BOOL CSyncMain::FrameCharacters()
{
	BOOL bMoved = FALSE;
	// 各キャラの移動
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		ptype_session sess = (*it);
		// ゲーム内キャラ以外とばす
		if (!sess->entity) continue;
//		// アクティブキャラをとばす
//		if (sess == p_pActiveSession) continue;
		// 処理する状態確認
		if (sess->obj_state & OBJ_STATE_MAIN_DROP_FLG) continue;
		// 移動（落下確認）
		bMoved |= MoveCharacter(sess);
	}
	return bMoved;
}

BOOL CSyncMain::GetRandomStagePos(POINT* pnt)
{
	std::vector<int> vecXPos;
	D3DXVECTOR2	vecGround = D3DXVECTOR2(0,0);
	m_pMainStage->GetGroundsXPos(&vecXPos);

	int nRndWidth = vecXPos.size();
	if (!nRndWidth)
		return FALSE;

	D3DXVECTOR2 vecPos;
	D3DXVECTOR2 vecPut;
	vecPos.x = (float)( vecXPos[genrand_int32()%nRndWidth]);
	vecPos.y = (float)(genrand_int32()%GetStageHeight());
	// 地面を探す
	while (!m_pMainStage->FindGround(&vecPut, &vecGround, &vecPos, CHARA_BODY_RANGE))
	{
		vecPos.x = (float)(vecXPos[genrand_int32()%nRndWidth]);
		vecPos.y = (float)(genrand_int32()%GetStageHeight());
	}
	pnt->x = (LONG)vecPut.x;
	pnt->y = (LONG)vecPut.y;

	return TRUE;
}

BOOL CSyncMain::MoveCharacter(ptype_session sess)
{
	BOOL bMoved = TRUE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;
//	switch (common::chr::MoveStage(m_pMainStage, sess, &g_mapCharaScrInfo))
	switch (common::chr::MoveOnStage(m_pMainStage, sess, &g_mapCharaScrInfo))
	{
	case common::chr::MOVE_STAGE_RESULT_NONE:
		bMoved = FALSE;
		break;
	case common::chr::MOVE_STAGE_RESULT_DROP:
// 20101218 未行動キャラの落下死を予防するルール
		if ( (m_bytRule & GAME_RULE_START_CARE)	&& !sess->turn_count)
		{
			std::vector<int> vecXPos;
			m_pMainStage->GetGroundsXPos(&vecXPos);
			PutCharacter(&vecXPos, sess, TRUE);
			bMoved = TRUE;
			packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess,pkt);
			if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
				AddPacketTeamUser(sess->team_no, pkt, packetSize);
			else
				AddPacketNoBlindUser(sess, pkt, packetSize);
		}
		else
			NotifyCharaDead(sess, CHARA_DEAD_DROP);
		break;
	case common::chr::MOVE_STAGE_RESULT_VEC_CHANGE:
		bMoved = TRUE;
		packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess,pkt);
		if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
			AddPacketTeamUser(sess->team_no, pkt, packetSize);
		else
			AddPacketNoBlindUser(sess, pkt, packetSize);
		break;
	}
	return bMoved;
}

void CSyncMain::FrameTrigger()
{
	if (!p_pActiveSession)
	{
		SetPhase(GAME_MAIN_PHASE_TURNEND);
		return;
	}
	if (m_bReqShotFlg) return;

	if (g_bOneClient)
	{
		// 自身の確認
		if (!p_pActiveSession->game_ready)	return;
	}

	BOOL bRecvAck = FALSE;
	BOOL bReqShot = TRUE;
	// 各キャラのトリガ終了確認
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		ptype_session sess = (*it);
		if (sess->connect_state != CONN_STATE_AUTHED)	continue;
		if (sess == p_pActiveSession)	continue;
		if (!sess->entity)	continue;
		
		if (sess->game_ready)
			bRecvAck = TRUE;

		// 最大待ち時間を越えていた。誰か準備OKな場合はタイムアウト
		if (m_nPhaseTimeCounter >= GAME_MAIN_PHASE_TIME_TRIGGER)//&& bRecvAck)
		{
			if (!sess->game_ready)
			{
				WCHAR logTrigger[32];
				SafePrintf(logTrigger, 32, L"TriggerWaitTimeOut(#%d)", sess->obj_no);
				AddMessageLog(logTrigger);
				sess->game_ready = 1;
			}
			continue;
		}

		// 発射するキャラがステルス状態
		if (p_pActiveSession->chara_state[CHARA_STATE_STEALTH_INDEX])
		{
			if (sess == p_pActiveSession && !sess->game_ready)
			{
				bReqShot = FALSE;
				continue;
			}
			// 同チームのみ見えるので、別チームは確認しない
			if (
			(p_pActiveSession->team_no == GALLERY_TEAM_NO || p_pActiveSession->team_no == sess->team_no)
			&& !sess->chara_state[CHARA_STATE_BLIND_INDEX] && !sess->game_ready)
			{
				bReqShot = FALSE;
				continue;
			}
		}
		else if (!sess->chara_state[CHARA_STATE_BLIND_INDEX] && !sess->game_ready)
		{
			// 暗転キャラ以外の確認
			bReqShot = FALSE;
			continue;
		}
	}

	// 全キャラ準備OK、弾発射要求
	if (bReqShot)
	{
		for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
			it != m_vecCharacters.end();
			it++)
		{
			ptype_session sess = (*it);
			if (sess->connect_state != CONN_STATE_AUTHED)	continue;
			if (!sess->entity)	continue;
			sess->game_ready = 1;
		}
		AddMessageLog(L"ReqShot");
		BYTE pkt[MAX_PACKET_SIZE];
		INT packetSize = 0;
		packetSize = PacketMaker::MakePacketData_MainInfoReqShot(p_pActiveSession->obj_no, pkt);
		AddPacket(p_pActiveSession, pkt, packetSize);
		m_bReqShotFlg = TRUE;
	}
	else if (!(m_nPhaseTimeCounter % (int)(FRAMES*2)))
	{
		// トリガー終了通知を要求してみる
		WCHAR log[32];
		BYTE pkt[MAX_PACKET_SIZE];
		INT packetSize = 0;
		packetSize = PacketMaker::MakePacketData_MainInfoReqTriggerEnd(p_pActiveSession->obj_no, pkt);

		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			// 認証済みか
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (!pSess->entity)	continue;
			if (!pSess->game_ready)
			{
				SafePrintf(log, 32, L"ReqTriggerEnd:%d", pSess->obj_no);
				AddMessageLog(log);
				AddPacket(pSess, pkt, packetSize);
			}
		}
	}
}

BOOL CSyncMain::FrameObjects()
{
	BOOL bActiveBullet = TRUE;
	INT packetSize=5;
	BYTE pkt[MAX_PACKET_SIZE];
	BOOL bRemoved = FALSE;
	int nVecChangeBulletCount =0;
	std::vector<int> vecList;

	// オブジェクトなし
	if (m_vecObjectNo.empty())	return FALSE;
	vecList.assign(m_vecObjectNo.begin(), m_vecObjectNo.end());

	for (std::vector<int>::iterator itno = vecList.begin();
		itno != vecList.end();
		itno++
		)
	{
		std::map<int, type_obj*>::iterator it = m_mapObjects.find((*itno));
		if (it == m_mapObjects.end())	continue;

		ptype_obj obj = (ptype_obj)(*it).second;
		// 削除フラグ確認
		if (obj->proc_flg & PROC_FLG_OBJ_REMOVE)	continue;

		ptype_blt blt = (ptype_blt)obj;
		// 戦術
		if ((BYTE)blt->obj_type & OBJ_TYPE_TACTIC) continue;

		bRemoved = FALSE;
		switch (obj->obj_state)
		{
		case OBJ_STATE_MAIN_ACTIVE:
			bActiveBullet = FALSE;
			switch (common::blt::MoveActBullet(m_pMainStage,blt, &m_vecCharacters, g_pLuah, m_nWind, &g_mapCharaScrInfo, &m_mapObjects, g_pCriticalSection, TRUE))
			{
			case common::blt::MOVE_ACT_BULLET_RESULT_DROP:
				{
					BYTE pktObjRemove[MAX_PACKET_SIZE];
					INT packetSizeObjRemove = PacketMaker::MakePacketData_MainInfoRemoveObject(OBJ_RM_TYPE_OUT, obj, pktObjRemove);
					if (packetSizeObjRemove)
						AddPacketAllUser(NULL, pktObjRemove, packetSizeObjRemove);
//> 20101218
					(*it).second->proc_flg = PROC_FLG_OBJ_REMOVE;
//					it = EraseMapObject(it);		// TURNENDで削除するように変更
//< 20101218
					bRemoved = TRUE;
					continue;
				}
				break;
			case common::blt::MOVE_ACT_BULLET_RESULT_REMOVE:		// スクリプトによって削除された
				bRemoved = TRUE;
				break;
			case common::blt::MOVE_ACT_BULLET_RESULT_VEC_CHANGE:	// 移動値変更
				packetSize = PacketMaker::AddPacketData_MainInfoMoveBullet(packetSize, blt, pkt);
				nVecChangeBulletCount++;
				break;
			case common::blt::MOVE_ACT_BULLET_RESULT_CHARA_HIT:
			case common::blt::MOVE_ACT_BULLET_RESULT_STAGE_HIT:
				OnHitStageItemBullet(blt);
				break;
			case common::blt::MOVE_ACT_BULLET_RESULT_MOVED:	// 通常移動
			case common::blt::MOVE_ACT_BULLET_RESULT_NONE:
			default:
				break;
			}
			break;
		case OBJ_STATE_MAIN_WAIT:
			switch (common::blt::MoveWaitBullet(m_pMainStage,blt, &m_vecCharacters, g_pLuah, m_nWind, &g_mapCharaScrInfo, &m_mapObjects, g_pCriticalSection, TRUE))
			{
			case common::blt::MOVE_WAIT_BULLET_RESULT_DROPOUT:
				{
					BYTE pktObjRemove[MAX_PACKET_SIZE];
					INT packetSizeObjRemove = PacketMaker::MakePacketData_MainInfoRemoveObject(OBJ_RM_TYPE_OUT, obj, pktObjRemove);
					if (packetSizeObjRemove)
						AddPacketAllUser(NULL, pktObjRemove, packetSizeObjRemove);
//> 20101218
					(*it).second->proc_flg = PROC_FLG_OBJ_REMOVE;
//					it = EraseMapObject(it);		// TURNENDで削除するように変更
//< 20101218
					bRemoved = TRUE;
					continue;
				}
				break;
			case common::blt::MOVE_WAIT_BULLET_RESULT_REMOVED:		// スクリプトによって削除された
				bRemoved = TRUE;
				break;
			case common::blt::MOVE_WAIT_BULLET_RESULT_VEC_CHANGE:	// 移動値変更
				packetSize = PacketMaker::AddPacketData_MainInfoMoveBullet(packetSize, blt, pkt);
				nVecChangeBulletCount++;
				break;
			case common::blt::MOVE_WAIT_BULLET_RESULT_FALLDOWN:
			case common::blt::MOVE_WAIT_BULLET_RESULT_FALLING:
			case common::blt::MOVE_WAIT_BULLET_RESULT_GETDOWN:
			case common::blt::MOVE_WAIT_BULLET_RESULT_GROUND:
			case common::blt::MOVE_WAIT_BULLET_RESULT_NONE:
			default:
				break;
			}
			break;
		case OBJ_STATE_MAIN_DROP:
		default:
			continue;
//			break;
		}

		// 削除されていないか
		if ( !bRemoved )
		{
			if (blt->proc_flg & PROC_FLG_OBJ_UPDATE_TYPE)
			{
				common::obj::SetLuaFlg((type_obj*)blt, PROC_FLG_OBJ_UPDATE_TYPE, FALSE);
				BYTE pktBltUpdateType[MAX_PACKET_SIZE];
				INT packetSizeBltUpdateType = PacketMaker::MakePacketData_MainInfoUpdateObjectType(blt, pktBltUpdateType);
				if (packetSizeBltUpdateType)
					AddPacketAllUser(NULL, pktBltUpdateType, packetSizeBltUpdateType);
			}
		}
	}

	// 移動値の変更があった弾があるか
	if (nVecChangeBulletCount > 0)
	{
		PacketMaker::MakePacketHeadData(PK_USER_MAININFO, PK_USER_MAININFO_BULLET_MV, nVecChangeBulletCount, pkt);
		packetSize = PacketMaker::AddEndMarker(packetSize, pkt);
		AddPacketAllUser(NULL, pkt, packetSize);
	}
	return !bActiveBullet;
}

BOOL CSyncMain::FrameTurnEnd()
{
	BOOL bPacket = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;
	ptype_session next_chara = NULL;
	int nMinDelay = MAXINT;
#ifndef NO_WIND
	// 風方向ランダム
	if (m_bytRule & GAME_RULE_WIND_ENABLE)
	{
		if (((int)genrand_int32())%RND_WIND_MOD)
		{
			if (m_nWindDirection == MAX_WIND_DIR_VALUE)
				m_nWindDirection -= (int)(-((int)genrand_int32()%(MAX_WIND_DIR_RND_VALUE)))+1;
			else if (m_nWindDirection == MIN_WIND_DIR_VALUE)
				m_nWindDirection += (int)((int)genrand_int32()%(MAX_WIND_DIR_RND_VALUE))+1;
			
			if (m_nWind == MAX_WIND_VALUE)
			{
				m_nWindDirection = min(m_nWindDirection, 0);
				m_nWindDirValancer = -2;
			}
			else if (m_nWind == MIN_WIND_VALUE)
			{
				m_nWindDirection = max(m_nWindDirection, 0);
				m_nWindDirValancer = 2;
			}
			else if (m_nWind <= MAX_WIND_VALUE/2 && m_nWind > 2)
			{
				m_nWindDirValancer = -1;
			}
			else if (m_nWind >= MIN_WIND_VALUE/2 && m_nWind < -2)
			{
				m_nWindDirValancer = 1;
			}

			m_nWindDirection =	max((int)MIN_WIND_DIR_VALUE,
											min((int)MAX_WIND_DIR_VALUE,
											m_nWindDirection+m_nWindDirValancer+
											(int)( (int)(genrand_int32()%((int)MAX_WIND_DIR_RND_VALUE-(int)MIN_WIND_DIR_RND_VALUE+1) ) + MIN_WIND_DIR_RND_VALUE)
											));
			m_nWind = max(MIN_WIND_VALUE,min(MAX_WIND_VALUE,(m_nWind+m_nWindDirection)));
	//		WCHAR log[64];
	//		SafePrintf(log, 64, L"WindDirection:%+d / Wind:%+d / Valancer:%+d", m_nWindDirection, m_nWind,m_nWindDirValancer);
	//		AddMessageLog(log);
		}
	}
#endif
	// アクティブキャラのディレイ値計算
	if (p_pActiveSession)
	{
		p_pActiveSession->turn_count++;	// ターン数増加
//> 20100213 経過時間の計算
		float fMaxPassageTime = (float)(g_pPPMain->GetActTimeLimit()*RUN_FRAMES);
		int nTimeOfDelay = p_pActiveSession->frame_count?(int) (((float)(p_pActiveSession->frame_count+1) / fMaxPassageTime) * MAX_PASSAGE_TIME_DELAY_VALUE):0;
//		int nTimeOfDelay = p_pActiveSession->frame_count?(int) (((float)p_pActiveSession->frame_count / (float)MAX_PASSAGE_TIME) * MAX_PASSAGE_TIME_DELAY_VALUE):0;
//> 20100213 経過時間の計算
//		int nCharaDelay = ((TCHARA_SCR_INFO*)p_pActiveSession->scrinfo)->delay;
		int nMoveOfDelay = (int) ( ((float)(p_pActiveSession->MV_m-min(p_pActiveSession->MV_c,p_pActiveSession->MV_m)) / (float)p_pActiveSession->MV_m) * MOVE_DELAY_FACT);
		WCHAR delaylog[64];
		SafePrintf(delaylog, 64, L"delay:%d+time(%d)/move(%d)", p_pActiveSession->delay, nTimeOfDelay,/*nCharaDelay,*/nMoveOfDelay);
		AddMessageLog(delaylog);

		p_pActiveSession->delay += (nTimeOfDelay/* + nCharaDelay*/+nMoveOfDelay);

		// 状態のカウント
		bPacket |= CountCharacterState(p_pActiveSession);
		// 状態の確認
		bPacket |= UpdateCharacterState(p_pActiveSession);
	}

	// スティール中のキャラが居た場合
	if (p_pStealSess)
	{
		switch (p_pStealSess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX])
		{
		case GAME_ITEM_STEAL_SET:		// 使用されたら
			p_pStealSess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] = GAME_ITEM_STEAL_USING;	// 使用中に設定
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(p_pStealSess, CHARA_STATE_ITEM_STEAL_INDEX, pkt);
			bPacket = AddPacketAllUser(NULL, pkt, packetSize);
			break;
		case GAME_ITEM_STEAL_USING:
			p_pStealSess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] = GAME_ITEM_STEAL_OFF;	// 未使用に設定
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(p_pStealSess, CHARA_STATE_ITEM_STEAL_INDEX, pkt);
			p_pStealSess = NULL;
			bPacket |= AddPacketAllUser(NULL, pkt, packetSize);
			break;
		default:
			p_pStealSess = NULL;
			break;
		}
	}

	// ディレイの低いキャラ分全員のディレイを減らす
//> 20110503
//	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
//		it != m_vecCharacters.end();
//		it++)
//	{
//		ptype_session sess = (*it);
	NotifyTurnEndCharacter(p_pActiveSession);
	// ターンエンド、スタートイベント
	NotifyTurnEndStage(p_pActiveSession, next_chara);
	NotifyTurnEndBullet(p_pActiveSession);

	// ディレイの低いキャラを探す
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		ptype_session sess = (*it);
		if (sess->connect_state != CONN_STATE_AUTHED)	continue;
		if (!sess->entity)	continue;
		if (sess->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG) continue;

		(*it)->live_count++;				// 生存ターン数増加
		(*it)->frame_count = 0;		// フレーム初期化
		(*it)->MV_c = (*it)->MV_m;	// 移動値の初期化
		(*it)->obj_state = OBJ_STATE_MAIN_WAIT;	// 状態を初期化

		nMinDelay = min(nMinDelay, (*it)->delay);	// 最小ディレイ値を探す
	}

	t_sessionInfo* ptbl = p_pNWSess->GetSessionTable();
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session sess = &ptbl[i].s;
//< 20110503
		if (sess->connect_state != CONN_STATE_AUTHED)	continue;
//		if (!sess->entity)	continue;	20110428

#if SC_NOLIMIT
		sess->EXP_c = 5000;
#else
		if (g_bOneClient)
			sess->EXP_c = 5000;
#endif
		// ディレイ値、アクティブ情報を各セッションに送信
		packetSize = PacketMaker::MakePacketData_MainInfoTurnEnd(sess, m_nWind, pkt);
		bPacket |= AddPacket(sess, pkt, packetSize);

		// 死んだキャラはディレイ値計算しない
		if (sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY|OBJ_STATE_MAIN_GALLERY_FLG)) continue;

		if (sess->entity && !(sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_MAIN_GALLERY_FLG)))
		{
			WCHAR delaylog[64];
			WCHAR name[MAX_USER_NAME+1];
			common::session::GetSessionName(sess, name);
			SafePrintf(delaylog, 64, L"delay[%d]:%s", sess->delay, name);
			AddMessageLog(delaylog);
			sess->delay -= nMinDelay;

			if ((sess->delay == 0))
			{
				if (!next_chara)
					next_chara = sess;
				else if ( ((TCHARA_SCR_INFO*)sess->scrinfo)->delay < ((TCHARA_SCR_INFO*)next_chara->scrinfo)->delay)
					next_chara = sess;
			}
		}

		// 毒状態の更新
		if (sess->chara_state[CHARA_STATE_PAIN_INDEX])
		{
			BYTE pkt[MAX_PACKET_SIZE];
			INT packetSize = 0;
			// HPを減らす
			UpdateHPStatus(sess, (int)ceil((-sess->HP_m * CHARA_STATE_PAIN_DAMAGE_FACTOR))-1);
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
			AddPacketAllUser(NULL, pkt, packetSize);
			sess->chara_state[CHARA_STATE_PAIN_INDEX]--;
			// カウント0で状態解除パケット
			if (!sess->chara_state[CHARA_STATE_PAIN_INDEX])
			{
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_PAIN_INDEX, pkt);
				AddPacketAllUser(NULL, pkt, packetSize);
			}
		}
	}

	NotifyTurnStartCharacter(next_chara);
	NotifyTurnStartBullet(next_chara);
	// 連射フラグクリア
	m_bDoubleShotFlg = FALSE;
	// 次のアクティブキャラを設定
	p_pActiveSession = next_chara;

	WCHAR tlog[32];
	SafePrintf(tlog, 32, L"TurnEnd:%d", m_nTurnCount);
	AddMessageLog(tlog);
	// ターン数増加
	m_nTurnCount++;

	SetSyncPhase(GAME_MAIN_PHASE_RETURN);
	return bPacket;
}

BOOL CSyncMain::FrameReturn()
{
	BOOL bPacket= FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;
	
	if (!IsControlChara(p_pActiveSession)
	&& !(p_pActiveSession = GetNextActiveChara()))
	{
		AddMessageLog(L"next active chara nothing!!!\n");
		g_bRestartFlg = TRUE;
		return FALSE;
	}
	if (p_pActiveSession)
	{
		WCHAR actlog[80];
		WCHAR name[MAX_USER_NAME+1];
		common::session::GetSessionName(p_pActiveSession, name);
		SafePrintf(actlog, 80, L"active:%d(%s)",p_pActiveSession->obj_no, name);
		AddMessageLog(actlog);

		p_pActiveSession->obj_state = OBJ_STATE_MAIN_ACTIVE;
		// 次の順番のキャラにアクティブ通知する
		packetSize = PacketMaker::MakePacketData_MainInfoActive(p_pActiveSession, pkt);
		bPacket |= AddPacketNoBlindUser(p_pActiveSession, pkt, packetSize);

		p_pActiveSession->delay = ((TCHARA_SCR_INFO*)p_pActiveSession->scrinfo)->delay;	// 基本ディレイ値を設定しておく
	}
	else
	{
		AddMessageLog(L"!active:none");
	}

	// 次ターンに移行
	SetPhase(GAME_MAIN_PHASE_ACT);
	return bPacket;
}

ptype_session CSyncMain::GetNextActiveChara()
{
	int nMinDelay = MAXINT;
	ptype_session retChara = NULL;
	// ディレイの低いキャラを探す
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		if ( (*it)->connect_state != CONN_STATE_AUTHED) continue;
		// 死体は飛ばす
		if ( (*it)->obj_state == OBJ_STATE_MAIN_DEAD
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP
		)
			continue;

		if (nMinDelay > (*it)->delay)
		{
			retChara = (ptype_session)(*it);
			nMinDelay = (*it)->delay;
		}
		else if (nMinDelay == (*it)->delay)	// 同ディレイ値のときのターン処理
		{
			if (!retChara || retChara->turn_count > (*it)->turn_count )
				retChara = (ptype_session)(*it);
		}
	}
	return retChara;
}


void CSyncMain::NotifyTurnEndCharacter(ptype_session active_sess)
{
	LuaFuncParam luaParams, luaResults;
	// ターンエンド
	if (active_sess && active_sess->connect_state == CONN_STATE_AUTHED)
	{
		luaParams.Number( active_sess->scrinfo->scr_index).Number(active_sess->obj_no).Number(active_sess->turn_count).Number(active_sess->extdata1).Number(active_sess->extdata2);
		common::scr::CallLuaFunc(g_pLuah, "onTurnEnd_Chara", &luaResults, 0, &luaParams, g_pCriticalSection);
	}
}

void CSyncMain::NotifyTurnStartCharacter(ptype_session  next_sess)
{
	LuaFuncParam luaParams, luaResults;
	// ターンスタート
	if (next_sess && next_sess->connect_state == CONN_STATE_AUTHED)
	{
		next_sess->MV_c = next_sess->MV_m = ((TCHARA_SCR_INFO*)next_sess->scrinfo)->move;

		luaParams.Clear();	luaResults.Clear();
		luaParams.Number( next_sess->scrinfo->scr_index).Number(next_sess->obj_no).Number(next_sess->turn_count+1).Number(next_sess->extdata1).Number(next_sess->extdata2);
		common::scr::CallLuaFunc(g_pLuah, "onTurnStart_Chara", &luaResults, 0, &luaParams, g_pCriticalSection);
	}
}

// 
void CSyncMain::NotifyTurnEndBullet(ptype_session active_sess)
{
//	BYTE pkt[MAX_PACKET_SIZE];
//	INT packetSize = 0;

	// オブジェクトなし
	if (m_vecObjectNo.empty())	return;

	BOOL bRemoved = FALSE;
	// 弾ループ
	std::vector<int> vecList;
	vecList.assign(m_vecObjectNo.begin(), m_vecObjectNo.end());
	for (std::vector<int>::iterator itno = vecList.begin();
		itno != vecList.end();
		itno++
		)
	{
		std::map<int, type_obj*>::iterator it = m_mapObjects.find((*itno));
		if (it == m_mapObjects.end())	continue;
		ptype_obj obj = (ptype_obj)(*it).second;

		if (obj->obj_type & OBJ_TYPE_BLT)
		{
			ptype_blt blt = (ptype_blt)obj;

			// 削除フラグ確認
			if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)
			{
				SafeDelete(blt);
				it = m_mapObjects.erase(it);
				bRemoved = TRUE;
				continue;
			}

			if (active_sess)
//			if (active_sess && blt->chr_obj_no == active_sess->obj_no)	// 自キャラの弾以外にも通知
			{
				blt->frame_count = 0;
				blt->turn_count++;
				LuaFuncParam luaResults;
				LuaFuncParam luaParams;
				switch ( blt->proc_type)
				{
				case BLT_PROC_TYPE_SCR_CHARA:
					luaParams.Number( blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->chr_obj_no).Number(blt->obj_no).Number(blt->turn_count).Number(blt->extdata1).Number(blt->extdata2).Number(active_sess->obj_no);
					common::scr::CallLuaFunc(g_pLuah, "onTurnEnd_CharaBullet", &luaResults, 0, &luaParams, g_pCriticalSection);
//					if (blt->lua_flg & (LUA_FLG_OBJ_REMOVE|LUA_FLG_OBJ_UPDATE_VEC|LUA_FLG_OBJ_UPDATE_POS|LUA_FLG_OBJ_UPDATE_STATE|LUA_FLG_OBJ_SET_EXDATA|LUA_FLG_OBJ_UPDATE_ANGLE|LUA_FLG_OBJ_UPDATE_TYPE))
//						bPacket |= TRUE;
					break;
				case BLT_PROC_TYPE_SCR_SPELL:
					luaParams.Number( blt->scrinfo->scr_index).Number(blt->chr_obj_no).Number(blt->obj_no).Number(blt->turn_count).Number(blt->extdata1).Number(blt->extdata2).Number(active_sess->obj_no);
					common::scr::CallLuaFunc(g_pLuah, "onTurnEnd_CharaSpell", &luaResults, 0, &luaParams, g_pCriticalSection);
					break;
				case BLT_PROC_TYPE_SCR_STAGE:
					luaParams.Number(blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->turn_count).Number(blt->extdata1).Number(blt->extdata2).Number(active_sess->obj_no);
					common::scr::CallLuaFunc(g_pLuah, "onTurnEnd_StageBullet", &luaResults, 0, &luaParams, g_pCriticalSection);
//					if (blt->lua_flg & (LUA_FLG_OBJ_REMOVE|LUA_FLG_OBJ_UPDATE_VEC|LUA_FLG_OBJ_UPDATE_POS|LUA_FLG_OBJ_UPDATE_STATE|LUA_FLG_OBJ_SET_EXDATA|LUA_FLG_OBJ_UPDATE_ANGLE|LUA_FLG_OBJ_UPDATE_TYPE))
//						bPacket |= TRUE;
					break;
				}
			}

			WCHAR bltlog[64];
			SafePrintf(bltlog, 64, L"blt(%d)_type[#%d(%d)],chr(%d),turn:%d/rm%d", blt->obj_no, blt->chara_type, blt->bullet_type, blt->chr_obj_no, blt->turn_count,(blt->proc_flg & PROC_FLG_OBJ_REMOVE)?1:0);
			AddMessageLog(bltlog);
			// onTurnEndで削除されているか確認
			if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)
			{
				SafeDelete(blt);
				it = m_mapObjects.erase(it);
				bRemoved = TRUE;
				continue;
			}
		}
	}
	// 削除があったらオブジェクトNoリスト更新
	if (bRemoved)
		UpdateObjectNo();
}

// 
void CSyncMain::NotifyTurnStartBullet(ptype_session  next_sess)
{
//	BYTE pkt[MAX_PACKET_SIZE];
//	INT packetSize = 0;

	// オブジェクトなし
	if (m_vecObjectNo.empty())	return;

	// 弾ループ
	for (std::map<int, type_obj*>::iterator it = m_mapObjects.begin();
		it != m_mapObjects.end();
		it++)
	{
		ptype_obj obj = (ptype_obj)(*it).second;

		if (obj->obj_type & OBJ_TYPE_BLT)
		{
			ptype_blt blt = (ptype_blt)obj;
			if (next_sess)
			{
				LuaFuncParam luaResults;
				LuaFuncParam luaParams;
				switch ( blt->proc_type )
				{
				case BLT_PROC_TYPE_SCR_CHARA:
					luaParams.Number( blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->chr_obj_no).Number(blt->obj_no).Number(blt->turn_count+1).Number(blt->extdata1).Number(blt->extdata2).Number(next_sess->obj_no);
					common::scr::CallLuaFunc(g_pLuah, "onTurnStart_CharaBullet", &luaResults, 0, &luaParams, g_pCriticalSection);
					break;
				case BLT_PROC_TYPE_SCR_SPELL:
					luaParams.Number( blt->scrinfo->scr_index).Number(blt->chr_obj_no).Number(blt->obj_no).Number(blt->turn_count+1).Number(blt->extdata1).Number(blt->extdata2).Number(next_sess->obj_no);
					common::scr::CallLuaFunc(g_pLuah, "onTurnStart_CharaSpell", &luaResults, 0, &luaParams, g_pCriticalSection);
					break;
				case BLT_PROC_TYPE_SCR_STAGE:
					luaParams.Number(blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->turn_count+1).Number(blt->extdata1).Number(blt->extdata2).Number(next_sess->obj_no);
					common::scr::CallLuaFunc(g_pLuah, "onTurnStart_StageBullet", &luaResults, 0, &luaParams, g_pCriticalSection);
					break;
				}
			}
		}
	}
}


void CSyncMain::NotifyTurnEndStage(ptype_session active_sess, ptype_session next_sess)
{
	LuaFuncParam luaResults;
	LuaFuncParam luaParams;
	int act_obj_no = -1;
	if (active_sess && active_sess->connect_state == CONN_STATE_AUTHED)
		act_obj_no = active_sess->obj_no;
	int next_obj_no = -1;
	if (next_sess && next_sess->connect_state == CONN_STATE_AUTHED)
		next_obj_no = next_sess->obj_no;

	luaParams.Number(m_pStageScrInfo->scr_index).Number(m_nTurnCount).Number(act_obj_no).Number(next_obj_no);
	common::scr::CallLuaFunc(g_pLuah, "onTurnEnd_Stage", &luaResults, 0, &luaParams, g_pCriticalSection);
}

bool CSyncMain::DamageCharaHP(int assailant_no, int victim_no, int hp)
{
	ptype_session ass_sess = NULL;
	if (assailant_no != DEF_STAGE_OBJ_NO)
	{
		if ((ass_sess =g_pNetSess->GetSessionFromUserNo(assailant_no)) == NULL)
		{	
			AddMessageLog(L"DamageCharaHP assailant_no is no connect user");
			return false;
		}
	}
	ptype_session vic_sess = NULL;
	if (victim_no != DEF_STAGE_OBJ_NO)
	{
		if ((vic_sess =g_pNetSess->GetSessionFromUserNo(victim_no)) == NULL)
		{	
			AddMessageLog(L"DamageCharaHP assailant_no is no connect user");
			return false;
		}
	}

	int exp = 0;
	// 死んだキャラの場合は無効
	if (vic_sess && vic_sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY|OBJ_STATE_MAIN_GALLERY_FLG))
		return false;

	// EXP更新
	if (hp)
	{
		if (ass_sess)
		{
			if (ass_sess != vic_sess)
				ass_sess->EXP_c = min(((TCHARA_SCR_INFO*)ass_sess->scrinfo)->sc_info.max_exp, abs(hp)+ass_sess->EXP_c);
		}
		if (vic_sess)
			exp = min(((TCHARA_SCR_INFO*)vic_sess->scrinfo)->sc_info.max_exp, abs(hp)+vic_sess->EXP_c)-vic_sess->EXP_c;
	}

	// ルール：チームダメージなし
	if ( !(m_bytRule & GAME_RULE_TEAM_DAMAGE) )
	{
		// 同チームならダメージ変更しない
		if (ass_sess && vic_sess && ass_sess->team_no == vic_sess->team_no)
			return false;
	}

	return UpdateCharaStatus(victim_no, hp, 0,0, exp);
}

bool CSyncMain::UpdateCharaStatus(int obj_no, int hp, int mv, int delay, int exp)
{
	BOOL bPacket = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
/// 20100929⑬
	if (!sess)
		return false;
/// 20100929⑬
	
	// hp 
	{
		LuaFuncParam luaParams, luaResults;
		// 存在しているなら情報取得
		// no, hp, ext1, ext2
		luaParams.Number(sess->scrinfo->scr_index).Number(sess->obj_no).Number(sess->HP_m).Number(sess->HP_c).Number(hp).Number(sess->extdata1).Number(sess->extdata2);
		if (!common::scr::CallLuaFunc(g_pLuah, "onChangeHP_Chara", &luaResults, 1, &luaParams, g_pCriticalSection))
			return false;
		double dResult = luaResults.GetNumber(0);
		if (dResult < 0)
			dResult = dResult - 0.5;
		else if (dResult > 0)
			dResult = dResult + 0.5;
		hp = (int)dResult;
	}

	// 死んでるならステータス変更なし
	if (sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY|OBJ_STATE_MAIN_GALLERY_FLG))
		return true;

	sess->HP_c = max(0, min(sess->HP_c+hp, sess->HP_m));
	sess->MV_c = max(0, min(sess->MV_c+mv, sess->MV_m));
	sess->delay = max(MIN_DELAY_VALUE,min(sess->delay+delay,MAX_DELAY_VALUE));
	sess->EXP_c = max(0,min(((TCHARA_SCR_INFO*)sess->scrinfo)->sc_info.max_exp, sess->EXP_c+exp));

	// HP確認
	if (sess->HP_c <= 0)
	{
		NotifyCharaDead(sess, CHARA_DEAD_KILL);
	}
	else
	{
		// ステータス更新ぱけっと
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
		bPacket = AddPacketAllUser(NULL, pkt, packetSize);
	}
	return true;
}

bool CSyncMain::UpdateCharaPos(int obj_no, int x,int y)
{
	BOOL bPacket = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess)	return false;
	if (sess->obj_state & OBJ_STATE_MAIN_GALLERY_FLG) return false;

	sess->ax = x;
	sess->ay = y;
	// 一更新パケット
	packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
	if (packetSize)
	{
		if (sess->chara_state[CHARA_STATE_STEALTH_INDEX])
			AddPacketTeamUser(sess->team_no, pkt, packetSize);
		else
			AddPacketNoBlindUser(sess, pkt, packetSize);
	}
	return true;
}

BOOL CSyncMain::IsGameEnd()
{
//#if	ONE_CLIIENT
//	return TRUE;
//#endif
	BOOL ret = FALSE;

	// 制限ターン数確認
	if (m_nTurnLimit && m_nTurnLimit <= m_nTurnCount)
		return TRUE;

	// 個人戦
	if (g_pPPPhase->GetTeamCount() <= 1)
		ret = IsGameEndOfIndividualMatch();
	else
		ret = IsGameEndOfTeamBattle();

	return ret;
}

BOOL CSyncMain::IsGameEndOfTeamBattle()
{
	// 生存確認
	for (std::vector <ptype_session>::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		if ((*it)->connect_state != CONN_STATE_AUTHED)
			continue;
		if ((*it)->entity && (*it)->HP_c > 0)
			continue;
		else if ( !((*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)))
		{
			ptype_session sess = (ptype_session)(*it);
			NotifyCharaDead(sess, CHARA_DEAD_KILL);
//			sess->obj_state = OBJ_STATE_MAIN_DEAD;
//			m_nLivingTeamCountTable[sess->team_no]--;
//			LuaFuncParam luaParams, luaResults;
//			luaParams.Number( sess->scrinfo->scr_index).Number(sess->obj_no);
//			common::scr::CallLuaFunc(g_pLuah, "onDead_MyChara", &luaResults, 0, &luaParams, g_pCriticalSection);
//			// 死亡パケット
//			BYTE pkt[MAX_PACKET_SIZE];
//			WORD packetSize = PacketMaker::MakePacketData_MainInfoDeadChara(PK_USER_MAININFO_CHARA_DEAD_KILL, (*it)->obj_no, pkt);
//			AddPacketAllUser(NULL, pkt, packetSize);
		}
	}

	// チーム確認
	int nTeamCount = g_pPPPhase->GetTeamCount();
	int nLivingTeamCount = 0;
	for (int i=0;i<nTeamCount;i++)
	{
		if (m_nLivingTeamCountTable[i] > 0)
			nLivingTeamCount++;
	}
	// 生存チーム数が1以下なら終わり
	return (nLivingTeamCount <= 1);
}

BOOL CSyncMain::IsGameEndOfIndividualMatch()
{
	int nCount = 0;
	for (std::vector <ptype_session>::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		if ((*it)->connect_state != CONN_STATE_AUTHED) continue;
		if ((*it)->entity && (*it)->HP_c > 0)
			nCount++;
		else if ( !((*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)))
		{
			ptype_session sess = (*it);

			NotifyCharaDead(sess, CHARA_DEAD_KILL);
//			(*it)->obj_state = OBJ_STATE_MAIN_DEAD;
//			LuaFuncParam luaParams, luaResults;
//			luaParams.Number( ((ptype_session)(*it))->scrinfo->scr_index).Number((*it)->obj_no);
//			common::scr::CallLuaFunc(g_pLuah, "onDead_MyChara", &luaResults, 0, &luaParams, g_pCriticalSection);
//			// 死亡パケット
//			BYTE pkt[MAX_PACKET_SIZE];
//			WORD packetSize = PacketMaker::MakePacketData_MainInfoDeadChara(PK_USER_MAININFO_CHARA_DEAD_KILL, (*it)->obj_no, pkt);
//			AddPacketAllUser(NULL, pkt, packetSize);
		}
	}
	// 生存者数 一人以下だったら
	if	( g_bOneClient)
	{
		if (nCount <= 0)
			return TRUE;
	}
	else
	{
		if (nCount <= 1)
			return TRUE;
	}
	return FALSE;
}

void CSyncMain::SetRankOrder()
{
	// 個人戦
	if (g_pPPPhase->GetTeamCount() <= 1)
		SetRankOrderOfIndividualMatch();
	else
		SetRankOrderOfTeamBattle();
}

// 個人戦ランク付け
void CSyncMain::SetRankOrderOfIndividualMatch()
{
	t_sessionInfo* pSessInfo = g_pNetSess->GetSessionTable();
	int nCount = 0;
	int nMaxTurn = 0;
	int	nRestHP = 0;
	std::vector<ptype_session> vecEntityCharacters;
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session pSess = &pSessInfo[i].s;
		if (pSess->entity)
			vecEntityCharacters.push_back(pSess);
	}

	int nLiveInc = vecEntityCharacters.size();
	int nNo = 0;
	// 生存ターン数順にframe_countに順位設定
	std::vector<ptype_session>::iterator it;
	while (!vecEntityCharacters.empty())
	{
		nCount = 0;
		nMaxTurn = -1;
		nRestHP = 0;

		it = vecEntityCharacters.begin();

		// 最大live_countを探す
		while (it != vecEntityCharacters.end())
		{
			if ((*it)->connect_state == CONN_STATE_AUTHED
				&& !((*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY))
				)
			{
				(*it)->live_count+=nLiveInc;
			}
			nMaxTurn = max( (*it)->live_count, nMaxTurn);
			// 生存していた場合、残りHPを記憶
			if ( !((*it)->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG ) )
				nRestHP = max( (int)(((float)(*it)->HP_c / (float)(*it)->HP_m) * 100), nRestHP);
//				fRestHP = max( (float)(*it)->HP_c / (float)(*it)->HP_m, fRestHP);
			it++;
		}

		it = vecEntityCharacters.begin();
		while (it != vecEntityCharacters.end())
		{
			if (nMaxTurn == (*it)->live_count)
			{
				if ( !((*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY) ) )
				{
					if (nRestHP == (int)(((float)(*it)->HP_c / (float)(*it)->HP_m)*100) )
					{
						(*it)->frame_count = nNo;
						nCount++;
						it = vecEntityCharacters.erase(it);
					}
					else
					{
						it++;
					}
				}
				else
				{
					(*it)->frame_count = nNo;
					nCount++;
					it = vecEntityCharacters.erase(it);
				}
			}
			else
			{
				it++;
			}
		}
		nNo += nCount;
	}
}

// チーム戦ランク付け
void CSyncMain::SetRankOrderOfTeamBattle()
{
	t_sessionInfo* pSessInfo = g_pNetSess->GetSessionTable();
//	int nCount = 0;

	int nMaxTurn = 0;
	int nMaxTurnTeamNo = -1;
	std::vector<ptype_session> vecEntityCharacters;
	struct TREST_HP_TABLE 
	{
		float fHPAvg;
		int nNum;
		int nNo;
		std::vector<ptype_session> chara;
	} *pRestHPTable;
	pRestHPTable = new TREST_HP_TABLE[g_nMaxLoginNum];

	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session pSess = &pSessInfo[i].s;
		pRestHPTable[i].fHPAvg = 0.0f;
		pRestHPTable[i].nNum = 0;
		pRestHPTable[i].nNo = 0;
		if (pSess->entity)
		{
			vecEntityCharacters.push_back(pSess);
			pRestHPTable[pSess->team_no].chara.push_back(pSess);
		}
	}

	// 残りHPを足し合わせる
	for (std::vector<ptype_session>::iterator it = vecEntityCharacters.begin();it != vecEntityCharacters.end();it++)
	{
		ptype_session sess = (ptype_session)(*it);
		pRestHPTable[sess->team_no].nNum++;
		if (!(sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)) && sess->HP_c>0)
		{
			float fRestHP = (float)sess->HP_c/(float)sess->HP_m;
			pRestHPTable[sess->team_no].fHPAvg += fRestHP;
		}
	}

	int nRestHP = 0;
	for (int i=0;i<g_pPPPhase->GetTeamCount();i++)
	{
		if (pRestHPTable[i].fHPAvg > 0.0f)
			nRestHP++;
	}

	// HPが残ってる人が複数居る場合（決着が付かずに終わった）
	if (nRestHP > 1)
	{
		// 平均HPを計算
		for (int i=0;i<MAXUSERNUM;i++)
		{
			if (!pRestHPTable[i].nNum)
				break;
			if (pRestHPTable[i].fHPAvg>0)
				pRestHPTable[i].fHPAvg = pRestHPTable[i].fHPAvg / (float)pRestHPTable[i].nNum;
		}

		// チーム順位を設定
		int nNo = 0;
		float fMaxHP = 0.0f;
		float fDelHP = 0.0f;
		int nCount = 0;

		std::vector<ptype_session>::iterator it;
		while (!vecEntityCharacters.empty())
		{
			nCount = 1;
			fMaxHP = 0.0f;
			// 残りHPの高いチームを検索
			for (int i=0;i<g_nMaxLoginNum;i++)
			{
				if (!pRestHPTable[i].nNum)	break;
				if (pRestHPTable[i].fHPAvg == fDelHP)
				{
					pRestHPTable[i].fHPAvg = 0.0f;
					continue;
				}

				if (pRestHPTable[i].fHPAvg > fMaxHP)
				{
					fMaxHP = pRestHPTable[i].fHPAvg;
					nCount = 1;
				}
				else if (pRestHPTable[i].fHPAvg == fMaxHP)
					nCount++;
			}

			for (it = vecEntityCharacters.begin();
				it != vecEntityCharacters.end();
				)
			{
				ptype_session sess = (ptype_session)(*it);
				if (pRestHPTable[sess->team_no].fHPAvg == fMaxHP)
				{
					sess->frame_count = nNo;
					it = vecEntityCharacters.erase(it);
				}
				else
				{
					it++;
				}
			}
			// 残りHPを削除
			fDelHP = fMaxHP;
			nNo += nCount;
		}
	}
	else	// 決着付いた
	{
		int nNo = 0;
		int nLiveInc = vecEntityCharacters.size();

		// 生存ターン数順にframe_countに順位設定
		std::vector<ptype_session>::iterator it;
		while (!vecEntityCharacters.empty())
		{
	//		nCount = 0;
			nRestHP = 0;
			nMaxTurn = -1;
			nMaxTurnTeamNo = -1;
			it = vecEntityCharacters.begin();

			// 最大live_countを探す
			while (it != vecEntityCharacters.end())
			{
				if ((*it)->connect_state == CONN_STATE_AUTHED
					&& !((*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY) )
					)
					(*it)->live_count+=nLiveInc;
				if ((*it)->live_count > nMaxTurn)
				{
					nMaxTurn = (*it)->live_count;
					nMaxTurnTeamNo = (*it)->team_no;
					// 生存していた場合、残りHPを記憶
	//				if ( !((*it)->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG ) )
	//					fRestHP = max( (*it)->HP_c/(*it)->HP_m, fRestHP);
				}
				it++;
			}

			it = vecEntityCharacters.begin();
			while (it != vecEntityCharacters.end())
			{
				if (nMaxTurn == (*it)->live_count || nMaxTurnTeamNo == (*it)->team_no)
				{
	/*
					if ( !((*it)->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG ) )
					{
						if (fRestHP == (*it)->HP_c/(*it)->HP_m)
						{
							(*it)->frame_count = nNo;
							it = vecEntityCharacters.erase(it);
						}
						else
						{
							it++;
						}
					}
					else
					{
	*/
						(*it)->frame_count = nNo;
	//					nCount++;
						it = vecEntityCharacters.erase(it);
	//				}
				}
				else
				{
					it++;
				}
			}
			nNo++;
		}
	}
	SafeDeleteArray(pRestHPTable);
}

// 状態更新
BOOL CSyncMain::CountCharacterState(type_session* sess)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	for (int i=0;i<CHARA_STATE_COUNT;i++)
	{
		switch (i)
		{
		case CHARA_STATE_STEALTH_INDEX:
			if (sess->chara_state[i])
			{
				sess->chara_state[i]--;
				// カウント0で状態解除パケット
				if (!sess->chara_state[i])
				{
					// 位置更新
					packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
					ret |= AddPacketNoBlindUser(NULL, pkt, packetSize);
					// 状態更新
					packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, i, pkt);
					ret = AddPacketAllUser(NULL, pkt, packetSize);
				}
			}
			break;
		case CHARA_STATE_BLIND_INDEX:
			if (sess->chara_state[i])
			{
				sess->chara_state[i]--;
				// カウント0で状態解除パケット
				if (!sess->chara_state[i])
				{
					// 全キャラの位置を状態回復したユーザー教える
					for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
						it != m_vecCharacters.end();
						it++)
					{
						ptype_session pPosSess = (*it);
						if (sess == pPosSess) continue;	// 自身
						if (pPosSess->connect_state != CONN_STATE_AUTHED)	continue;
						if (!pPosSess->entity) continue;

						// 観戦じゃなく、自チームじゃない人がステルスなら位置は教えない
						if (pPosSess->chara_state[CHARA_STATE_STEALTH_INDEX]
						&& (pPosSess->team_no != GALLERY_TEAM_NO && pPosSess->team_no != sess->team_no))
							continue;
						// 位置更新
						packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(pPosSess, pkt);
						ret |= AddPacket(sess, pkt, packetSize);
					}
//					// 位置更新
//					packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
//					ret |= AddPacketNoBlindUser(NULL, pkt, packetSize);
					// 状態更新
					packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, i, pkt);
					ret = AddPacketAllUser(NULL, pkt, packetSize);
				}
			}
			break;
		case CHARA_STATE_UPMOVE_INDEX:
		case CHARA_STATE_NOMOVE_INDEX:
		case CHARA_STATE_NOANGLE_INDEX:
		case CHARA_STATE_REVERSE_INDEX:
			if (sess->chara_state[i])
			{
				sess->chara_state[i]--;
				// カウント0で状態解除パケット
				if (!sess->chara_state[i])
				{
					packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, i, pkt);
					ret = AddPacketAllUser(NULL, pkt, packetSize);
				}
			}
			break;
		case CHARA_STATE_SHIELD_INDEX:
		case CHARA_STATE_POWER_INDEX:
		case CHARA_STATE_DOUBLE_INDEX:
		case CHARA_STATE_ITEM_BLIND_INDEX:
		case CHARA_STATE_ITEM_REVERSE_INDEX:
		case CHARA_STATE_ITEM_REPAIRBLT_INDEX:
		case CHARA_STATE_ITEM_TELEPORT_INDEX:
		case CHARA_STATE_ITEM_NOANGLE_INDEX:
		case CHARA_STATE_ITEM_NOMOVE_INDEX:
			break;
		case CHARA_STATE_PAIN_INDEX:	// 20111201毒だけ毎ターン処理
			break;
		}
	}
	// パケットが溜まっていたら
	return ret;
}

// 
BOOL CSyncMain::UpdateCharacterState(type_session* sess)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	// パワーアップ状態の解除
	if (sess->chara_state[CHARA_STATE_POWER_INDEX] == CHARA_STATE_POWERUP_USE)
	{
		sess->chara_state[CHARA_STATE_POWER_INDEX] = 0x0;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_POWER_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
	}

	return ret;
}


BOOL CSyncMain::OnHitStageItemBullet(type_blt* blt)
{
	BOOL ret = FALSE;
	if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)	return FALSE;
//> 20110510
	// オブジェクト削除
	ret = RemoveObject(blt->obj_no, OBJ_RM_TYPE_BOMB)?TRUE:FALSE;
//< 20110510

	switch (blt->bullet_type)
	{
	case CHARA_STATE_ITEM_REVERSE_INDEX:
		ret |= BombItemBulletReverse(blt);	
		break;
	case CHARA_STATE_ITEM_BLIND_INDEX:
		ret |= BombItemBulletBlind(blt);
		break;
	case CHARA_STATE_ITEM_REPAIRBLT_INDEX:
		ret |= BombItemBulletRepair(blt);
		break;
	case CHARA_STATE_ITEM_TELEPORT_INDEX:
		ret |= BombItemBulletTeleport(blt);
		break;
		//////////////////
	case CHARA_STATE_ITEM_DRAIN_INDEX:
		ret |= BombItemBulletDrain(blt);	
		break;
	case CHARA_STATE_ITEM_FETCH_INDEX:
		ret |= BombItemBulletFetch(blt);	
		break;
	case CHARA_STATE_ITEM_EXCHANGE_INDEX:
		ret |= BombItemBulletExchange(blt);	
		break;
	case CHARA_STATE_ITEM_NOANGLE_INDEX:
		ret |= BombItemBulletNoAngle(blt);
		break;
	case CHARA_STATE_ITEM_NOMOVE_INDEX:
		ret |= BombItemBulletNoMove(blt);
		break;
	}
//> 20110510
//	// オブジェクト削除
//	ret |= RemoveObject(blt->obj_no, OBJ_RM_TYPE_BOMB)?TRUE:FALSE;
//< 20110510
	return ret;
}

BOOL CSyncMain::BombItemBulletReverse(type_blt* blt)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// キャラとの当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// スルー
//		if (!(*it)->entity													// 不在
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// 死亡
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// 落下
//			continue;
		if (!(*it)->entity													// 不在
		||	(*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)		// 死亡
		)
			continue;

		type_session* sess = (*it);
		// 高さを確認
		if (sess->ay >= (m_pMainStage->GetStageHeight()+((TCHARA_SCR_INFO*) sess->scrinfo)->rec_tex_chr.bottom/2) )
			continue;
		if (common::obj::IsHit(GAME_ITEM_REVERSE_BOMB_RANGE, &pos, (*it)))
		{
			sess->chara_state[CHARA_STATE_REVERSE_INDEX] = GAME_ITEM_REVERSE_TURN_COUNT;
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_REVERSE_INDEX, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
//			break;
		}
	}
	return ret;
}

BOOL CSyncMain::BombItemBulletBlind(type_blt* blt)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// キャラとの当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// スルー
//		if (!(*it)->entity													// 不在
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// 死亡
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// 落下
//			continue;
		if (!(*it)->entity													// 不在
		||	(*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)		// 死亡
		)
			continue;

		type_session* sess = (*it);
		if (common::obj::IsHit(GAME_ITEM_BLIND_BOMB_RANGE, &pos, (*it)))
		{
			sess->chara_state[CHARA_STATE_BLIND_INDEX] = GAME_ITEM_BLIND_TURN_COUNT;
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_BLIND_INDEX, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
//			break;
		}
	}
	return ret;
}

BOOL CSyncMain::BombItemBulletRepair(type_blt* blt)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	ptype_session blt_sess = g_pNetSess->GetSessionFromUserNo(blt->chr_obj_no);

	int nRepairTotal = 0;
	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// キャラとの当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// スルー
		if (!(*it)->entity													// 不在
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// 死亡
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// 落下
			continue;

		type_session* sess = (*it);
		// 高さを確認
		if (sess->ay >= (m_pMainStage->GetStageHeight()+((TCHARA_SCR_INFO*) sess->scrinfo)->rec_tex_chr.bottom/2) )
			continue;
		if (common::obj::IsHit(GAME_ITEM_REPAIR_BULLET_BOMB_RANGE, &pos, (*it)))
		{
			float fRepair = (sess->HP_m*GAME_ITEM_REPAIR_BULLET_RATE);
			// パワーアップ状態確認
			if (blt_sess && blt_sess->chara_state[CHARA_STATE_POWER_INDEX] == CHARA_STATE_POWERUP_USE)
				fRepair *= CHARA_STATE_POWERUP_FACTOR;

			UpdateHPStatus(sess, (int)fRepair);
			// 自キャラへの回復は加算しない
			if (sess != p_pActiveSession)
				nRepairTotal += (int)fRepair;
			
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
//			break;
		}
	}

	if (nRepairTotal)
	{
		ptype_session blt_sess = g_pNetSess->GetSessionFromUserNo(blt->chr_obj_no);
		// 生存キャラならスペル用ポイントの増加
		if (!(blt_sess->obj_state & OBJ_STATE_MAIN_DEAD_FLG))
			blt_sess->EXP_c = min(blt_sess->EXP_c+nRepairTotal, (((TCHARA_SCR_INFO*)blt_sess->scrinfo)->sc_info.max_exp) );
	}
	return ret;
}

BOOL CSyncMain::BombItemBulletTeleport(type_blt* blt)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	if (!p_pActiveSession)
		return FALSE;

	if (blt->frame_count <= 1)	// 1フレームで当たる場合は移動しない
		return FALSE;

	if (blt->ax < 0 || blt->ax > m_pMainStage->GetStageWidth())	// 画面の外側で当たった場合移動しない
		return FALSE;

	p_pActiveSession->ax = blt->ax;
	p_pActiveSession->ay = blt->ay;
	common::chr::MoveOnStage(m_pMainStage,p_pActiveSession, &g_mapCharaScrInfo);

	packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(p_pActiveSession, pkt);
	ret = AddPacketNoBlindUser(p_pActiveSession, pkt, packetSize);
	
	return ret;
}

BOOL CSyncMain::BombItemBulletDrain(type_blt* blt)				// 吸収弾
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	short sCurrnetHP = p_pActiveSession->HP_c;
	ptype_session blt_sess = g_pNetSess->GetSessionFromUserNo(blt->chr_obj_no);

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// キャラとの当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// スルー
		if (!(*it)->entity													// 不在
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// 死亡
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// 落下
			continue;

		type_session* sess = (*it);
		// 高さを確認
		if (sess->ay >= (m_pMainStage->GetStageHeight()+((TCHARA_SCR_INFO*) sess->scrinfo)->rec_tex_chr.bottom/2) )
			continue;
		// ルール：チームダメージなしなら当たり判定しない
		if ( !(m_bytRule & GAME_RULE_TEAM_DAMAGE) )
		{
			if (p_pActiveSession->team_no == sess->team_no)
				continue;
		}
		// 当たり判定
		if (common::obj::IsHit(GAME_ITEM_DRAIN_BOMB_RANGE, &pos, (*it)))
		{
			D3DXVECTOR2 vecLen = D3DXVECTOR2((float)(*it)->ax-pos.x, (float)(*it)->ay-pos.y);
			float fLength = D3DXVec2LengthSq(&vecLen);

			float fRange = GAME_ITEM_DRAIN_BOMB_RANGE*GAME_ITEM_DRAIN_BOMB_RANGE;
			// 近いほど強くなる
			float fPower = (fRange>fLength)?max((fRange-fLength)/fRange, 0.5f):0.0f;
			
//			float fPower = (fRange>fLength)?1.0f:0.0f;
			// パワーアップ状態確認
			if (blt_sess && blt_sess->chara_state[CHARA_STATE_POWER_INDEX] == CHARA_STATE_POWERUP_USE)
				fPower *= CHARA_STATE_POWERUP_FACTOR;
			short sValue = (short)(sess->HP_m*GAME_ITEM_DRAIN_RATE*fPower);

			UpdateHPStatus(sess, -sValue);
			UpdateHPStatus(p_pActiveSession, sValue);
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
		}
	}

	if (sCurrnetHP != p_pActiveSession->HP_c)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(p_pActiveSession, pkt);
		ret |= AddPacketAllUser(NULL, pkt, packetSize);
	}

	return ret;
}

BOOL CSyncMain::BombItemBulletFetch(type_blt* blt)				// 引き寄せ弾
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// キャラとの当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// スルー
		if (!(*it)->entity													// 不在
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// 死亡
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// 落下
			continue;

		type_session* sess = (*it);
		if (p_pActiveSession == sess)	continue;	// 自分ならスルー
		if (sess->ay >= (m_pMainStage->GetStageHeight()+((TCHARA_SCR_INFO*) sess->scrinfo)->rec_tex_chr.bottom/2) )
			continue;
		if (common::obj::IsHit(GAME_ITEM_FETCH_BOMB_RANGE, &pos, (*it)))
		{
			sess->ax = p_pActiveSession->ax;
			sess->ay = p_pActiveSession->ay;
			common::chr::GetDownStage(m_pMainStage, sess);

			packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
		}
	}
	return ret;
}

BOOL CSyncMain::BombItemBulletExchange(type_blt* blt)		// 位置替え弾
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	short sMx,sMy;	// 自キャラの移動位置
	short sTx = p_pActiveSession->ax;
	short sTy = p_pActiveSession->ay;
	FLOAT fHitLen = (float)(NEED_MAX_TEXTURE_WIDTH*NEED_MAX_TEXTURE_WIDTH);	// 当たった距離
	BOOL bHit = FALSE;		// 当たりフラグ

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// キャラとの当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// スルー
		if (!(*it)->entity													// 不在
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// 死亡
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// 落下
			continue;

		type_session* sess = (*it);
		if (p_pActiveSession == sess)	continue;	// 自分ならスルー
		if (sess->ay >= (m_pMainStage->GetStageHeight()+((TCHARA_SCR_INFO*) sess->scrinfo)->rec_tex_chr.bottom/2) )
			continue;
		if (common::obj::IsHit(GAME_ITEM_EXCHANGE_BOMB_RANGE, &pos, (*it)))
		{
			bHit = TRUE;
			// 一番弾と近いキャラの位置を記憶
			FLOAT fLen = D3DXVec2LengthSq( &(pos-D3DXVECTOR2((float)(*it)->ax, (float)(*it)->ay)));
			if (fHitLen > fLen)
			{
				fHitLen = fLen;
				sMx = (*it)->ax;
				sMy = (*it)->ay;
			}

			// 当たったキャラの位置を変更
			sess->ax = sTx;
			sess->ay = sTy;

			common::chr::GetDownStage(m_pMainStage, sess);

			// パケット作成
			packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
		}
	}

	// 当たったので発射キャラの位置を移動
	if (bHit)
	{
		p_pActiveSession->ax = sMx;
		p_pActiveSession->ay = sMy;
		common::chr::GetDownStage(m_pMainStage, p_pActiveSession);

		packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(p_pActiveSession, pkt);
		ret |= AddPacketAllUser(NULL, pkt, packetSize);	
	}
	return ret;
}

BOOL CSyncMain::BombItemBulletNoAngle(type_blt* blt)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// キャラとの当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// スルー
//		if (!(*it)->entity													// 不在
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// 死亡
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// 落下
//			continue;
		if (!(*it)->entity													// 不在
		||	(*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)		// 死亡
		)
			continue;

		type_session* sess = (*it);
		if (common::obj::IsHit(GAME_ITEM_NOANGLE_BOMB_RANGE, &pos, (*it)))
		{
			sess->chara_state[CHARA_STATE_NOANGLE_INDEX] = GAME_ITEM_NOANGLE_TURN_COUNT;
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_NOANGLE_INDEX, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
//			break;
		}
	}
	return ret;
}

BOOL CSyncMain::BombItemBulletNoMove(type_blt* blt)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// キャラとの当たり判定
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// スルー
		ptype_session sess = (*it);
		if (sess->connect_state != CONN_STATE_AUTHED)	continue;
//		if (!sess->entity)	continue;
		if (!(*it)->entity													// 不在
		||	(*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)		// 死亡
		)
			continue;

		if (common::obj::IsHit(GAME_ITEM_NOMOVE_BOMB_RANGE, &pos, (*it)))
		{
			sess->chara_state[CHARA_STATE_NOMOVE_INDEX] = GAME_ITEM_NOMOVE_TURN_COUNT;
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_NOMOVE_INDEX, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
//			break;
		}
	}
	return ret;
}

// Luaからの処理フラグを立てる
void CSyncMain::SetObjectLuaFlg(int obj_no, DWORD flg, BOOL on)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())	return;
	// 削除フラグが立っていたら何もしない
	if ((*itfind).second->proc_flg & PROC_FLG_OBJ_REMOVE)	return;
	if (on)
		(*itfind).second->proc_flg |= flg;
	else
		(*itfind).second->proc_flg &= (0xFFFFFFFF^flg);

}

int CSyncMain::GetEntityCharacters()
{
	return m_vecCharacters.size();
}

int CSyncMain::GetLivingCharacters()
{
	int nCount=0;
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		if ( (*it)->entity
			&& !((*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)))
			nCount++;
	}
	return nCount;
}

type_session* CSyncMain::GetCharacterAtVector(int index)
{
	if ((int)m_vecCharacters.size() <= index || index < 0)
		return NULL;
	return (type_session*)m_vecCharacters[index];
}

bool CSyncMain::UpdateObjType(int obj_no, BYTE type)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())	return false;
	(*itfind).second->obj_type = ((*itfind).second->obj_type&0xF0 | (OBJ_TYPE_MASK&type));

	// 弾の場合スクリプトにイベント通知
	if ((*itfind).second->obj_type & OBJ_TYPE_BLT)
	{
		ptype_blt blt = (type_blt*)((*itfind).second);
		// ステージが作成した弾か
		if (blt->obj_no == (short)STAGE_OBJ_NO)
		{
			LuaFuncParam luaParams, luaResults;
			// 存在しているなら情報取得
			// script,弾タイプ,弾ObjNo,弾位置x,y/移動x,y/extdata/obj_state
			luaParams.Number(m_pStageScrInfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number((BYTE)(OBJ_TYPE_MASK&type));
			if (!common::scr::CallLuaFunc(g_pLuah, "onUpdateType_StageBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
				return false;
		}
		else
		{
			LuaFuncParam luaParams;
			LuaFuncParam luaResults;
			// script,弾タイプ,弾ObjNo,弾を作ったキャラのObjNo,弾位置x,y/移動x,y/extdata
			if (blt->bullet_type != DEF_BLT_TYPE_SPELL)
			{
				luaParams.Number(blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->chr_obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number((BYTE)(OBJ_TYPE_MASK&type));
				if (!common::scr::CallLuaFunc(g_pLuah, "onUpdateType_CharaBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
					return false;
			}
			else
			{
				luaParams.Number(blt->scrinfo->scr_index).Number(blt->obj_no).Number(blt->chr_obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number((BYTE)(OBJ_TYPE_MASK&type));
				if (!common::scr::CallLuaFunc(g_pLuah, "onUpdateType_CharaSpell", &luaResults, 0, &luaParams, g_pCriticalSection))
					return false;
			}
		}
	}

	return true;
}

type_blt* CSyncMain::GetBulletInfo(int obj_no)
{
	g_pCriticalSection->EnterCriticalSection_Object(L'9');
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())	return NULL;
	type_blt* blt = NULL;
	if ((*itfind).second->obj_type & OBJ_TYPE_BLT)
		blt =(type_blt*) (*itfind).second;
	g_pCriticalSection->LeaveCriticalSection_Object();
	return blt;
}

int CSyncMain::GetBulletAtkValue(int obj_no)
{
	int ret = 0;
	g_pCriticalSection->EnterCriticalSection_Object(L'0');
	type_blt* blt = GetBulletInfo(obj_no);
	if (!blt)	return 0;
	switch (blt->proc_type)
	{
	case BLT_PROC_TYPE_SCR_CHARA:
		ret = ((TCHARA_SCR_INFO*)blt->scrinfo)->blt_info[blt->bullet_type].blt_atk;
		break;
	case BLT_PROC_TYPE_SCR_SPELL:
		ret = ((TCHARA_SCR_INFO*)blt->scrinfo)->sc_info.blt_atk;
		break;
	case BLT_PROC_TYPE_SCR_STAGE:
		ret = ((TSTAGE_SCR_INFO*)blt->scrinfo)->blt_info[blt->bullet_type].blt_atk;
		break;
	default:
		break;
	}
	g_pCriticalSection->LeaveCriticalSection_Object();
	return ret;
}

// セッション切断イベント
void CSyncMain::OnDisconnectSession(ptype_session sess)
{
	if (sess->connect_state == CONN_STATE_AUTHED)
	{
		NotifyCharaDead(sess, CHARA_DEAD_CLOSE, sess);
//	if (g_pPPPhase->GetTeamCount() > 1)
//		m_nLivingTeamCountTable[sess->team_no]--;	
	}
	AddMessageLog(L"CSyncMain::OnDisconnectSession");
	// 20101110
	if (sess == p_pActiveSession)
	{
		p_pActiveSession->vx = 0;
		BYTE pkt[MAX_PACKET_SIZE];
		INT packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(p_pActiveSession,pkt);
		AddPacketAllUser(p_pActiveSession, pkt, packetSize);		
		p_pActiveSession = NULL;
	}
	if (sess == p_pStealSess)
		p_pStealSess = NULL;
}

BOOL CSyncMain::NotifyCharaDead(ptype_session sess, E_TYPE_CHARA_DEAD type, ptype_session ignore_sess)
{
	BYTE pkt[MAX_PACKET_SIZE];
	WORD packetSize = 0;
	E_TYPE_PACKET_MAININFO_HEADER pk_head = PK_USER_MAININFO_CHARA_DEAD_KILL;

	if (!sess->entity) return FALSE;

	BOOL bDead = FALSE;
	if (sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY))
		bDead = TRUE;

	switch (type)
	{
	case CHARA_DEAD_CLOSE:
		sess->obj_state = OBJ_STATE_MAIN_DEAD;
		pk_head = PK_USER_MAININFO_CHARA_DEAD_CLOSE;
		break;
	case CHARA_DEAD_DROP:
		sess->obj_state = OBJ_STATE_MAIN_DROP;
		// 異常状態をクリアしておく
		ZeroMemory(sess->chara_state, sizeof(char)*CHARA_STATE_COUNT);
		pk_head = PK_USER_MAININFO_CHARA_DEAD_DROP;
		break;
	case CHARA_DEAD_KILL:
		sess->obj_state = OBJ_STATE_MAIN_DEAD;
		// 異常状態をクリアしておく
		ZeroMemory(sess->chara_state, sizeof(char)*CHARA_STATE_COUNT);
		pk_head = PK_USER_MAININFO_CHARA_DEAD_KILL;
		break;
	}

	/// 20101018
	if (sess->scrinfo)
	{
		LuaFuncParam luaResults;
		LuaFuncParam luaParams;
		luaParams.Number( sess->scrinfo->scr_index ).Number(sess->obj_no).Number((double)type);
		common::scr::CallLuaFunc(g_pLuah, "onDead_Chara", &luaResults, 0, &luaParams, g_pCriticalSection);
	}

	if (!bDead)
	{
		// 今回死んだ場合イベントを起こす
		sess->HP_c = 0;
		// 20110201
		sess->frame_count = 0;

		WCHAR msglog[64];
		WCHAR wsName[MAX_CHARACTER_NAME+1];
		common::session::GetSessionName(sess,wsName);
		SafePrintf(msglog, 64, L"dead:(%d)[%s]", sess->obj_no, wsName);
		AddMessageLog(msglog);

		// 生存テーブルを減らすか
		if (g_pPPPhase->GetTeamCount() > 1)
		{
			m_nLivingTeamCountTable[sess->team_no]--;
			///
			for (int i=0;i<g_pPPPhase->GetTeamCount();i++)
			{
				SafePrintf(msglog, 64, L"live_table:team%d[%d]", i+1, m_nLivingTeamCountTable[i]);
				AddMessageLog(msglog);
			}
		}
		// 状態をクリアしておく(クライアントも死亡パケット受信時に同処理を個々に行う
//		ZeroMemory(sess->chara_state,sizeof(char)* CHARA_STATE_COUNT);
	}
	
	UpdateWMCopyData();

	// 死亡パケット
	packetSize = PacketMaker::MakePacketData_MainInfoDeadChara(pk_head, sess->obj_no, pkt);
	BOOL bPacket = AddPacketAllUser(ignore_sess, pkt, packetSize);

	return bPacket;
}


void CSyncMain::SetSyncPhase(E_STATE_GAME_MAIN_PHASE next_phase )
{
	if (next_phase == GAME_MAIN_PHASE_CHECK)
	{
//		MessageBox(NULL,L"SetSyncPhase don't set to check phase", L"code error", MB_OK);
		AddMessageLog(L"SetSyncPhase don't set to check phase");
		return;
	}
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		ptype_session sess = (*it);
		// スルー
		if (sess->connect_state != CONN_STATE_AUTHED)	continue;
		if (sess->team_no != GALLERY_TEAM_NO && !sess->entity)	continue;
// 20110428
//		if (!sess->entity)	continue;
		sess->game_ready = 0;
	}
	SetPhase(GAME_MAIN_PHASE_SYNC);
	m_eGameNextPhase = next_phase;
}

// readyが全て1ならTRUEを返す
BOOL CSyncMain::FrameSync()
{
	int nCharactersCount = (int)m_vecCharacters.size();
	if (m_nPhaseReturnIndex >= nCharactersCount)
		return TRUE;

	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	ptype_session sess = m_vecCharacters[m_nPhaseReturnIndex];
	// スルー
	if (sess->connect_state != CONN_STATE_AUTHED)
// 20110428
//	if (sess->connect_state != CONN_STATE_AUTHED	|| !sess->entity)
	{
		m_nPhaseTimeCounter = 0;
		m_nPhaseReturnIndex++;
		return (m_nPhaseReturnIndex >= nCharactersCount);
	}

	// SYN送信
	if (!m_nPhaseTimeCounter++)
	{
		packetSize = PacketMaker::MakePacketData_SYN(pkt);
		AddPacket(sess, pkt, packetSize);
		return FALSE;
	}

	// ack
	if (sess->game_ready)
	{
		m_nPhaseTimeCounter = 0;
		m_nPhaseReturnIndex++;
		return (m_nPhaseReturnIndex >= nCharactersCount);
	}

	// check TimeOut
	if (m_nPhaseTimeCounter > m_nPhaseTime)
	{
		// タイムアウトしたユーザーをキックする
		AddMessageLog(L"AckTimeOut\n");
		KickUser(sess->sess_index);
		
		m_nPhaseTimeCounter = 0;
		m_nPhaseReturnIndex++;
		return (m_nPhaseReturnIndex >= nCharactersCount);
	}

	return FALSE;	
}

void CSyncMain::SetShotPowerStart(ptype_session sess, int type)
{
	m_bShotPowerWorking = TRUE;
	m_nPhaseTimeCounter = 0;
	m_nPhaseTime = GAME_MAIN_PHASE_TIME_SHOTPOWER;
}

// 風の値変更
void CSyncMain::SetWind(int nValue, int nDir)
{
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	// 風向きバランス取り
	if (m_nWind == MAX_WIND_VALUE)
		m_nWindDirValancer = -1;
	else if (m_nWind == MIN_WIND_VALUE)
		m_nWindDirValancer = 1;
	else if (m_nWind <= MAX_WIND_VALUE/2 && m_nWind >= MIN_WIND_VALUE/2)
		m_nWindDirValancer = 0;

	m_nWind = max(MIN_WIND_VALUE,min(nValue,MAX_WIND_VALUE));
	m_nWindDirection = max(MIN_WIND_DIR_VALUE,min(nDir,MAX_WIND_DIR_VALUE));
	packetSize = PacketMaker::MakePacketData_MainInfoSetWind(m_nWind, pkt);
	AddPacketAllUser(NULL, pkt, packetSize);
	
	g_pCriticalSection->EnterCriticalSection_Object(L'L');
	// 弾ループ
	std::vector<int> vecList;
	vecList.assign(m_vecObjectNo.begin(), m_vecObjectNo.end());
	for (std::vector<int>::iterator itno = vecList.begin();
		itno != vecList.end();
		itno++
		)
	{
		std::map<int, type_obj*>::iterator it = m_mapObjects.find((*itno));
		if (it == m_mapObjects.end())	continue;
		ptype_obj obj = (ptype_obj)(*it).second;

		if (!(obj->obj_type & OBJ_TYPE_BLT))	continue;
		ptype_blt blt = (ptype_blt)obj;
		
		switch ( blt->proc_type)
		{
		case BLT_PROC_TYPE_SCR_STAGE:
			if (blt->obj_no == (short)STAGE_OBJ_NO)
			{
				LuaFuncParam luaParams, luaResults;
				// 存在しているなら情報取得
				// script,弾タイプ,弾ObjNo,弾位置x,y/移動x,y/extdata/wind
				luaParams.Number(m_pStageScrInfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number(m_nWind);
				if (!common::scr::CallLuaFunc(g_pLuah, "onChangeWind_StageBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
					continue;
			}
			break;
		case BLT_PROC_TYPE_SCR_CHARA:
		case BLT_PROC_TYPE_SCR_SPELL:
			{
				LuaFuncParam luaParams;
				LuaFuncParam luaResults;
				// script,弾タイプ,弾ObjNo,弾を作ったキャラのObjNo,弾位置x,y/移動x,y/extdata,wind
				if (blt->bullet_type != DEF_BLT_TYPE_SPELL)
				{
					luaParams.Number(blt->scrinfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->chr_obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number(m_nWind);
					if (!common::scr::CallLuaFunc(g_pLuah, "onChangeWind_CharaBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
						continue;
				}
				else
				{
					luaParams.Number(blt->scrinfo->scr_index).Number(blt->obj_no).Number(blt->chr_obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number(m_nWind);
					if (!common::scr::CallLuaFunc(g_pLuah, "onChangeWind_CharaSpell", &luaResults, 0, &luaParams, g_pCriticalSection))
						continue;
				}
			}
		}
	}
	g_pCriticalSection->LeaveCriticalSection_Object();
}
