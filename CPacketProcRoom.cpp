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

// �V�������[�U�[�𕔉��ɒǉ�
BOOL CPacketProcRoom::AddNewUser(ptype_session sess)
{
	if (sess->connect_state != CONN_STATE_AUTHED)
		return FALSE;

	int sessindex = sess->sess_index;
	// �����ݒ�
	sess->obj_state = OBJ_STATE_ROOM_READY;
	sess->cost = g_nMaxCost;

	// �}�X�^�[�����Ȃ�
	if (!m_pMasterSession)
	{
		sess->master = TRUE;		// �ڑ��Z�b�V�������}�X�^�[�ɐݒ�
		m_pMasterSession = sess;
	}

	// �V�K���[�U�ɓ����ς݂̃��[�U���𑗐M
	BOOL bTellPacket = TellNewUserToEntryUserList(sess);
	// �����ς݂̃��[�U�ɐV�K���[�U�������m
	BOOL bFamPacket = FamiliariseNewUser(sess);
	// �`�[�������𑗐M
	BOOL bTeamCountPacket = TellTeamCount(sess, FALSE);
	// ���[�����𑗐M
	BOOL bRulePacket = SetRule(sess, m_bytRuleFlg);
	// �X�e�[�W�I����񑗐M
	BOOL bStageSelectPacket = SetStageSelect(sess, (BYTE)m_nStageIndex);
	// �����^�[�������M
	BOOL bTurnLimit = SetTurnLimit(sess, m_nTurnLimit);
	// �������ԏ�񑗐M
	BOOL bActTimeLimit = SetActTimeLimit(sess, m_nActTimeLimit);

	// �T�[�o�̃��X�g�{�b�N�X�Ƀ��[�U���ǉ�
	WCHAR	username[MAX_USER_NAME+1];
	common::session::GetSessionName(sess, username);
	// �ǉ�
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
	// �����ݒ�
	sess->obj_state = OBJ_STATE_ROOM_READY;

	// �}�X�^�[�����m�F
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

	// �ʒu�̏�����
//	sess->ax = WIN_WIDTH/2;
//	sess->ay = ROOM_CHARA_BASE_MAX_MOVE_H/2;
	sess->ax = 0;
	sess->ay = 0;
	sess->vx = 0;
	sess->vy = 0;

	WORD packetSize;
	BYTE pktdata[MAX_PACKET_SIZE];
	// �莝���A�C�e���̃R�X�g�m�F
	int nTotalCost = 0;
	for (int i=0;i<GAME_ITEM_STOCK_MAX_COUNT;i++)
	{
		// �A�C�e���������Ă��邩
		if (sess->items[i] & GAME_ITEM_ENABLE_FLG)
		{
			// �A�C�e�������e�[�u������T��
			for (int nTableIndex = 0;nTableIndex < GAME_ITEM_COUNT;nTableIndex++)
			{
				if (c_tblCost[nTableIndex].flg == sess->items[i])
				{
					// �莝�����R�X�g�I�[�o�[
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

	// �����ς݂̃��[�U���𑗐M
	ret |= TellNewUserToEntryUserList(sess);
	// �`�[�������𑗐M
	BOOL bTeamCountPacket = TellTeamCount(sess, FALSE);
	// ���[�����𑗐M
	BOOL bRulePacket = SetRule(sess, m_bytRuleFlg);
	// �X�e�[�W�I����񑗐M
	BOOL bStageSelectPacket = SetStageSelect(sess, (BYTE)m_nStageIndex);
	// �����^�[�������M
	BOOL bTurnLimit = SetTurnLimit(sess, m_nTurnLimit);
	// �������ԏ�񑗐M
	BOOL bActTimeLimit = SetActTimeLimit(sess, m_nActTimeLimit);

	return ret;
}

//> �`�[������m�点��
BOOL CPacketProcRoom::TellTeamCount(ptype_session sess, BOOL bAll, ptype_session ignore_sess)
{
	BYTE		pktdata[MAX_PACKET_SIZE];		// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	BYTE		pktdataReady[MAX_PACKET_SIZE];		// �p�P�b�g�f�[�^
	INT		packetSizeReady = 0;

	// �p�P�b�g�쐬
	packetSize = PacketMaker::MakePacketData_RoomInfoTeamCount(m_nTeamCount, pktdata);
	if (bAll)
	{
		// �S���̏���OK������
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
		// ����OK��ԂȂ�������Ă���
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
//< �`�[������m�点��

//> �V�K���[�U�����m����
BOOL CPacketProcRoom::FamiliariseNewUser(ptype_session sess)
{
	BYTE		pktdata[MAX_PACKET_SIZE];		// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	
	packetSize = PacketMaker::MakePacketData_RoomInfoIn(sess, pktdata);

	// ������(�������܂߂�)�Ƀ��[�U���𑗐M����p�P�b�g���쐬
	if (!AddPacketAllUser(NULL, pktdata, packetSize))
	{
		return FALSE;
	}

	return TRUE;
}
//< �V�K���[�U�����m����

//> �V�K���[�U�ɎQ���ςݑS���[�U���𑗂�
BOOL CPacketProcRoom::TellNewUserToEntryUserList(ptype_session sess)
{
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT packetSize = 0;
	BOOL bSend=FALSE;

//	ptype_session pObjSess = p_pNWSess->GetSessionFromUserNo(obj_no);
	int nUserCount = p_pNWSess->GetConnectUserCount();
	// ��l�ڂ̃��[�U�Ȃ瑊�肪���Ȃ�
	if (nUserCount <= 1)
		return FALSE;
	// ���[�����w�b�_�쐬		// nUserCount���玩��������
	packetSize += PacketMaker::MakePacketData_RoomInfoInHeader((BYTE)nUserCount-1, pktdata);

	//> �����҂ɕ����ɋ���̑S���[�U�[�̏��𑗂�p�P�b�g�쐬
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
			// ���[�U����ǉ�
			packetSize += PacketMaker::MakePacketData_RoomInfoInChara(pRoomSess, &pktdata[packetSize]);
			bSend = TRUE;
		}
	}
	// �p�P�b�g�v���M
	if (bSend)
	{
		// �t�b�^�Z�b�g
		packetSize += PacketMaker::MakePacketData_SetFooter(packetSize, pktdata);
		if (!AddPacket(sess, pktdata, packetSize))
			return FALSE;
	}

	return bSend;
}
//< �V�K���[�U�ɎQ���ςݑS���[�U���𑗂�

//> ��M�p�P�b�g����
BOOL CPacketProcRoom::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;

	// �F�؍ς݈ȊO�������Ȃ�
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
//< ��M�p�P�b�g����

//> �ؒf����
BOOL CPacketProcRoom::DisconnectSession(ptype_session sess)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;

	// �F�؍ς݃��[�U�[�ȊO�͏������Ȃ�
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	sess->entity = 0;
	sess->frame_count = 0;
	sess->game_ready = 0;
	sess->live_count = 0;
	sess->team_no = 0;

	// �ؒf�����Z�b�V�������}�X�^�[�������ꍇ
	if (sess->master)
	{
		ptype_session pNewMasterSess = NULL;
		sess->master = 0;
		// �ŏ��Ɍ��������Z�b�V�����Ƀ}�X�^�[�������ڂ�
		//> �V�����}�X�^�[��T��
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (pSess == sess)	continue;
			pNewMasterSess = pSess;
			pNewMasterSess->game_ready = 0;	// ������Ԃ�����
			pNewMasterSess->master = 1;	// ������Ԃ�����
			break;
		}

		if (pNewMasterSess)
			packetSize = PacketMaker::MakePacketData_RoomInfoMaster(pNewMasterSess->sess_index, TRUE, pktdata);
		if (packetSize)
			ret = AddPacketAllUser(sess, pktdata, packetSize);
		
		// �ύX
		m_pMasterSession = pNewMasterSess;

		if (ret)
		{
			// �`�[�����ݒ���l��ɂ���
			m_nTeamCount = 1;
			ret = TellTeamCount(sess, TRUE, sess);
		}
	}

	// �Z�b�V�������ؒf���邱�ƂŃ`�[�����ݒ�̍ő�l���m�F
	int nAuthedUserCount = p_pNWSess->CalcAuthedUserCount();
	if (!nAuthedUserCount)	nAuthedUserCount = 1;
//> 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
//	int nMaxTeamCount = max(1,(int)((float)(nAuthedUserCount-1) / 2.0f));
//	int nMaxTeamCount = max(1,nAuthedUserCount-1);	// 20101114
	int nMaxTeamCount = min(MAXUSERNUM-1, max(1,nAuthedUserCount-2));
//< 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
	if (m_nTeamCount > nMaxTeamCount)
	{
		m_nTeamCount = nMaxTeamCount;
		ret = TellTeamCount(NULL, TRUE, sess);
	}
	return ret;
}
//< �ؒf����

//> �L�����I���p�P�b�g����
BOOL CPacketProcRoom::SetCharacter(ptype_session sess, BYTE* data)
{
	BOOL ret = FALSE;
	// PK_CMD_ROOM_CHARA_SELx
	// size			: 2	0
	// header		: 1	2
	// chara_type: 1	3
	// footer		: 2	4
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	// ���[�Uno
	BYTE bytSessIndex = sess->sess_index;
	BYTE bytCharaNo =  data[PACKET_ROOM_CHARA_SEL_FLG_INDEX];

	/////////////////////////////////////////////////////////////////////////
	/// ���̃Z�b�V�������Ϗ�����Ԃ��ύX�\���`�F�b�N���K�v
	/////////////////////////////////////////////////////////////////////////
	// ����OK�Ȃ�ύX���󂯕t���Ȃ�
	if (sess->game_ready)
	{
		AddMessageLog(L"E_ProcPK_CMD_ROOM_CHARA_SEL::���ɃQ�[������OK�̂��ߕύX�s��");
		return FALSE;
	}
	else if (m_pMasterSession && m_pMasterSession->game_ready)
	{
		AddMessageLog(L"E_ProcPK_CMD_ROOM_CHARA_SEL::���ɃQ�[���J�n���̂��ߕύX�s��");
		return FALSE;
	}
	if (sess->chara_type == bytCharaNo)
	{
									// �N���C�A���g�Ő؂�ւ����������M���Ȃ��悤�ɂ���͂�
//		return FALSE;		// �p�P�b�g���M����K�v�Ȃ����ǈꉞ����
	}
	else
		sess->chara_type = bytCharaNo;

	// �p�P�b�g�쐬
	packetSize = PacketMaker::MakePacketData_RoomInfoCharaSelect(bytSessIndex, bytCharaNo, pktdata);
	if (packetSize)
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	
	return ret;
}

//> �Q�[�������p�P�b�g����
BOOL CPacketProcRoom::SetGameReady(ptype_session sess, BYTE* data)
{
	BOOL ret = TRUE;
	// PK_CMD_ROOM_READY
	// size			: 2	0
	// header		: 1	2
	// gameready: 1	3
	// footer		: 2	4
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	// ���[�Uno
	BYTE bytSessIndex = sess->sess_index;
	BYTE bytGameReady =  data[PACKET_ROOM_GAME_READY_FLG_INDEX];

	/////////////////////////////////////////////////////////////////////////
	/// ���̃Z�b�V�������Ϗ�����Ԃ��ύX�\���`�F�b�N���K�v
	/////////////////////////////////////////////////////////////////////////
	// �}�X�^�[���������̏ꍇ
	if (sess == m_pMasterSession)
	{
		BOOL bAllReady = TRUE;
		//> �}�X�^�[�ȊO�A�S��������OK���m�F
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->connect_state != CONN_STATE_AUTHED) continue;
			if (pSess == m_pMasterSession)	continue;
			// �ϐ�
//			if ((int)(pSess->lx >= ROOM_GALLERY_WIDTH) && (pSess->lx < (ROOM_WIDTH-ROOM_GALLERY_WIDTH)))
			{
				if (!pSess->game_ready)
				{
					// ����NG��������I��
					bAllReady = FALSE;
					break;
				}
			}
		}
		// �������o���Ă��Ȃ�������A�󂯕t���Ȃ�
		if (!bAllReady)
			ret = FALSE;
	}
	else	// �}�X�^�[���������̃Z�b�V����
	{
		// �}�X�^�[��������������OK�Ȃ珀���ύX���󂯕t���Ȃ�
		if (m_pMasterSession
		&& m_pMasterSession->game_ready)
			ret = FALSE;
	}

	if (!g_bOneClient)
	{
		// �����ύX�s�v
		if (!ret)	return FALSE;
	}
// �N���C�A���g�Ő؂�ւ����������M���Ȃ��悤�ɂ���͂�

	// ���ɓ����ݒ肵�Ă���
	if (sess->game_ready == bytGameReady)
	{
									// �N���C�A���g�Ő؂�ւ����������M���Ȃ��悤�ɂ���͂�
//		return FALSE;		// �p�P�b�g���M����K�v�Ȃ����ǈꉞ����
	}
	else
		sess->game_ready = bytGameReady;

	// ���������̎��A�`�[���ԍ�����
	if (!bytGameReady)
		sess->team_no = 0;

	// �}�X�^�[������OK�Ȃ�`�[������
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
	
	// �ړ����ŏ���OK�ɂȂ�����ړ����~�߂�A����OK�̈ʒu�Ń`�[���ԍ��ݒ�
	if (bytGameReady)
	{
		sess->obj_state = OBJ_STATE_ROOM_READY_OK;
		if (( sess->vx != 0) || ( sess->vy != 0))
		{
			sess->vx = 0;	sess->vy = 0;	// �ړ����~�߂�			
			// �ʒu�̑��M
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
				// �l��̎��̊ϐ�
				if (sess->lx < ROOM_ENTRY_LEFT || sess->lx >= ROOM_ENTRY_RIGHT)
					sess->team_no = GALLERY_TEAM_NO;
			}
		}
	}

	return ret;
}
//< �Q�[�������p�P�b�g����

//> �}�X�^�[�����p�P�b�g����
BOOL CPacketProcRoom::SetRoomMaster(ptype_session sess, BYTE* data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	int			sess_index = sess->sess_index;
	BOOL	bMaster	= data[PACKET_ROOM_MASTER_FLG_INDEX];
	// PK_CMD_ROOM_READY
	// size			: 2	0
	// header		: 1	2
	// master		: 1	3
	// footer		: 2	4
	
	// ���ɓ����ݒ肵�Ă���
	if (sess->master == bMaster)
	{
									// �N���C�A���g�Ő؂�ւ����������M���Ȃ��悤�ɂ���͂�
//		return FALSE;		// �p�P�b�g���M����K�v�Ȃ����ǈꉞ����
	}
	else
		sess->master = bMaster;

	packetSize = PacketMaker::MakePacketData_RoomInfoMaster(sess_index, bMaster, pktdata);
	if (packetSize)
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	
	return ret;
}
//< �}�X�^�[�����p�P�b�g����

//> �ړ����p�P�b�g����
BOOL CPacketProcRoom::SetMove(ptype_session sess, BYTE* data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;

	if (!sess || sess->connect_state != CONN_STATE_AUTHED)	return FALSE;
	int			sessindex = sess->sess_index;
	// PK_CMD_ROOM_MV
	// size			: 2	0
	// header		: 1	2
	// mv_x			: 1	3
	// mv_y			: 1	4
	// footer		: 2	6

	// �ړ��l(-1,0,1)
	short mvecx = 0;
	memcpy(&mvecx, &data[PACKET_ROOM_MV_INDEX], sizeof(short));
	short mvecy = 0;
	memcpy(&mvecy, &data[PACKET_ROOM_MV_INDEX+sizeof(short)], sizeof(short));
	/////////////////////////////////////////////////////////////////////////
	/// ���̃Z�b�V�������Ϗ�����Ԃ��ύX�\���`�F�b�N���K�v
	/////////////////////////////////////////////////////////////////////////
	// ����OK��Ԃ͈ړ��s��
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
		SafePrintf(&logmsg[0], 32, L"�ړ��̊J�n[%d][%d]", sess->ax, sess->ay);
		AddMessageLog(logmsg);
	}
#endif
	sess->obj_state = OBJ_STATE_ROOM_MOVE;
	// �ʒu�̑��M
	packetSize = PacketMaker::MakePacketData_RoomInfoRoomCharaMove(sess, pktdata);
	if (packetSize)
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	
	return ret;
}
//< �ړ����p�P�b�g����

//> �L�����A�C�e���I���p�P�b�g����
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
	dwItemFlg &= (DWORD)GAME_ITEM_ENABLE_FLG;	// �L���͈͊O�̃t���O������

	// �C���f�b�N�X�m�F
	if (nItemIndex < 0 && nItemIndex >= GAME_ITEM_STOCK_MAX_COUNT)
		return FALSE;

	BOOL bOff = FALSE;
	DWORD dwSearchFlg = dwItemFlg;
	if (!dwItemFlg)
	{
		bOff = TRUE;
		dwSearchFlg = sess->items[nItemIndex];
	}
	// �t���O�T��
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
		// �A�C�e���Z�b�g�Ŏc��R�X�g���A�C�e���R�X�g�������ꍇ�͋���
		if (!bOff)
		{
			if ((int)sess->cost < c_tblCost[nTableIndex].cost)
				return FALSE;
		}
	}
	sess->items[nItemIndex] = dwItemFlg;

	// �莝���A�C�e���̃R�X�g�m�F
	int nTotalCost = 0;
	for (int i=0;i<GAME_ITEM_STOCK_MAX_COUNT;i++)
	{
		// �A�C�e���������Ă��邩
		if (sess->items[i] & GAME_ITEM_ENABLE_FLG)
		{
			// �A�C�e�������e�[�u������T��
			for (int nTableIndex = 0;nTableIndex < GAME_ITEM_COUNT;nTableIndex++)
			{
				if (c_tblCost[nTableIndex].flg == sess->items[i])
				{
					// �莝�����R�X�g�I�[�o�[
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

	// ���𑗂�Ԃ�
	if ((int)sess->cost != (g_nMaxCost-nTotalCost))
	{
		sess->cost = (WORD)max(0,(g_nMaxCost-nTotalCost));
		packetSize = PacketMaker::MakePacketData_RoomInfoItemSelect(nItemIndex, dwItemFlg, sess->cost, pktdata);
		if (packetSize)
			ret = AddPacket(sess, pktdata, packetSize);
	}
	
	return ret;
}

//> �`�[�����ύX�p�P�b�g����
BOOL CPacketProcRoom::SetTeamCount(ptype_session sess, BYTE data)
{
	BOOL	ret = FALSE;
	// PK_CMD_ROOM_ITEM_SEL
	// size			: 2	0
	// header		: 1	2
	// team_count: 1	3
	// footer		: 2	5

	// �}�X�^�[�ȊO�ύX����
	if (!sess->master)	return FALSE;
	
//> 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
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
//< 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���

	m_nTeamCount = nTeamCount;	
	ret = TellTeamCount(sess, TRUE);
	return ret;
}
//< �`�[�����ύX�p�P�b�g����

//> ���[���ύX�p�P�b�g����
BOOL CPacketProcRoom::SetRule(ptype_session sess, BYTE data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	// PK_CMD_ROOM_ITEM_SEL
	// size			: 2	0
	// header		: 1	2
	// fule_flg		: 1	3
	// footer		: 2	5
	m_bytRuleFlg = data;
	
	// ���M
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
//< ���[���ύX�p�P�b�g����

//> �X�e�[�W�I���p�P�b�g����
BOOL CPacketProcRoom::SetStageSelect(ptype_session sess, BYTE data)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;
	// PK_CMD_ROOM_STAGE_SEL
	// size			: 2	0
	// header		: 1	2
	// stage_index: 1	3
	// footer		: 2	5
	int nStageIndex = data;
	
	// �v���C���f�b�N�X�l�m�F
	if (nStageIndex < 0 || nStageIndex >= (int)g_mapStageScrInfo.size())
		return FALSE;
	
	m_nStageIndex = nStageIndex;

	// ���M
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
//< �X�e�[�W�I���p�P�b�g����

BOOL CPacketProcRoom::SetTeamNo()
{
	int nUserCount = p_pNWSess->GetConnectUserCount();

	// �l��
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
		// �Q�킪��l�ȉ��Ȃ�FALSE
		if ((nEntryUserCount <= 1 || nEntryUserCount > MAXUSERNUM)&& !g_bOneClient)	return FALSE;
	}
//> 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
/*
	// ���[�U�[���ɒ[�����o��ꍇFALSE
	if (nUserCount % m_nTeamCount)	return FALSE;

	int nMaxSeat = nUserCount / m_nTeamCount;
	int nUserNum = 0;
	int nTeamWidth = (int)(WIN_WIDTH / m_nTeamCount);
	int nTeamSeats[MAX_TEAM_COUNT];
	ZeroMemory(nTeamSeats, sizeof(int)*MAX_TEAM_COUNT);

	// �ڑ��ς݃��[�U�̏�Ԃ�؂�ւ���
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(nUserCount);
		pSess;
		pSess=p_pNWSess->GetSessionNext(nUserCount))
	{
		// �F�؍ς݂�
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		
		// �ʒu�ɂ���ă`�[���ԍ����擾
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

	// �ʒu�ɂ��`�[���ԍ���ݒ�
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// �F�؍ς݂�
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		// �ʒu�ɂ���ă`�[���ԍ����擾
		int nTeamNo = GALLERY_TEAM_NO;
		if ((int)(pSess->lx >= ROOM_GALLERY_WIDTH) && (pSess->lx < (ROOM_WIDTH-ROOM_GALLERY_WIDTH)))
		{
			nTeamNo = min( (int)((pSess->lx-ROOM_GALLERY_WIDTH) / nTeamWidth), m_nTeamCount-1);
			nTeamSeats[nTeamNo]++;
		}
		pSess->team_no = nTeamNo;
	}
	// �l�̋��Ȃ��`�[��������ꍇ�A�`�[���������s
	for (int i=0;i<m_nTeamCount;i++)
	{
		if (!nTeamSeats[i])
			return FALSE;
	}
	
//< 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
	return TRUE;
}

// �߂�
BOOL CPacketProcRoom::Reset(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase)
{
	BOOL ret = Init(pNWSess, pPrevPhase, pPrevPhase->GetStageIndex(), pPrevPhase->GetRuleFlg(), pPrevPhase->GetActTimeLimit());

	//> �}�X�^�[�ȊO�A�S��������OK����
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
//> 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
//	// ���݂̃`�[�����ݒ肪�ő�`�[�������傫���Ȃ�1�ɐݒ�
//	if (m_nTeamCount > nAuthedUserCount/2)
//		m_nTeamCount = 1;
	if (nAuthedUserCount <= 2 || m_nTeamCount > nAuthedUserCount-1)
		m_nTeamCount = 1;
//< 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
	return ret;
}

// �����^�[�����p�P�b�g����
BOOL CPacketProcRoom::SetTurnLimit(ptype_session sess, short data)
{
	ptype_session pSendSess = sess;
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;

	if (!pSendSess)
	{
		if (m_nTurnLimit == data)
			return FALSE;
	}

	if (data)	// �ݒ肠��
	{
		// �Œᐧ���^�[�������m�F
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

// �������ԃp�P�b�g����
BOOL CPacketProcRoom::SetActTimeLimit(ptype_session sess, BYTE data)
{
	ptype_session pSendSess = sess;
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;

	if (!pSendSess && m_nActTimeLimit == data)
		return FALSE;

	if (data < GAME_TURN_ACT_COUNT_MIN || data > GAME_TURN_ACT_COUNT_MAX)	// �l�͈̔͊m�F
		return FALSE;

	// �l�X�V
	m_nActTimeLimit = (int)data;
	// �p�P�b�g���M
	packetSize = PacketMaker::MakePacketData_RoomInfoActTimeLimit(m_nActTimeLimit, pktdata);
	if (pSendSess)
		ret = AddPacket(sess,  pktdata, packetSize);
	else
		ret = AddPacketAllUser(NULL, pktdata, packetSize);
	return ret;
}

// �`�[�������_��
BOOL CPacketProcRoom::SetTeamRandom(ptype_session sess, BYTE data)
{
	ptype_session pSendSess = sess;
	BOOL	ret = FALSE;
	BYTE	pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;

	if (!pSendSess && pSendSess->master)
		return FALSE;

	int nTeams = data;
	if (nTeams <= 0)
		return FALSE;
	std::vector <ptype_session> vecPlayer;
	// �Q���l���v�Z
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// �F�؍ς݂�
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		if ((int)(pSess->lx >= ROOM_GALLERY_WIDTH) && (pSess->lx < (ROOM_WIDTH-ROOM_GALLERY_WIDTH))){
			vecPlayer.push_back(pSess);
		}
	}
	int nPlayer = (int)vecPlayer.size();
	// �Q�[���l����葽���ꍇ��vector�������_���ɕ��ׂ�
//	if (nPlayer > MAXUSERNUM)
		random_shuffle( vecPlayer.begin(), vecPlayer.end());

	if (nPlayer < nTeams)
		nTeams = nPlayer;

	int nPlayersInTeam = (int)((float)nPlayer / (float)nTeams);
	
	// �[��
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
	WCHAR* wstrTeamNo[] = { L"�@",L"�A",L"�B",L"�C",L"�D",L"�E",L"�F",L"�G",L"�H" };
	std::wstring wstr;
	bool bFirst = true;
	for (int i=0;i<nTeams;++i)
	{
		WCHAR header[32];
		wsprintf(header, L"�`�[��%s:", wstrTeamNo[i]);
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
	// �G���h�}�[�J
	packetSize += SetEndMarker(&pktdata[packetSize]);
	SetWordData(&pktdata[0], packetSize);
	ret = AddPacketAllUser(NULL, pktdata, packetSize);
	return ret;
}
