#include "CPacketProcMain.h"
#include "main.h"
#include "ext.h"

// ������
BOOL CPacketProcMain::Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase)
{
	if (!CPacketProcPhase::Init(pNWSess, pPrevPhase))
		return FALSE;
	
	m_pSaveScrInfo = NULL;

	return TRUE;
}


//> ��M�p�P�b�g����
BOOL CPacketProcMain::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;
	// �F�؍ς݈ȊO�������Ȃ�
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
//< ��M�p�P�b�g����

//> �ؒf����
BOOL CPacketProcMain::DisconnectSession(ptype_session sess)
{
	if (p_pNWSess)
		g_pSyncMain->OnDisconnectSession(sess);

	BOOL ret = CPacketProcPhase::DisconnectSession(sess);
	return ret;
}
//< �ؒf����

BOOL CPacketProcMain::SetMove(ptype_session sess, BYTE* data)
{
	if (g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_ACT)
		return FALSE;
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)
		return FALSE;

	BOOL	ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	// PK_CMD_ROOM_MV
	// size			: 2	0
	// header		: 1	2
	// mv_x			: 1	3	(short)
	// footer		: 2	5

	// �ړ��l(-1,0,1)
	short mvecx = 0;
	memcpy(&mvecx, &data[PACKET_ROOM_MV_INDEX], sizeof(short));

	sess->vx = mvecx;
	sess->vy = 0;

	if (sess->vx != 0)
	{
		E_TYPE_USER_DIRECTION old_dir = sess->dir;
		sess->dir = (sess->vx>0)?USER_DIRECTION_RIGHT:USER_DIRECTION_LEFT;
		// �p�x�̔��]���K�v��
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
	else if (g_pLogFile)	// �ړ��l�ݒ�m�F
	{
		WCHAR msglog[80];
		SafePrintf(msglog, 80, L"ChrStop#%d(px:%d,py%d,vx%d,vy%d)",sess->sess_index, sess->ax, sess->ay, sess->vx, sess->vy);
		AddMessageLog(msglog);
	}
#else
	}
#endif

	// �ړ��s���
	if (sess->chara_state[CHARA_STATE_NOMOVE_INDEX])
		sess->vx = 0;

	// �ʒu�̑��M
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
	BYTE	pkt[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	// PK_CMD_ROOM_MV
	// size			: 2	0
	// header		: 1	2
	// mv_x			: 1	3	(E_TYPE_USER_DIRECTION)
	// footer		: 2	4

	// ����
	E_TYPE_USER_DIRECTION dir = USER_DIRECTION_LEFT;
	memcpy(&dir, &data[PACKET_ROOM_MV_INDEX], sizeof(E_TYPE_USER_DIRECTION));

	E_TYPE_USER_DIRECTION old_dir = sess->dir;
	sess->dir = dir;
	sess->vx = 0;
	sess->vy = 0;
	sess->MV_c = max(0, sess->MV_c-1);

	if (old_dir != sess->dir)
		sess->angle = (sess->angle+180)%360;

	// �ʒu�̑��M
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
	BYTE		pkt[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	// PK_CMD_ROOM_MV
	// size			: 2	0
	// header		: 1	2
	// mv_x			: 1	3	(short)
	// footer		: 2	5

	// �ړ��l(-1,0,1)
	short mvecx = 0;
	memcpy(&mvecx, &data[PACKET_ROOM_MV_INDEX], sizeof(short));

	sess->vx = mvecx;
	sess->vy = 0;

	if (sess->vx != 0)
	{
		E_TYPE_USER_DIRECTION old_dir = sess->dir;
		sess->dir = (sess->vx>0)?USER_DIRECTION_RIGHT:USER_DIRECTION_LEFT;
		// �p�x�̔��]���K�v��
		if (old_dir != sess->dir)
			sess->angle = (sess->angle+180)%360;
#if 1
	}
	else if (g_pLogFile)	// �ړ��l�ݒ�m�F
	{
		WCHAR msglog[80];
		SafePrintf(msglog, 80, L"ChrStop#%d(px:%d,py%d,vx%d,vy%d)",sess->sess_index, sess->ax, sess->ay, sess->vx, sess->vy);
		AddMessageLog(msglog);
	}
#else
	}
#endif

	// �ړ��s���
	if (sess->chara_state[CHARA_STATE_NOMOVE_INDEX])
		sess->vx = 0;

	// �ʒu�̑��M
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

// angle				: �p�x
// power			: ����
// blt_type			: �e�̎��
BOOL CPacketProcMain::SetShot(ptype_session sess, BYTE* data)
{
	AddMessageLog(L"PK_CMD_MAIN_SHOT");
	if ( g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_TRIGGER)
		return FALSE;
	if ( sess->obj_state != OBJ_STATE_MAIN_ACTIVE)
		return FALSE;

	BOOL	ret = FALSE;
	// �p�x
	memcpy(&m_sSaveShotAngle, &data[3], sizeof(short));
	// ����
	m_nSaveShotPower = min(data[5],MAX_SHOT_POWER);
	// �e�̎��
	m_nSaveBltType = data[6];
	// ����
	m_nSaveProcType = data[7];
	// �C���W�P�[�^�[�p�x
	memcpy(&m_sSaveIndicatorAngle, &data[8], sizeof(short));
	
	// �C���W�P�[�^�[�p���[
	m_nSaveIndicatorPower = data[10];
	// �L�����̎��
	m_nSaveCharaType = sess->chara_type;
	//�L�����I�u�W�F�N�g�ԍ�
	m_nSaveCharaObjNo = sess->obj_no;

	m_pSaveScrInfo = (TCHARA_SCR_INFO*)sess->scrinfo;

	// �p���[�A�b�v���g�p��Ԃɐݒ�
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

// ���ˏ���
BOOL CPacketProcMain::ShootingBullet(ptype_session sess, int nFrame)
{
	BOOL ret = FALSE;
	g_pCriticalSection->EnterCriticalSection_Object(L'2');

	switch (m_nSaveProcType)
	{
	case BLT_PROC_TYPE_SCR_CHARA:
		{
			if (common::scr::CallShootingFunc(g_pLuah, sess, m_nSaveProcType, m_nSaveBltType, (int)m_sSaveShotAngle, m_nSaveShotPower, m_pSaveScrInfo, m_nSaveCharaObjNo, nFrame, (int)m_sSaveIndicatorAngle, m_nSaveIndicatorPower, g_pCriticalSection))
				g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOT);	// ���t�F�[�Y
			else if (!nFrame)
				g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOOTING);
		}
		break;
	case BLT_PROC_TYPE_SCR_SPELL:
		{
			// �t���[��0�Ȃ�e���˂ɂ��f�B���C�l����
			if (!nFrame)
			{
				sess->EXP_c = 0;
				WCHAR log[32];
				SafePrintf(log, 32,L"shooting frame:%d", sess->frame_count);
				AddMessageLog(log);
			}
			if (common::scr::CallShootingFunc(g_pLuah, sess, m_nSaveProcType, m_nSaveBltType, (int)m_sSaveShotAngle, m_nSaveShotPower, m_pSaveScrInfo, m_nSaveCharaObjNo, nFrame, (int)m_sSaveIndicatorAngle, m_nSaveIndicatorPower, g_pCriticalSection))
				g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOT);	// ���t�F�[�Y
			else if (!nFrame)
				g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOOTING);
		}
		break;
	case BLT_PROC_TYPE_ITEM:
		{
			POINT pnt;
			// �̂̒��S��荂�߂Ŕ��˂�����
			short sHeadAngle = 90;
			if (sess->dir != USER_DIRECTION_LEFT)
				sHeadAngle = 270;
			double dRad = D3DXToRadian( (sess->angle+sHeadAngle)%360);

			double dBodyOffsetX = cos(dRad) * (CHARA_BODY_RANGE*1.5);
			double dBodyOffsetY = sin(dRad) * (CHARA_BODY_RANGE*1.5);
			// ���W�A������e���������o��
			dRad = D3DXToRadian(m_sSaveShotAngle);
			double dx = cos(dRad);
			double dy = sin(dRad);
			pnt.x = (LONG)(dx * (float)m_nSaveShotPower) * BLT_VEC_FACT_N;
			pnt.y = (LONG)(dy * (float)m_nSaveShotPower) * BLT_VEC_FACT_N;
			// �t���[��0�Ȃ�e���˂ɂ��f�B���C�l�����A�g�p�A�C�e���N���A
			if (!nFrame)
			{
				BYTE		pkt[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
				INT		packetSize = 0;
				// �g�p�A�C�e���N���A�p�P�b�g
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
			blt.adx = BLT_DEFAULT_ADDVEC_X;								// �����l
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
	// �A�N�e�B�u�L�����̂ݎ󂯓����
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)	return FALSE;
// CSyncMain::NotifyTurnEnd�Ɉړ�
//	// �o�ߎ��Ԃɂ��f�B���C�l����
//	int nTimeOfDelay = sess->frame_count?((int)( ((float)sess->frame_count / (float)MAX_PASSAGE_TIME)*(float)MAX_PASSAGE_TIME_DELAY_VALUE)):0;
//	sess->delay += nTimeOfDelay;
	// �t�F�[�Y�ύX
	g_pSyncMain->SetPhase(GAME_MAIN_PHASE_CHECK);
	return FALSE;
}

// �A�C�e���g�p�v��
BOOL CPacketProcMain::OrderItem(ptype_session sess, BYTE* data)
{
	BOOL ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	// �A�N�e�B�u�L�����ȊO
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)	return FALSE;
	// �g�p�A�C�e���C���f�b�N�X
	int nItemIndex = data[3];
	// �A�C�e���t���O
	DWORD dwItemFlg = sess->items[nItemIndex];
	// �t���O�L���͈͊m�F
	if (GAME_ITEM_ENABLE_FLG & dwItemFlg)
	{
		WCHAR logmsg[32];
		SafePrintf(&logmsg[0], 32, L"OrderItem[%d:%x]", nItemIndex, dwItemFlg);
		AddMessageLog(logmsg);

		BOOL bSteal = FALSE;
		ptype_session pStealSess = g_pSyncMain->GetStealSession();
		// �X�e�B�[���g�p���L����������ꍇ�A�X�e�B�[������
		if (pStealSess && (pStealSess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] & GAME_ITEM_STEAL_USING))
		{
			bSteal = TRUE;
			ret = StealItem(pStealSess, dwItemFlg);
		}

		// �A�C�e�����N���A
		sess->items[nItemIndex] = 0x0;
		packetSize = PacketMaker::MakePacketData_MainInfoItemSelect(nItemIndex, dwItemFlg, pktdata, bSteal);
		if (packetSize)
			ret |= AddPacket(sess, pktdata, packetSize);
	
		if (!bSteal)
			ret |= UseItem(sess, dwItemFlg);
	}
	
	return ret;
}

// �A�C�e���𓐂�
BOOL CPacketProcMain::StealItem(ptype_session sess, DWORD item_flg)
{
	BOOL ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	// �󂫃X���b�g��T��
	int nSlot = 0;
	for (nSlot=0;nSlot<GAME_ITEM_STOCK_MAX_COUNT;nSlot++)
	{
		if (!sess->items[nSlot])	// �󂫃X���b�g
			break;
	}

	// �󂫂������ꍇ
	if (nSlot >= GAME_ITEM_STOCK_MAX_COUNT)
		return (BOOL)g_pSyncMain->UpdateCharaStatus(sess->obj_no,0,0,0,GAME_ADD_ITEM_ENOUGH_EXP);
	
	// �A�C�e���ǉ�
	sess->items[nSlot] = item_flg;
	// �p�P�b�g���M
	packetSize = PacketMaker::MakePacketData_MainInfoAddItem(sess->obj_no,nSlot, item_flg, pkt, TRUE);
	ret = AddPacket(sess, pkt, packetSize);
	
	// �X�e�B�[����Ԃ�����
	g_pSyncMain->SetStealSession(NULL);
	sess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] = GAME_ITEM_STEAL_OFF;
	packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_STEAL_INDEX, pkt);
	ret |= AddPacketAllUser(NULL, pkt, packetSize);
	return ret;
}

// �A�C�e���g�p
BOOL CPacketProcMain::UseItem(ptype_session sess, DWORD item_flg)
{
	BOOL ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	switch (item_flg)
	{
	case GAME_ITEM_MOVE_UP:											// �ړ��l����
		sess->delay += /*(((TCHARA_SCR_INFO*)sess->scrinfo)->delay-GAME_CHARA_START_DELAY_RND_MIN)+*/GAME_ITEM_MOVE_UP_DELAY;
		sess->MV_m += GAME_ITEM_MOVE_UP_VALUE;
		sess->MV_c += GAME_ITEM_MOVE_UP_VALUE;
		WCHAR logmsg[32];
		SafePrintf(&logmsg[0], 32, L"MvUp[%d:%d]", sess->MV_m, sess->MV_c);
		AddMessageLog(logmsg);
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
		ret = AddPacket(sess, pkt, packetSize);
		break;
	case GAME_ITEM_DOUBLE_SHOT:									// �A��
		sess->delay += GAME_ITEM_DOUBLE_DELAY;
		sess->chara_state[CHARA_STATE_DOUBLE_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_DOUBLE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_STEALTH:											// �X�e���X
		sess->delay += GAME_ITEM_STEALTH_DELAY;
		sess->chara_state[CHARA_STATE_STEALTH_INDEX] = GAME_ITEM_STEALTH_TURN_COUNT;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_STEALTH_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		// 20101105
//		g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TURNEND);	// �^�[���I��
		break;
	case GAME_ITEM_REVERSE:										// �t���A�C�e�����ˑO���
		sess->delay += GAME_ITEM_REVERSE_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_REVERSE_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_REVERSE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_BLIND:											// �Â��Ȃ�e���ˑO���
		sess->delay += GAME_ITEM_BLIND_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_BLIND_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_BLIND_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_SHIELD:											// �V�[���h
		sess->delay += /*(((TCHARA_SCR_INFO*)sess->scrinfo)->delay-GAME_CHARA_START_DELAY_RND_MIN)+*/GAME_ITEM_SHIELD_DELAY;
		sess->chara_state[CHARA_STATE_SHIELD_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_SHIELD_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_POWER:													// �p���[�A�b�v
		sess->delay += GAME_ITEM_POWER_DELAY;
		sess->chara_state[CHARA_STATE_POWER_INDEX] = CHARA_STATE_POWERUP_ON;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_POWER_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_NOANGLE:													// �p�x�ύX�s��
		sess->delay += GAME_ITEM_NOANGLE_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_NOANGLE_INDEX] = GAME_ITEM_NOANGLE_TURN_COUNT;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_NOANGLE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_NOMOVE:													// �ړ��s��
		sess->delay += GAME_ITEM_NOMOVE_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_NOMOVE_INDEX] = GAME_ITEM_NOMOVE_TURN_COUNT;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_NOMOVE_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_REPAIR:														// ��
		sess->delay += /*(((TCHARA_SCR_INFO*)sess->scrinfo)->delay-GAME_CHARA_START_DELAY_RND_MIN)+*/GAME_ITEM_REPAIR_DELAY;
		UpdateHPStatus(sess, (int)(sess->HP_m * GAME_ITEM_REPAIR_RATE));
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_REPAIR_BULLET:										// �񕜒e
		sess->delay += GAME_ITEM_REPAIR_BULLET_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_REPAIRBLT_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_REPAIRBLT_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_TELEPORT:													// �ړ��e
		sess->delay += GAME_ITEM_TELEPORT_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_TELEPORT_INDEX] = 0x01;
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharaState(sess, CHARA_STATE_ITEM_TELEPORT_INDEX, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		break;
	case GAME_ITEM_REPAIR_BIG:												// ��(�啝)
		sess->delay += GAME_ITEM_REPAIR_BIG_DELAY;
		UpdateHPStatus(sess, (int)(sess->HP_m * GAME_ITEM_REPAIR_BIG_RATE));
		packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(sess, pkt);
		ret = AddPacketAllUser(NULL, pkt, packetSize);
		// �ړ����̏ꍇ�A�ړ��l��0�ɂ��Ĉʒu�̑��M
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
		g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TURNEND);	// �^�[���I��
		break;
	case GAME_ITEM_REPAIR_TEAM:											// ��(�`�[��)
		{
			sess->delay += GAME_ITEM_REPAIR_TEAM_DELAY;
			int nSearchIndex = 0;
			for(ptype_session pTeamSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
				pTeamSess;
				pTeamSess=p_pNWSess->GetSessionNext(&nSearchIndex))
			{
				if (pTeamSess->connect_state != CONN_STATE_AUTHED
				|| !pTeamSess->entity															// ���݂��Ȃ��Z�b�V����
				|| pTeamSess->obj_state == OBJ_STATE_MAIN_DEAD			// ����ł�
				|| pTeamSess->obj_state == OBJ_STATE_MAIN_DROP			// ����ł�
				|| pTeamSess->team_no != sess->team_no)							// �ʃ`�[��
					continue;
				UpdateHPStatus(pTeamSess, (int)(pTeamSess->HP_m * GAME_ITEM_REPAIR_TEAM_RATE));
				packetSize = PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(pTeamSess, pkt);
				ret |= AddPacketAllUser(NULL, pkt, packetSize);
			}
			g_pSyncMain->SetCheckPhase(GAME_MAIN_PHASE_TURNEND);	// �^�[���I��
			break;
		}
	case GAME_ITEM_DRAIN:													// �z���e
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
	case GAME_ITEM_WIND_CHANGE:											// �������ύX
		sess->delay += (short)GAME_ITEM_WIND_CHANGE_DELAY;
		g_pSyncMain->SetWind(-g_pSyncMain->GetWind(), -g_pSyncMain->GetWindDirection());
//		WCHAR logmsg[32];
		SafePrintf(&logmsg[0], 32, L"WindChange[%d](%d)", g_pSyncMain->GetWind(),g_pSyncMain->GetWindDirection());
		AddMessageLog(logmsg);
		break;
	case GAME_ITEM_STEAL:
		sess->delay += (short)GAME_ITEM_STEAL_DELAY;
		sess->chara_state[CHARA_STATE_ITEM_STEAL_INDEX] = GAME_ITEM_STEAL_SET;
		// SyncMain�ɐݒ�
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

	// �t�F�[�Y�m�F	20101223
	if (g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_ACT)	return FALSE;

	g_pCriticalSection->EnterCriticalSection_Session(L'7');
	// �F�؍ς݃A�N�e�B�u�L�����̂ݎ󂯓����
	if (sess->connect_state != CONN_STATE_AUTHED)
	{
		g_pCriticalSection->LeaveCriticalSection_Session();
		return FALSE;
	}

	if ( (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)							// �A�N�e�B�u�ȃZ�b�V������
	|| (g_pSyncMain->GetPhase() != GAME_MAIN_PHASE_ACT)				// �A�N�e�B�u�ȃt�F�[�Y��
	|| (!g_pSyncMain->IsShotPowerWorking())											//	�V���b�g�p���[�`���[�W����
	|| (g_pSyncMain->GetPhaseTimeCount() > GAME_MAIN_PHASE_TIME_SHOTPOWER)	// ���ԊO�̗v��
	)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoRejShot(sess->obj_no, pkt);
		if (packetSize)
			ret = AddPacket(sess, pkt, packetSize);
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	}

	int nIndex = 3;
	// �L�����ԍ�
	short nCharaIndex;
	memcpy(&nCharaIndex, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// �X�N���v�g/�A�C�e���̃^�C�v
	int nProcType = data[nIndex];
	nIndex++;
	// ���o�^�C�v
	int nBltType = data[nIndex];
	nIndex++;
	// �p�x
	short sShotAngle = 0;
	memcpy(&sShotAngle, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// �p���[
	short sShotPower = 0;
	memcpy(&sShotPower, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// indicator�p�x
	short sShotIndicatorAngle = 0;
	memcpy(&sShotIndicatorAngle, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// indicator�p���[
	short sShotIndicatorPower = 0;
	memcpy(&sShotIndicatorPower, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);

	// �g���K�[����
	BOOL bRejTrigger = FALSE;
	// �����^�C�v
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

	// �ڑ��ς݃��[�U�̏�Ԃ�؂�ւ���
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// �F�؍ς݂�
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		if (!pSess->entity)	continue;

		// �҂�
		pSess->game_ready = 0;
	}

	// �ړ����̏ꍇ�A�ړ��l��0�ɂ��Ĉʒu�̑��M
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
	// �t�F�[�Y�ύX
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
		return true;	// ���ˏI��
	}

	bool ret = common::scr::CallShootingFunc(g_pLuah, sess, nProcType, nBltType, nAngle, nPower, &((*itfind).second), nChrObjNo, nFrame, indicator_angle, indicator_short, g_pCriticalSection)?true:false;
	if (ret)
	{
		sess->delay += (*itfind).second.blt_info->blt_delay;
		g_pSyncMain->SetPhase(GAME_MAIN_PHASE_SHOT);	// ���t�F�[�Y
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


// �A�˂��邩�m�F
BOOL CPacketProcMain::IsDoubleShot(ptype_session sess)
{
	BOOL ret = TRUE;

	// �A�˂ɏ�Ԑݒ肳��Ă��邩
	if ( !sess || !sess->chara_state[CHARA_STATE_DOUBLE_INDEX])
		return FALSE;
	
	// �ʃL�����������̏ꍇFALSE��Ԃ�
	if (sess->obj_no != m_nSaveCharaObjNo)
		return FALSE;

	switch (m_nSaveProcType)
	{
	case BLT_PROC_TYPE_SCR_CHARA:
	case BLT_PROC_TYPE_ITEM:
		break;
	case BLT_PROC_TYPE_SCR_SPELL:	// �X�y���͘A�˕s��
	default:
		ret = FALSE;
		break;
	}

	return ret;
}

// �p�P�b�g�F���^�[���p�X
BOOL CPacketProcMain::SetTurnPass(ptype_session sess,BYTE* data)
{
	BOOL ret = FALSE;
	BYTE		pkt[MAX_PACKET_SIZE];
	INT		packetSize = 0;

	g_pCriticalSection->EnterCriticalSection_Session(L'8');
	// ���F�؎󂯓���Ȃ�
	if (sess->connect_state != CONN_STATE_AUTHED)
	{
		g_pCriticalSection->LeaveCriticalSection_Session();
		return FALSE;
	}
	// ��A�N�e�B�u�A�������͎󂯓���Ȃ�
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE || sess->vy != 0)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoRejTurnPass(sess, pkt);
		if (packetSize)
			ret = AddPacket(sess, pkt, packetSize);
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	}

	// �ړ����̏ꍇ�A�ړ��l��0�ɂ��Ĉʒu�̑��M
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
	// �p�X�����ꍇ�͎��Ԍo�߂�0�ɂ���
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

	// �F�؍ς݃A�N�e�B�u�L�����̂ݎ󂯓����
	if (sess->connect_state != CONN_STATE_AUTHED)	return FALSE;
	if (sess->obj_state != OBJ_STATE_MAIN_ACTIVE)
	{
		packetSize = PacketMaker::MakePacketData_MainInfoRejShot(sess->obj_no, pkt);
		if (packetSize)
			ret = AddPacket(sess, pkt, packetSize);
		return ret;
	}

	int nIndex = 3;
	// �L�����ԍ�
	short nCharaIndex;
	memcpy(&nCharaIndex, &data[nIndex], sizeof(short));
	nIndex += sizeof(short);
	// �X�N���v�g/�A�C�e���̃^�C�v
	int nProcType = data[nIndex];
	nIndex++;
	// ���o�^�C�v
	int nBltType = data[nIndex];
	nIndex++;
	// �p�x
	short sShotAngle = 0;
	memcpy(&sShotAngle, &data[nIndex], sizeof(short));
	nIndex+= sizeof(short);

	BOOL bRejTrigger = FALSE;

	if (sess->vy != 0)
		bRejTrigger = TRUE;
	else
	{
		// �����^�C�v
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

	// �ړ����̏ꍇ�A�ړ��l��0�ɂ��Ĉʒu�̑��M
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
	
	// �ۑ�
	m_nSaveCharaObjNo = nCharaIndex;
	m_nSaveBltType = nBltType;
	m_nSaveProcType = nProcType;
	m_sSaveShotAngle = sShotAngle;
	m_nSaveShotPower = 0;
	m_pSaveScrInfo = (TCHARA_SCR_INFO*)sess->scrinfo;

	// �t�F�[�Y�ύX
	g_pSyncMain->SetShotPowerStart(sess, nBltType);

	return ret;
}
