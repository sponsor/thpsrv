#include "CSyncMain.h"
#include "main.h"
#include "ext.h"

#ifdef _DEBUG
#define SC_NOLIMIT	1			// �X�y�J��������
#endif
#define START_INTERVAL	1	// 1�^�[���ڂ̃C���^�[�o��

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
		MessageBox(g_hWnd, L"�s���ȃX�e�[�W���I������܂����B", L"error", MB_OK);
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
		MessageBox(g_hWnd, L"�X�e�[�W���[�h���s���܂����B\ndata�t�H���_�̍\�����m�F���Ă�������", L"error", MB_OK);
		return FALSE;
	}

	m_nTurnLimit = pPPPhase->GetTurnLimit();
	m_bytRule = pPPPhase->GetRuleFlg();
	// �����҃e�[�u�����Z�b�g
	int nTeamCount = pPPPhase->GetTeamCount();
	int nEntityCount = pNetSess->CalcEntityUserCount();
//> 20101105
	// �`�[���o�g��
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
		// ���݊m�F
		pSess->obj_no = (short)MAXUSERNUM;
		if (!pSess->entity)	continue;
		AddObject((type_obj*)pSess);
		pSess->game_ready = 0;

		// �����m�F���ăe�[�u���X�V
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
		// ���݊m�F
		pSess->obj_no = MAXUSERNUM;
		if (!pSess->entity)	continue;
		AddObject((type_obj*)pSess);
		pSess->game_ready = 0;

		// �����m�F���ăe�[�u���X�V
		if (!(pSess->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG))
		{
			if (nTeamCount > 1)
				m_nLivingTeamCountTable[pSess->team_no]++;
		}
	}
*/
	g_pCriticalSection->LeaveCriticalSection_Session();

	// ���[���F����������Ȃ��m�F�A�����������_��
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
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 5;
	int nUserNum = 0;

	std::vector<int> vecGroundXPos;
	if (!m_pMainStage->GetGroundsXPos(&vecGroundXPos))
	{
		AddMessageLog(L"�n�`�f�[�^���s���ȃX�e�[�W�摜�ł�");
		MessageBox(NULL, L"�n�`�f�[�^���s���ȃX�e�[�W�摜�ł�", L"error", MB_OK);
		return FALSE;
	}

	g_pCriticalSection->EnterCriticalSection_Session(L'^');
	// �ڑ��ς݃��[�U�̏�Ԃ�؂�ւ���
	t_sessionInfo* ptbl = p_pNWSess->GetSessionTable();
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		ptype_session pSess = &ptbl[i].s;
//		if (!pSess->entity) continue;
//		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		// �؂�ւ�
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
			// �X�e�[�W�ɒu��
			PutCharacter(&vecGroundXPos, pSess);
		}

		nUserNum++;
		// �p�P�b�g�ǉ�
		packetSize = PacketMaker::AddPacketData_MainStart(packetSize, pSess, pktdata);

		if (pSess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_MAIN_GALLERY_FLG)) continue;

		//LuaFuncParam luaParams,luaResults;
		//// ���[�U�ԍ��ƈʒu
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
		// ���[�U�ԍ��ƈʒu
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
		// �F�؍ς݂�
//		if (!pSess->entity) continue;
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		// �؂�ւ�
		if (pSess->team_no == GALLERY_TEAM_NO)
		{
			pSess->obj_state = OBJ_STATE_MAIN_GALLERY;
		}
		else
		{
			pSess->obj_state = OBJ_STATE_MAIN_WAIT;

			// �X�e�[�W�ɒu��
			PutCharacter(&vecGroundXPos, pSess);
		}
		nUserNum++;

		// �p�P�b�g�ǉ�
		packetSize = PacketMaker::AddPacketData_MainStart(packetSize, pSess, pktdata);
	}
	// �L�������[�h�C�x���g���Ăԃ��[�v
	nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// �F�؍ς݂�
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		if (!(pSess->entity) || pSess->obj_state == OBJ_STATE_MAIN_DROP) continue;

		LuaFuncParam luaParams,luaResults;
		// ���[�U�ԍ��ƈʒu
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

	// �p�P�b�g�쐬
	if (nUserNum)
	{
		PacketMaker::MakePacketHeadData_MainStart(nUserNum, pktdata);
		packetSize = PacketMaker::AddEndMarker(packetSize, pktdata);
		// �S���[�U�ɑ��M
		if (packetSize)
			ret = AddPacketAllUser(NULL, pktdata, packetSize);
	}

	if (g_bOneClient)
	{
		BOOL ret = (nUserNum <= 1);
		if (!ret)
			AddMessageLog(L"ONE_CLIENT�I�v�V�������s��v");
		return ret;
	}
	return (nUserNum>1);
}

// �L�����N�^������
void CSyncMain::PutCharacter(std::vector<int>* vecGrounds, ptype_session sess, BOOL PutOnly)
{
	D3DXVECTOR2	vecPos;
	D3DXVECTOR2	vecPut;
	D3DXVECTOR2	vecVec = D3DXVECTOR2(0,0);
	D3DXVECTOR2	vecGround = D3DXVECTOR2(0,0);

	int nRndWidth = vecGrounds->size();
	vecPos.x = (float)( (*vecGrounds)[genrand_int32()%nRndWidth]);
	vecPos.y = (float)(genrand_int32()%g_pSyncMain->GetStageHeight());
	// �n�ʂ�T��
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
	// �n�ʂ�T��
	if ( g_pSyncMain->m_pMainStage->FindGround(&vecPut, &vecGround,  &vec, CHARA_BODY_RANGE) )
	{
		vec = vecPut - vecPos;
		sess->angle = (GetAngle(vec.x, vec.y)+REVERSE_ANGLE)%360;
	}

	if (!PutOnly)
	{
		// �L�����̏����X�e�[�^�X�Ȃǐݒ�
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
		if (m_nPhaseTimeCounter < m_nPhaseTime)	// ���ԓ�
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
		// ���t�F�[�Y
		if (bNextPhase)
		{
			// �ړ����������ꍇ�͈ړ����~�߂�
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
			// �p���[���[�^�̑������Ԃ������ς��Ȃ�
			if (p_pActiveSession && m_bShotPowerWorking)
			{
				// 20140810�@�V���b�g�p���[MAX�֘A�̖��
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
				// �t�F�[�Y�ύX
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
		// �e���˃L���������߂ɂȂ��Ă��Ȃ����m�F�A�Ȃ��Ă�����^�[���I���Ɉȍ~
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
		// �e���˃L���������߂ɂȂ��Ă��Ȃ����m�F�A�Ȃ��Ă�����^�[���I���Ɉȍ~
		if (p_pActiveSession
		&& (p_pActiveSession->connect_state == CONN_STATE_AUTHED)
		&& (p_pActiveSession->obj_state & OBJ_STATE_GAME)
		&& !(p_pActiveSession->obj_state & (OBJ_STATE_MAIN_NOACT_FLG|OBJ_STATE_MAIN_NOLIVE_FLG))
		)
		{
			m_nPhaseTimeCounter++;
			g_pPPMain->ShootingBullet(p_pActiveSession, m_nPhaseTimeCounter);

			if (m_nPhaseTimeCounter > m_nPhaseTime	// ���ԊO
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

		// �L�����̈ړ�
		FrameCharacters();
		// �e�̈ړ�
		FrameObjects();
		g_pCriticalSection->LeaveCriticalSection_Object();
		bGameEnd = IsGameEnd();
		g_pCriticalSection->LeaveCriticalSection_Session();
		break;
	case GAME_MAIN_PHASE_SHOT:
		g_pCriticalSection->EnterCriticalSection_Session(L'#');
		// �L�����̈ړ�
		FrameCharacters();
		g_pCriticalSection->EnterCriticalSection_Object(L'5');
		// �e�̈ړ�
		if (!FrameObjects())
		{
			// �A�N�e�B�u�Ȓe�������Ȃ����ꍇ�A�񔭖ڂ������m�F
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
			// �p���[�A�b�v��Ԃ��N���A
			if (p_pActiveSession->chara_state[CHARA_STATE_POWER_INDEX] == CHARA_STATE_POWERUP_USE)
			{
				p_pActiveSession->chara_state[CHARA_STATE_POWER_INDEX] = CHARA_STATE_POWERUP_OFF;
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(p_pActiveSession, CHARA_STATE_POWER_INDEX, pkt);
				AddPacketAllUser(NULL, pkt, packetSize);
			}
			// ���O�ɓ�A���̏�Ԃ��N���A
			p_pActiveSession->chara_state[CHARA_STATE_DOUBLE_INDEX] = 0x0;
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(p_pActiveSession, CHARA_STATE_DOUBLE_INDEX, pkt);
			AddPacketAllUser(NULL, pkt, packetSize);
//			g_pPPMain->TriggerStart(p_pActiveSession);
			g_pPPMain->ShootingBullet(p_pActiveSession, 0);		// �񔭖ڂ𔭎�
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

	// ���C���Q�[���I��
	if (bGameEnd || m_bKillGame)
	{
		GameEnd();
	}
	return bCharaPacket|bObjectPacket|m_bPacket|bGameEnd;
}

// �I��
void CSyncMain::GameEnd()
{
	g_pCriticalSection->EnterCriticalSection_Session(L'(');
	AddMessageLog(L"GameEnd");
	// ���ʕt��
	SetRankOrder();

	t_sessionInfo* pSessInfo = g_pNetSess->GetSessionTable();
	// �p�P�b�g���M
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
	
	// ���ʂ�
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
	// �e�I�u�W�F�N�g�̈ړ�
	for (std::map<int, type_obj* >::iterator it = m_mapObjects.begin();
		it != m_mapObjects.end();
		it++)
		m_vecObjectNo.push_back( (int)(*it).first );
}

//> Lua�ł��g��
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
	// �������x�ɂ͉����x�̔����𑫂��Ă���
	blt->vx += (short)(blt->adx*0.5);
	blt->vy += (short)(blt->ady*0.5);
	
	blt->bx = blt->ax*BLT_POS_FACT_N;
	blt->by = blt->ay*BLT_POS_FACT_N;
	blt->scrinfo = NULL;
	switch (blt->proc_type)
	{
	case BLT_PROC_TYPE_SCR_CHARA:
		// �L�����X�N���v�g�Ɗ֘A�t��
		blt->scrinfo = common::scr::FindCharaScrInfoFromCharaType(blt->chara_type, &g_mapCharaScrInfo);
		blt->hit_range = ((TCHARA_SCR_INFO*)blt->scrinfo)->blt_info[blt->bullet_type].hit_range;
		break;
	case BLT_PROC_TYPE_SCR_STAGE:
		// �X�e�[�W�X�N���v�g�Ɗ֘A�t��
		blt->scrinfo = m_pStageScrInfo;
		blt->hit_range = m_pStageScrInfo->blt_info[blt->bullet_type].hit_range;
		break;
	case BLT_PROC_TYPE_SCR_SPELL:
		// �L�����X�N���v�g�Ɗ֘A�t��
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

	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
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
	// �폜�t���O�m�F
	if (obj->proc_flg & PROC_FLG_OBJ_REMOVE)
		return true;
	// ����ł������ԕύX�͍s��Ȃ�
	if (obj->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG)
		return true;

	if (obj)
	{
		// ��Ԑݒ�
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
			// �X�e�[�W���쐬�����e��
			if (blt->obj_no == (short)STAGE_OBJ_NO)
			{
				LuaFuncParam luaParams, luaResults;
				// ���݂��Ă���Ȃ���擾
				// script,�e�^�C�v,�eObjNo,�e�ʒux,y/�ړ�x,y/extdata/obj_state
				luaParams.Number(m_pStageScrInfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number((DWORD)blt->obj_state&OBJ_STATE_MAIN_MASK);
				if (!common::scr::CallLuaFunc(g_pLuah, "onUpdateState_StageBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
					return false;
			}
			else
			{
				LuaFuncParam luaParams;
				LuaFuncParam luaResults;
				// script,�e�^�C�v,�eObjNo,�e��������L������ObjNo,�e�ʒux,y/�ړ�x,y/extdata
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
		// ��Ԑݒ�
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
		// ��Ԑݒ�
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

	// �����͈͂𓾂�A�����C�x���g��ʒm����
	// �X�e�[�W���쐬�����e��
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

	// �e�̍���
	ptype_session blt_sess = g_pNetSess->GetSessionFromUserNo(blt_chr_no);

	//> ��������O�ɔ͈͓��̃L�������m�F
	// (�V�[���h�����̃L���������邩�m�F)
	// �����͈̔�
	float fRR = (float)(range*range);

	g_pCriticalSection->EnterCriticalSection_Session(L')');

	// �L�����Ɣ����͈͂̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		ptype_session sess = (*it);
		// ��������͔�΂�
//		if (sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY))
//			continue;
		D3DXVECTOR2 vecLen = D3DXVECTOR2((float)sess->ax-pos.x, (float)sess->ay-pos.y);
		float fLength = D3DXVec2LengthSq(&vecLen);
		if (fLength <= fRR)
		{
			// �L�����ɓ�����
			// �V�[���h��Ԋm�F
			if (sess->chara_state[CHARA_STATE_SHIELD_INDEX])
			{
				// �V�[���h������
				sess->chara_state[CHARA_STATE_SHIELD_INDEX] = 0;
				// �e���폜
				RemoveObject(blt_no, OBJ_RM_TYPE_SHIELD);
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_SHIELD_INDEX, pkt);
				AddPacketAllUser(NULL, pkt, packetSize);
				g_pCriticalSection->LeaveCriticalSection_Session();
				SafeDeleteArray(tblHit);
				SafeDeleteArray(tblPower);
				return true;
			}
			// �߂��قǋ����Ȃ�
			float fPower = (fRR>fLength)?(fRR-fLength)/fRR:0.0f;
			// �p���[�A�b�v��Ԋm�F
			if (blt_sess && blt_sess->chara_state[CHARA_STATE_POWER_INDEX] == CHARA_STATE_POWERUP_USE)
				fPower *= CHARA_STATE_POWERUP_FACTOR;

			tblPower[sess->sess_index] = ((int)(fPower*100.0f));	// 100%�ɕϊ�
			tblHit[sess->sess_index] = TRUE;
		}
	}
	//< ��������O�ɔ͈͓��̃L�������m�F

	// �e�̏����擾
	ptype_blt blt = NULL;
	std::map<int, type_obj*>::iterator itblt = m_mapObjects.find(blt_no);
	if (itblt != m_mapObjects.end())
	{
		if ( (*itblt).second->obj_type & OBJ_TYPE_BLT)
			blt = (ptype_blt)(*itblt).second;							// �e
	}

	// �������a����ō폜�w�肠��Ȃ�X�e�[�W�폜
	int nRetErasePixels = 0;
	if (range && erase)
	{
		//> �X�e�[�W�폜
		g_pCriticalSection->EnterCriticalSection_StageTexture(L'1');
		nRetErasePixels = m_pMainStage->EraseStage(&pos, range);	// �X�e�[�W�폜����s�N�Z�������Ԃ��Ă���
		g_pCriticalSection->LeaveCriticalSection_StageTexture();
		if (nRetErasePixels)
		{
			// �X�e�[�W�X�N���v�g�փX�e�[�W�폜�C�x���g
			LuaFuncParam luaParams,luaResults;
			
			// �L���������˂����e�̏ꍇ�A�X�e�[�W�폜�C�x���g���N����
//> 20110420 �X�e�[�W���쐬�����e�̏ꍇ�AonErase_Stage���Ăяo����Ȃ������̂��C��
//			if (blt_chr_no != STAGE_OBJ_NO)
//< 20110420 �X�e�[�W���쐬�����e�̏ꍇ�AonErase_Stage���Ăяo����Ȃ������̂��C��
			{
				// �X�e�[�W�X�N���v�g�ԍ�,�e�̃^�C�v,���������e�̈ʒux,y/�����ʒux,y/�͈͂̋߂��ɂ��З͒l/extdata
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
		//< �X�e�[�W�폜
	}

	// �����C�x���g��ʒm����
	if (blt_chr_no == DEF_STAGE_OBJ_NO)
	{
		// Lua�ɒe�����C�x���g�ʒm
		LuaFuncParam luaParams,luaResults;
		// script,�e�^�C�v,�eObjNo,�e��������L������ObjNo,�e�ʒux,y/�ړ�x,y/extdata
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
		// Lua�ɒe�����C�x���g�ʒm
		LuaFuncParam luaParams,luaResults;
		if (blt_type != DEF_BLT_TYPE_SPELL)
		{
			// script,�e�^�C�v,�eObjNo,�e��������L������ObjNo,�e�ʒux,y/�ړ�x,y/extdata
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
			// script,�eObjNo,�e��������L������ObjNo,�e�ʒux,y/�ړ�x,y/extdata
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
	// �p�P�b�g�쐬
	packetSize = PacketMaker::MakePacketHeader_MainInfoBombObject(scr_id, blt_type,blt_chr_no, blt_no, pos_x, pos_y, erase, pkt);

	// �����͈͓��̃L�����ɃC�x���g�ʒm
	t_sessionInfo* tblSess = g_pNetSess->GetSessionTable();
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		if (!tblHit[i])	continue;
		ptype_session sess = &tblSess[i].s;
		float fPower = tblPower[i]*0.01f;	// �����_�ɕϊ�
		// �L�����̃X�N���v�g�擾
		if (blt_sess)
		{
			// Lua�ɔ����͈͓��̃L������m�点��
			LuaFuncParam luaParams,luaResults;
			if (blt_type != DEF_BLT_TYPE_SPELL)
			{
				// script,�e�^�C�v,���������L������ObjNo,�e��������L������ObjNo,���������e��ObjNo,���������e�̈ʒux,y/�����ʒux,y/�͈͂̋߂��ɂ��З͒l
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
				// script,���������L������ObjNo,�e��������L������ObjNo,���������e��ObjNo,���������e�̈ʒux,y/�����ʒux,y/�͈͂̋߂��ɂ��З͒l
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
			// Lua�ɔ����͈͓��̃L������m�点��
			LuaFuncParam luaParams,luaResults;
			// script,�e�^�C�v,���������L������ObjNo,���������e�̈ʒux,y/�����ʒux,y/�͈͂̋߂��ɂ��З͒l
			luaParams.Number(m_pStageScrInfo->scr_index).Number(blt_type).Number(sess->obj_no).Number(blt_no).Number(pos_x).Number(pos_y).Number(fPower);
			if (!common::scr::CallLuaFunc(g_pLuah,"onHitChara_StageBulletBomb", &luaResults, 0, &luaParams, g_pCriticalSection))
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				SafeDeleteArray(tblHit);
				SafeDeleteArray(tblPower);
				return false;
			}
		}
		// �p�P�b�g�ǉ�
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
			// �L�����̃X�N���v�g�擾
			TCHARA_SCR_INFO* pCharaScrInfo = common::scr::FindCharaScrInfoFromCharaType(scr_id, &g_mapCharaScrInfo);
			SafePrintf(path, _MAX_PATH, pCharaScrInfo->tex_path);
		}
		break;
	}
	// �摜�\��t��
	BOOL ret = m_pMainStage->PasteImage(g_pFiler, path,&rcPasteImage, sx, sy);
	g_pCriticalSection->LeaveCriticalSection_StageTexture();

	// �摜�\��t���p�P�b�g���M
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
	// �폜�t���O�m�F
	if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)
		return true;
	*/
	if (index >= OBJ_OPTION_COUNT)
	{
		MessageBox(NULL, L"SetBulletOptionData��index�l�𒴂��Ă��܂�", L"lua error", MB_OK); 
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
		MessageBox(NULL, L"GetBulletOptionData��index�l�𒴂��Ă��܂�", L"lua error", MB_OK); 
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
	// �폜�t���O�m�F
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
	// �폜�t���O�m�F
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
		MessageBox(NULL, L"GetCharaOptionData��index�l�𒴂��Ă��܂�", L"lua error", MB_OK); 
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
		MessageBox(NULL, L"GetCharaOptionData��index�l�𒴂��Ă��܂�", L"lua error", MB_OK); 
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

// �Q�[�����ɃL�����̏����A�C�e���ǉ�
bool CSyncMain::AddCharaItem(int obj_no, DWORD item_flg)
{
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	if (!sess) return false;

	// �A�C�e���t���O�`�F�b�N
	if (!(item_flg&GAME_ITEM_ENABLE_FLG))
		return false;

	// �󂫃X���b�g��T��
	int nSlot = 0;
	for (nSlot=0;nSlot<GAME_ITEM_STOCK_MAX_COUNT;nSlot++)
	{
		if (!sess->items[nSlot])	// �󂫃X���b�g
			break;
	}

	// �󂫂������ꍇ
	if (nSlot >= GAME_ITEM_STOCK_MAX_COUNT)
		return UpdateCharaStatus(obj_no,0,0,0,GAME_ADD_ITEM_ENOUGH_EXP);

	// �A�C�e���ǉ�
	sess->items[nSlot] = item_flg;
	// �p�P�b�g���M
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

///< Lua�ł��g��

//> Frame
// �A�N�e�B�u�^�[���ȃL�����̃t���[������
BOOL CSyncMain::FrameActiveCharacter()
{
	// �A�N�e�B�u�L���������Ȃ��ꍇ�I��
	if (!p_pActiveSession) return TRUE;
	if (p_pActiveSession->connect_state != CONN_STATE_AUTHED) return TRUE;
	if (!(p_pActiveSession->obj_state & OBJ_STATE_GAME)) return TRUE;
	if (p_pActiveSession->obj_state & (OBJ_STATE_MAIN_NOACT_FLG|OBJ_STATE_MAIN_NOLIVE_FLG))	return TRUE;
	// �A�N�e�B�u�L�����̈ړ�
	MoveCharacter(p_pActiveSession);
	// �A�N�e�B�u�p��
	return FALSE;
}

// �L�����̃t���[������
BOOL CSyncMain::FrameCharacters()
{
	BOOL bMoved = FALSE;
	// �e�L�����̈ړ�
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		ptype_session sess = (*it);
		// �Q�[�����L�����ȊO�Ƃ΂�
		if (!sess->entity) continue;
//		// �A�N�e�B�u�L�������Ƃ΂�
//		if (sess == p_pActiveSession) continue;
		// ���������Ԋm�F
		if (sess->obj_state & OBJ_STATE_MAIN_DROP_FLG) continue;
		// �ړ��i�����m�F�j
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
	// �n�ʂ�T��
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
// 20101218 ���s���L�����̗�������\�h���郋�[��
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
		// ���g�̊m�F
		if (!p_pActiveSession->game_ready)	return;
	}

	BOOL bRecvAck = FALSE;
	BOOL bReqShot = TRUE;
	// �e�L�����̃g���K�I���m�F
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

		// �ő�҂����Ԃ��z���Ă����B�N������OK�ȏꍇ�̓^�C���A�E�g
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

		// ���˂���L�������X�e���X���
		if (p_pActiveSession->chara_state[CHARA_STATE_STEALTH_INDEX])
		{
			if (sess == p_pActiveSession && !sess->game_ready)
			{
				bReqShot = FALSE;
				continue;
			}
			// ���`�[���̂݌�����̂ŁA�ʃ`�[���͊m�F���Ȃ�
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
			// �Ó]�L�����ȊO�̊m�F
			bReqShot = FALSE;
			continue;
		}
	}

	// �S�L��������OK�A�e���˗v��
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
		// �g���K�[�I���ʒm��v�����Ă݂�
		WCHAR log[32];
		BYTE pkt[MAX_PACKET_SIZE];
		INT packetSize = 0;
		packetSize = PacketMaker::MakePacketData_MainInfoReqTriggerEnd(p_pActiveSession->obj_no, pkt);

		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			// �F�؍ς݂�
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

	// �I�u�W�F�N�g�Ȃ�
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
		// �폜�t���O�m�F
		if (obj->proc_flg & PROC_FLG_OBJ_REMOVE)	continue;

		ptype_blt blt = (ptype_blt)obj;
		// ��p
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
//					it = EraseMapObject(it);		// TURNEND�ō폜����悤�ɕύX
//< 20101218
					bRemoved = TRUE;
					continue;
				}
				break;
			case common::blt::MOVE_ACT_BULLET_RESULT_REMOVE:		// �X�N���v�g�ɂ���č폜���ꂽ
				bRemoved = TRUE;
				break;
			case common::blt::MOVE_ACT_BULLET_RESULT_VEC_CHANGE:	// �ړ��l�ύX
				packetSize = PacketMaker::AddPacketData_MainInfoMoveBullet(packetSize, blt, pkt);
				nVecChangeBulletCount++;
				break;
			case common::blt::MOVE_ACT_BULLET_RESULT_CHARA_HIT:
			case common::blt::MOVE_ACT_BULLET_RESULT_STAGE_HIT:
				OnHitStageItemBullet(blt);
				break;
			case common::blt::MOVE_ACT_BULLET_RESULT_MOVED:	// �ʏ�ړ�
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
//					it = EraseMapObject(it);		// TURNEND�ō폜����悤�ɕύX
//< 20101218
					bRemoved = TRUE;
					continue;
				}
				break;
			case common::blt::MOVE_WAIT_BULLET_RESULT_REMOVED:		// �X�N���v�g�ɂ���č폜���ꂽ
				bRemoved = TRUE;
				break;
			case common::blt::MOVE_WAIT_BULLET_RESULT_VEC_CHANGE:	// �ړ��l�ύX
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

		// �폜����Ă��Ȃ���
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

	// �ړ��l�̕ύX���������e�����邩
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
	// �����������_��
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
	// �A�N�e�B�u�L�����̃f�B���C�l�v�Z
	if (p_pActiveSession)
	{
		p_pActiveSession->turn_count++;	// �^�[��������
//> 20100213 �o�ߎ��Ԃ̌v�Z
		float fMaxPassageTime = (float)(g_pPPMain->GetActTimeLimit()*RUN_FRAMES);
		int nTimeOfDelay = p_pActiveSession->frame_count?(int) (((float)(p_pActiveSession->frame_count+1) / fMaxPassageTime) * MAX_PASSAGE_TIME_DELAY_VALUE):0;
//		int nTimeOfDelay = p_pActiveSession->frame_count?(int) (((float)p_pActiveSession->frame_count / (float)MAX_PASSAGE_TIME) * MAX_PASSAGE_TIME_DELAY_VALUE):0;
//> 20100213 �o�ߎ��Ԃ̌v�Z
//		int nCharaDelay = ((TCHARA_SCR_INFO*)p_pActiveSession->scrinfo)->delay;
		int nMoveOfDelay = (int) ( ((float)(p_pActiveSession->MV_m-min(p_pActiveSession->MV_c,p_pActiveSession->MV_m)) / (float)p_pActiveSession->MV_m) * MOVE_DELAY_FACT);
		WCHAR delaylog[64];
		SafePrintf(delaylog, 64, L"delay:%d+time(%d)/move(%d)", p_pActiveSession->delay, nTimeOfDelay,/*nCharaDelay,*/nMoveOfDelay);
		AddMessageLog(delaylog);

		p_pActiveSession->delay += (nTimeOfDelay/* + nCharaDelay*/+nMoveOfDelay);

		// ��Ԃ̃J�E���g
		bPacket |= CountCharacterState(p_pActiveSession);
		// ��Ԃ̊m�F
		bPacket |= UpdateCharacterState(p_pActiveSession);
	}

	// �X�e�B�[�����̃L�����������ꍇ
	if (p_pStealSess)
	{
		switch (p_pStealSess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX])
		{
		case GAME_ITEM_STEAL_SET:		// �g�p���ꂽ��
			p_pStealSess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] = GAME_ITEM_STEAL_USING;	// �g�p���ɐݒ�
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(p_pStealSess, CHARA_STATE_ITEM_STEAL_INDEX, pkt);
			bPacket = AddPacketAllUser(NULL, pkt, packetSize);
			break;
		case GAME_ITEM_STEAL_USING:
			p_pStealSess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] = GAME_ITEM_STEAL_OFF;	// ���g�p�ɐݒ�
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(p_pStealSess, CHARA_STATE_ITEM_STEAL_INDEX, pkt);
			p_pStealSess = NULL;
			bPacket |= AddPacketAllUser(NULL, pkt, packetSize);
			break;
		default:
			p_pStealSess = NULL;
			break;
		}
	}

	// �f�B���C�̒Ⴂ�L�������S���̃f�B���C�����炷
//> 20110503
//	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
//		it != m_vecCharacters.end();
//		it++)
//	{
//		ptype_session sess = (*it);
	NotifyTurnEndCharacter(p_pActiveSession);
	// �^�[���G���h�A�X�^�[�g�C�x���g
	NotifyTurnEndStage(p_pActiveSession, next_chara);
	NotifyTurnEndBullet(p_pActiveSession);

	// �f�B���C�̒Ⴂ�L������T��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		ptype_session sess = (*it);
		if (sess->connect_state != CONN_STATE_AUTHED)	continue;
		if (!sess->entity)	continue;
		if (sess->obj_state & OBJ_STATE_MAIN_NOLIVE_FLG) continue;

		(*it)->live_count++;				// �����^�[��������
		(*it)->frame_count = 0;		// �t���[��������
		(*it)->MV_c = (*it)->MV_m;	// �ړ��l�̏�����
		(*it)->obj_state = OBJ_STATE_MAIN_WAIT;	// ��Ԃ�������

		nMinDelay = min(nMinDelay, (*it)->delay);	// �ŏ��f�B���C�l��T��
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
		// �f�B���C�l�A�A�N�e�B�u�����e�Z�b�V�����ɑ��M
		packetSize = PacketMaker::MakePacketData_MainInfoTurnEnd(sess, m_nWind, pkt);
		bPacket |= AddPacket(sess, pkt, packetSize);

		// ���񂾃L�����̓f�B���C�l�v�Z���Ȃ�
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

		// �ŏ�Ԃ̍X�V
		if (sess->chara_state[CHARA_STATE_PAIN_INDEX])
		{
			BYTE pkt[MAX_PACKET_SIZE];
			INT packetSize = 0;
			// HP�����炷
			UpdateHPStatus(sess, (int)ceil((-sess->HP_m * CHARA_STATE_PAIN_DAMAGE_FACTOR))-1);
			packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
			AddPacketAllUser(NULL, pkt, packetSize);
			sess->chara_state[CHARA_STATE_PAIN_INDEX]--;
			// �J�E���g0�ŏ�ԉ����p�P�b�g
			if (!sess->chara_state[CHARA_STATE_PAIN_INDEX])
			{
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_PAIN_INDEX, pkt);
				AddPacketAllUser(NULL, pkt, packetSize);
			}
		}
	}

	NotifyTurnStartCharacter(next_chara);
	NotifyTurnStartBullet(next_chara);
	// �A�˃t���O�N���A
	m_bDoubleShotFlg = FALSE;
	// ���̃A�N�e�B�u�L������ݒ�
	p_pActiveSession = next_chara;

	WCHAR tlog[32];
	SafePrintf(tlog, 32, L"TurnEnd:%d", m_nTurnCount);
	AddMessageLog(tlog);
	// �^�[��������
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
		// ���̏��Ԃ̃L�����ɃA�N�e�B�u�ʒm����
		packetSize = PacketMaker::MakePacketData_MainInfoActive(p_pActiveSession, pkt);
		bPacket |= AddPacketNoBlindUser(p_pActiveSession, pkt, packetSize);

		p_pActiveSession->delay = ((TCHARA_SCR_INFO*)p_pActiveSession->scrinfo)->delay;	// ��{�f�B���C�l��ݒ肵�Ă���
	}
	else
	{
		AddMessageLog(L"!active:none");
	}

	// ���^�[���Ɉڍs
	SetPhase(GAME_MAIN_PHASE_ACT);
	return bPacket;
}

ptype_session CSyncMain::GetNextActiveChara()
{
	int nMinDelay = MAXINT;
	ptype_session retChara = NULL;
	// �f�B���C�̒Ⴂ�L������T��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		if ( (*it)->connect_state != CONN_STATE_AUTHED) continue;
		// ���͔̂�΂�
		if ( (*it)->obj_state == OBJ_STATE_MAIN_DEAD
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP
		)
			continue;

		if (nMinDelay > (*it)->delay)
		{
			retChara = (ptype_session)(*it);
			nMinDelay = (*it)->delay;
		}
		else if (nMinDelay == (*it)->delay)	// ���f�B���C�l�̂Ƃ��̃^�[������
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
	// �^�[���G���h
	if (active_sess && active_sess->connect_state == CONN_STATE_AUTHED)
	{
		luaParams.Number( active_sess->scrinfo->scr_index).Number(active_sess->obj_no).Number(active_sess->turn_count).Number(active_sess->extdata1).Number(active_sess->extdata2);
		common::scr::CallLuaFunc(g_pLuah, "onTurnEnd_Chara", &luaResults, 0, &luaParams, g_pCriticalSection);
	}
}

void CSyncMain::NotifyTurnStartCharacter(ptype_session  next_sess)
{
	LuaFuncParam luaParams, luaResults;
	// �^�[���X�^�[�g
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

	// �I�u�W�F�N�g�Ȃ�
	if (m_vecObjectNo.empty())	return;

	BOOL bRemoved = FALSE;
	// �e���[�v
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

			// �폜�t���O�m�F
			if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)
			{
				SafeDelete(blt);
				it = m_mapObjects.erase(it);
				bRemoved = TRUE;
				continue;
			}

			if (active_sess)
//			if (active_sess && blt->chr_obj_no == active_sess->obj_no)	// ���L�����̒e�ȊO�ɂ��ʒm
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
			// onTurnEnd�ō폜����Ă��邩�m�F
			if (blt->proc_flg & PROC_FLG_OBJ_REMOVE)
			{
				SafeDelete(blt);
				it = m_mapObjects.erase(it);
				bRemoved = TRUE;
				continue;
			}
		}
	}
	// �폜����������I�u�W�F�N�gNo���X�g�X�V
	if (bRemoved)
		UpdateObjectNo();
}

// 
void CSyncMain::NotifyTurnStartBullet(ptype_session  next_sess)
{
//	BYTE pkt[MAX_PACKET_SIZE];
//	INT packetSize = 0;

	// �I�u�W�F�N�g�Ȃ�
	if (m_vecObjectNo.empty())	return;

	// �e���[�v
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
	// ���񂾃L�����̏ꍇ�͖���
	if (vic_sess && vic_sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY|OBJ_STATE_MAIN_GALLERY_FLG))
		return false;

	// EXP�X�V
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

	// ���[���F�`�[���_���[�W�Ȃ�
	if ( !(m_bytRule & GAME_RULE_TEAM_DAMAGE) )
	{
		// ���`�[���Ȃ�_���[�W�ύX���Ȃ�
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
/// 20100929�L
	if (!sess)
		return false;
/// 20100929�L
	
	// hp 
	{
		LuaFuncParam luaParams, luaResults;
		// ���݂��Ă���Ȃ���擾
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

	// ����ł�Ȃ�X�e�[�^�X�ύX�Ȃ�
	if (sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY|OBJ_STATE_MAIN_GALLERY_FLG))
		return true;

	sess->HP_c = max(0, min(sess->HP_c+hp, sess->HP_m));
	sess->MV_c = max(0, min(sess->MV_c+mv, sess->MV_m));
	sess->delay = max(MIN_DELAY_VALUE,min(sess->delay+delay,MAX_DELAY_VALUE));
	sess->EXP_c = max(0,min(((TCHARA_SCR_INFO*)sess->scrinfo)->sc_info.max_exp, sess->EXP_c+exp));

	// HP�m�F
	if (sess->HP_c <= 0)
	{
		NotifyCharaDead(sess, CHARA_DEAD_KILL);
	}
	else
	{
		// �X�e�[�^�X�X�V�ς�����
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
	// ��X�V�p�P�b�g
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

	// �����^�[�����m�F
	if (m_nTurnLimit && m_nTurnLimit <= m_nTurnCount)
		return TRUE;

	// �l��
	if (g_pPPPhase->GetTeamCount() <= 1)
		ret = IsGameEndOfIndividualMatch();
	else
		ret = IsGameEndOfTeamBattle();

	return ret;
}

BOOL CSyncMain::IsGameEndOfTeamBattle()
{
	// �����m�F
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
//			// ���S�p�P�b�g
//			BYTE pkt[MAX_PACKET_SIZE];
//			WORD packetSize = PacketMaker::MakePacketData_MainInfoDeadChara(PK_USER_MAININFO_CHARA_DEAD_KILL, (*it)->obj_no, pkt);
//			AddPacketAllUser(NULL, pkt, packetSize);
		}
	}

	// �`�[���m�F
	int nTeamCount = g_pPPPhase->GetTeamCount();
	int nLivingTeamCount = 0;
	for (int i=0;i<nTeamCount;i++)
	{
		if (m_nLivingTeamCountTable[i] > 0)
			nLivingTeamCount++;
	}
	// �����`�[������1�ȉ��Ȃ�I���
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
//			// ���S�p�P�b�g
//			BYTE pkt[MAX_PACKET_SIZE];
//			WORD packetSize = PacketMaker::MakePacketData_MainInfoDeadChara(PK_USER_MAININFO_CHARA_DEAD_KILL, (*it)->obj_no, pkt);
//			AddPacketAllUser(NULL, pkt, packetSize);
		}
	}
	// �����Ґ� ��l�ȉ���������
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
	// �l��
	if (g_pPPPhase->GetTeamCount() <= 1)
		SetRankOrderOfIndividualMatch();
	else
		SetRankOrderOfTeamBattle();
}

// �l�탉���N�t��
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
	// �����^�[��������frame_count�ɏ��ʐݒ�
	std::vector<ptype_session>::iterator it;
	while (!vecEntityCharacters.empty())
	{
		nCount = 0;
		nMaxTurn = -1;
		nRestHP = 0;

		it = vecEntityCharacters.begin();

		// �ő�live_count��T��
		while (it != vecEntityCharacters.end())
		{
			if ((*it)->connect_state == CONN_STATE_AUTHED
				&& !((*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY))
				)
			{
				(*it)->live_count+=nLiveInc;
			}
			nMaxTurn = max( (*it)->live_count, nMaxTurn);
			// �������Ă����ꍇ�A�c��HP���L��
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

// �`�[���탉���N�t��
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

	// �c��HP�𑫂����킹��
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

	// HP���c���Ă�l����������ꍇ�i�������t�����ɏI������j
	if (nRestHP > 1)
	{
		// ����HP���v�Z
		for (int i=0;i<MAXUSERNUM;i++)
		{
			if (!pRestHPTable[i].nNum)
				break;
			if (pRestHPTable[i].fHPAvg>0)
				pRestHPTable[i].fHPAvg = pRestHPTable[i].fHPAvg / (float)pRestHPTable[i].nNum;
		}

		// �`�[�����ʂ�ݒ�
		int nNo = 0;
		float fMaxHP = 0.0f;
		float fDelHP = 0.0f;
		int nCount = 0;

		std::vector<ptype_session>::iterator it;
		while (!vecEntityCharacters.empty())
		{
			nCount = 1;
			fMaxHP = 0.0f;
			// �c��HP�̍����`�[��������
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
			// �c��HP���폜
			fDelHP = fMaxHP;
			nNo += nCount;
		}
	}
	else	// �����t����
	{
		int nNo = 0;
		int nLiveInc = vecEntityCharacters.size();

		// �����^�[��������frame_count�ɏ��ʐݒ�
		std::vector<ptype_session>::iterator it;
		while (!vecEntityCharacters.empty())
		{
	//		nCount = 0;
			nRestHP = 0;
			nMaxTurn = -1;
			nMaxTurnTeamNo = -1;
			it = vecEntityCharacters.begin();

			// �ő�live_count��T��
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
					// �������Ă����ꍇ�A�c��HP���L��
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

// ��ԍX�V
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
				// �J�E���g0�ŏ�ԉ����p�P�b�g
				if (!sess->chara_state[i])
				{
					// �ʒu�X�V
					packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
					ret |= AddPacketNoBlindUser(NULL, pkt, packetSize);
					// ��ԍX�V
					packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, i, pkt);
					ret = AddPacketAllUser(NULL, pkt, packetSize);
				}
			}
			break;
		case CHARA_STATE_BLIND_INDEX:
			if (sess->chara_state[i])
			{
				sess->chara_state[i]--;
				// �J�E���g0�ŏ�ԉ����p�P�b�g
				if (!sess->chara_state[i])
				{
					// �S�L�����̈ʒu����ԉ񕜂������[�U�[������
					for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
						it != m_vecCharacters.end();
						it++)
					{
						ptype_session pPosSess = (*it);
						if (sess == pPosSess) continue;	// ���g
						if (pPosSess->connect_state != CONN_STATE_AUTHED)	continue;
						if (!pPosSess->entity) continue;

						// �ϐ킶��Ȃ��A���`�[������Ȃ��l���X�e���X�Ȃ�ʒu�͋����Ȃ�
						if (pPosSess->chara_state[CHARA_STATE_STEALTH_INDEX]
						&& (pPosSess->team_no != GALLERY_TEAM_NO && pPosSess->team_no != sess->team_no))
							continue;
						// �ʒu�X�V
						packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(pPosSess, pkt);
						ret |= AddPacket(sess, pkt, packetSize);
					}
//					// �ʒu�X�V
//					packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
//					ret |= AddPacketNoBlindUser(NULL, pkt, packetSize);
					// ��ԍX�V
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
				// �J�E���g0�ŏ�ԉ����p�P�b�g
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
		case CHARA_STATE_PAIN_INDEX:	// 20111201�ł������^�[������
			break;
		}
	}
	// �p�P�b�g�����܂��Ă�����
	return ret;
}

// 
BOOL CSyncMain::UpdateCharacterState(type_session* sess)
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	// �p���[�A�b�v��Ԃ̉���
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
	// �I�u�W�F�N�g�폜
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
//	// �I�u�W�F�N�g�폜
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
	// �L�����Ƃ̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// �X���[
//		if (!(*it)->entity													// �s��
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// ���S
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// ����
//			continue;
		if (!(*it)->entity													// �s��
		||	(*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)		// ���S
		)
			continue;

		type_session* sess = (*it);
		// �������m�F
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
	// �L�����Ƃ̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// �X���[
//		if (!(*it)->entity													// �s��
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// ���S
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// ����
//			continue;
		if (!(*it)->entity													// �s��
		||	(*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)		// ���S
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
	// �L�����Ƃ̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// �X���[
		if (!(*it)->entity													// �s��
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// ���S
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// ����
			continue;

		type_session* sess = (*it);
		// �������m�F
		if (sess->ay >= (m_pMainStage->GetStageHeight()+((TCHARA_SCR_INFO*) sess->scrinfo)->rec_tex_chr.bottom/2) )
			continue;
		if (common::obj::IsHit(GAME_ITEM_REPAIR_BULLET_BOMB_RANGE, &pos, (*it)))
		{
			float fRepair = (sess->HP_m*GAME_ITEM_REPAIR_BULLET_RATE);
			// �p���[�A�b�v��Ԋm�F
			if (blt_sess && blt_sess->chara_state[CHARA_STATE_POWER_INDEX] == CHARA_STATE_POWERUP_USE)
				fRepair *= CHARA_STATE_POWERUP_FACTOR;

			UpdateHPStatus(sess, (int)fRepair);
			// ���L�����ւ̉񕜂͉��Z���Ȃ�
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
		// �����L�����Ȃ�X�y���p�|�C���g�̑���
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

	if (blt->frame_count <= 1)	// 1�t���[���œ�����ꍇ�͈ړ����Ȃ�
		return FALSE;

	if (blt->ax < 0 || blt->ax > m_pMainStage->GetStageWidth())	// ��ʂ̊O���œ��������ꍇ�ړ����Ȃ�
		return FALSE;

	p_pActiveSession->ax = blt->ax;
	p_pActiveSession->ay = blt->ay;
	common::chr::MoveOnStage(m_pMainStage,p_pActiveSession, &g_mapCharaScrInfo);

	packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(p_pActiveSession, pkt);
	ret = AddPacketNoBlindUser(p_pActiveSession, pkt, packetSize);
	
	return ret;
}

BOOL CSyncMain::BombItemBulletDrain(type_blt* blt)				// �z���e
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	short sCurrnetHP = p_pActiveSession->HP_c;
	ptype_session blt_sess = g_pNetSess->GetSessionFromUserNo(blt->chr_obj_no);

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// �L�����Ƃ̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// �X���[
		if (!(*it)->entity													// �s��
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// ���S
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// ����
			continue;

		type_session* sess = (*it);
		// �������m�F
		if (sess->ay >= (m_pMainStage->GetStageHeight()+((TCHARA_SCR_INFO*) sess->scrinfo)->rec_tex_chr.bottom/2) )
			continue;
		// ���[���F�`�[���_���[�W�Ȃ��Ȃ瓖���蔻�肵�Ȃ�
		if ( !(m_bytRule & GAME_RULE_TEAM_DAMAGE) )
		{
			if (p_pActiveSession->team_no == sess->team_no)
				continue;
		}
		// �����蔻��
		if (common::obj::IsHit(GAME_ITEM_DRAIN_BOMB_RANGE, &pos, (*it)))
		{
			D3DXVECTOR2 vecLen = D3DXVECTOR2((float)(*it)->ax-pos.x, (float)(*it)->ay-pos.y);
			float fLength = D3DXVec2LengthSq(&vecLen);

			float fRange = GAME_ITEM_DRAIN_BOMB_RANGE*GAME_ITEM_DRAIN_BOMB_RANGE;
			// �߂��قǋ����Ȃ�
			float fPower = (fRange>fLength)?max((fRange-fLength)/fRange, 0.5f):0.0f;
			
//			float fPower = (fRange>fLength)?1.0f:0.0f;
			// �p���[�A�b�v��Ԋm�F
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

BOOL CSyncMain::BombItemBulletFetch(type_blt* blt)				// �����񂹒e
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// �L�����Ƃ̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// �X���[
		if (!(*it)->entity													// �s��
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// ���S
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// ����
			continue;

		type_session* sess = (*it);
		if (p_pActiveSession == sess)	continue;	// �����Ȃ�X���[
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

BOOL CSyncMain::BombItemBulletExchange(type_blt* blt)		// �ʒu�ւ��e
{
	BOOL ret = FALSE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	short sMx,sMy;	// ���L�����̈ړ��ʒu
	short sTx = p_pActiveSession->ax;
	short sTy = p_pActiveSession->ay;
	FLOAT fHitLen = (float)(NEED_MAX_TEXTURE_WIDTH*NEED_MAX_TEXTURE_WIDTH);	// ������������
	BOOL bHit = FALSE;		// ������t���O

	D3DXVECTOR2 pos = D3DXVECTOR2(	blt->bx*BLT_POS_FACT_F,blt->by*BLT_POS_FACT_F);
	// �L�����Ƃ̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// �X���[
		if (!(*it)->entity													// �s��
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// ���S
		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// ����
			continue;

		type_session* sess = (*it);
		if (p_pActiveSession == sess)	continue;	// �����Ȃ�X���[
		if (sess->ay >= (m_pMainStage->GetStageHeight()+((TCHARA_SCR_INFO*) sess->scrinfo)->rec_tex_chr.bottom/2) )
			continue;
		if (common::obj::IsHit(GAME_ITEM_EXCHANGE_BOMB_RANGE, &pos, (*it)))
		{
			bHit = TRUE;
			// ��Ԓe�Ƌ߂��L�����̈ʒu���L��
			FLOAT fLen = D3DXVec2LengthSq( &(pos-D3DXVECTOR2((float)(*it)->ax, (float)(*it)->ay)));
			if (fHitLen > fLen)
			{
				fHitLen = fLen;
				sMx = (*it)->ax;
				sMy = (*it)->ay;
			}

			// ���������L�����̈ʒu��ύX
			sess->ax = sTx;
			sess->ay = sTy;

			common::chr::GetDownStage(m_pMainStage, sess);

			// �p�P�b�g�쐬
			packetSize = PacketMaker::MakePacketData_MainInfoMoveChara(sess, pkt);
			ret |= AddPacketAllUser(NULL, pkt, packetSize);
		}
	}

	// ���������̂Ŕ��˃L�����̈ʒu���ړ�
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
	// �L�����Ƃ̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// �X���[
//		if (!(*it)->entity													// �s��
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DEAD		// ���S
//		||	(*it)->obj_state == OBJ_STATE_MAIN_DROP)	// ����
//			continue;
		if (!(*it)->entity													// �s��
		||	(*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)		// ���S
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
	// �L�����Ƃ̓����蔻��
	for (std::vector< type_session* >::iterator it = m_vecCharacters.begin();
		it != m_vecCharacters.end();
		it++)
	{
		// �X���[
		ptype_session sess = (*it);
		if (sess->connect_state != CONN_STATE_AUTHED)	continue;
//		if (!sess->entity)	continue;
		if (!(*it)->entity													// �s��
		||	(*it)->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_EMPTY)		// ���S
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

// Lua����̏����t���O�𗧂Ă�
void CSyncMain::SetObjectLuaFlg(int obj_no, DWORD flg, BOOL on)
{
	std::map<int, type_obj*>::iterator itfind = m_mapObjects.find(obj_no);
	if (itfind == m_mapObjects.end())	return;
	// �폜�t���O�������Ă����牽�����Ȃ�
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

	// �e�̏ꍇ�X�N���v�g�ɃC�x���g�ʒm
	if ((*itfind).second->obj_type & OBJ_TYPE_BLT)
	{
		ptype_blt blt = (type_blt*)((*itfind).second);
		// �X�e�[�W���쐬�����e��
		if (blt->obj_no == (short)STAGE_OBJ_NO)
		{
			LuaFuncParam luaParams, luaResults;
			// ���݂��Ă���Ȃ���擾
			// script,�e�^�C�v,�eObjNo,�e�ʒux,y/�ړ�x,y/extdata/obj_state
			luaParams.Number(m_pStageScrInfo->scr_index).Number(blt->bullet_type).Number(blt->obj_no).Number(blt->ax).Number(blt->ay).Number(blt->vx).Number(blt->vy).Number(blt->extdata1).Number(blt->extdata2).Number((BYTE)(OBJ_TYPE_MASK&type));
			if (!common::scr::CallLuaFunc(g_pLuah, "onUpdateType_StageBullet", &luaResults, 0, &luaParams, g_pCriticalSection))
				return false;
		}
		else
		{
			LuaFuncParam luaParams;
			LuaFuncParam luaResults;
			// script,�e�^�C�v,�eObjNo,�e��������L������ObjNo,�e�ʒux,y/�ړ�x,y/extdata
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

// �Z�b�V�����ؒf�C�x���g
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
		// �ُ��Ԃ��N���A���Ă���
		ZeroMemory(sess->chara_state, sizeof(char)*CHARA_STATE_COUNT);
		pk_head = PK_USER_MAININFO_CHARA_DEAD_DROP;
		break;
	case CHARA_DEAD_KILL:
		sess->obj_state = OBJ_STATE_MAIN_DEAD;
		// �ُ��Ԃ��N���A���Ă���
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
		// ���񎀂񂾏ꍇ�C�x���g���N����
		sess->HP_c = 0;
		// 20110201
		sess->frame_count = 0;

		WCHAR msglog[64];
		WCHAR wsName[MAX_CHARACTER_NAME+1];
		common::session::GetSessionName(sess,wsName);
		SafePrintf(msglog, 64, L"dead:(%d)[%s]", sess->obj_no, wsName);
		AddMessageLog(msglog);

		// �����e�[�u�������炷��
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
		// ��Ԃ��N���A���Ă���(�N���C�A���g�����S�p�P�b�g��M���ɓ��������X�ɍs��
//		ZeroMemory(sess->chara_state,sizeof(char)* CHARA_STATE_COUNT);
	}
	
	UpdateWMCopyData();

	// ���S�p�P�b�g
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
		// �X���[
		if (sess->connect_state != CONN_STATE_AUTHED)	continue;
		if (sess->team_no != GALLERY_TEAM_NO && !sess->entity)	continue;
// 20110428
//		if (!sess->entity)	continue;
		sess->game_ready = 0;
	}
	SetPhase(GAME_MAIN_PHASE_SYNC);
	m_eGameNextPhase = next_phase;
}

// ready���S��1�Ȃ�TRUE��Ԃ�
BOOL CSyncMain::FrameSync()
{
	int nCharactersCount = (int)m_vecCharacters.size();
	if (m_nPhaseReturnIndex >= nCharactersCount)
		return TRUE;

	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	ptype_session sess = m_vecCharacters[m_nPhaseReturnIndex];
	// �X���[
	if (sess->connect_state != CONN_STATE_AUTHED)
// 20110428
//	if (sess->connect_state != CONN_STATE_AUTHED	|| !sess->entity)
	{
		m_nPhaseTimeCounter = 0;
		m_nPhaseReturnIndex++;
		return (m_nPhaseReturnIndex >= nCharactersCount);
	}

	// SYN���M
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
		// �^�C���A�E�g�������[�U�[���L�b�N����
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

// ���̒l�ύX
void CSyncMain::SetWind(int nValue, int nDir)
{
	BYTE pkt[MAX_PACKET_SIZE];
	INT packetSize = 0;

	// �������o�����X���
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
	// �e���[�v
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
				// ���݂��Ă���Ȃ���擾
				// script,�e�^�C�v,�eObjNo,�e�ʒux,y/�ړ�x,y/extdata/wind
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
				// script,�e�^�C�v,�eObjNo,�e��������L������ObjNo,�e�ʒux,y/�ړ�x,y/extdata,wind
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
