#include "CPacketProcLoad.h"
#include "ext.h"

//> public >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BOOL CPacketProcLoad::Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase)
{
	if (!CPacketProcPhase::Init(pNWSess, pPrevPhase))
		return FALSE;
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 7;

	if (g_bOneClient || m_pMasterSession->game_ready)
	{
		m_eGamePhase = GAME_PHASE_LOAD;
		if (!CheckTeam(GetTeamCount()))
		{
#ifdef	_DEBUG
			MessageBox(NULL, L"�`�[���킯����v���܂���", L"error", MB_OK);
			AddMessageLog(L"�`�[���킯����v���܂���");
			return FALSE;
#endif
		}

		// �ڑ��ς݃��[�U�̏�Ԃ�؂�ւ���
		int cnt=0;
		int nTeamNo = 0;
		int nSearchIndex = 0;
		for(ptype_session pSess=pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=pNWSess->GetSessionNext(&nSearchIndex))
		{
			pSess->scrinfo = NULL;
			// �F�؍ς݂�
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;

			// �؂�ւ�
			pSess->obj_state = OBJ_STATE_LOADING;
			// �ϐ�ȊO
			if (pSess->team_no != GALLERY_TEAM_NO)
			{
				if (m_nTeamCount == 1)
				{
					pSess->team_no = nTeamNo;
					nTeamNo++;
				}
				pSess->entity = 1;	// ����1
				// �L���������_���̊m��
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
				// �X�N���v�g�Ɗ֘A�t��
			}
			else
				pSess->entity = 0;	// ����0
			// �L������Ԃ��[���N���A
			ZeroMemory(pSess->chara_state, sizeof(char)*CHARA_STATE_COUNT);

			// �����^�[����
			pSess->live_count = 0;
			packetSize = PacketMaker::AddPacketData_Load(packetSize, pSess, pktdata);
			cnt++;
		}
		if (cnt > 0)
		{
			// ���[�h����
			PacketMaker::MakePacketHeadData_Load(m_nTeamCount, m_bytRuleFlg, m_nStageIndex, cnt, pktdata);
			packetSize = PacketMaker::AddEndMarker(packetSize, pktdata);
			if (packetSize)
				ret = AddPacketAllUser(NULL, pktdata, packetSize);
		}
	}
	return TRUE;
}
//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//> ��M�p�P�b�g����
BOOL CPacketProcLoad::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;

	// �F�؍ς݈ȊO�������Ȃ�
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	switch ( data[PACKET_HEADER_INDEX] )
	{
	case PK_CMD_LOAD_COMPLETE:		// ���[�h����
		ret = SetLoadComplete(sess);
		break;
	default:
		break;
	}

	return ret;
}
//< ��M�p�P�b�g����

//> �ؒf����
BOOL CPacketProcLoad::DisconnectSession(ptype_session sess)
{
	BOOL ret = CPacketProcPhase::DisconnectSession(sess);
	sess->obj_state = OBJ_STATE_MAIN_DEAD;
	return ret;
}
//< �ؒf����

//> ����m�F
BOOL CPacketProcLoad::CheckTeam(int nTeamCount)
{
	int nUserCount = p_pNWSess->GetConnectUserCount();
	if (nTeamCount <= 1)	return TRUE;
//> 20101105 �[���ł��`�[���ݒ�ł���悤�ɂ���
	int *pTeamSeats;
	pTeamSeats = new int[g_nMaxLoginNum-1];
	ZeroMemory(pTeamSeats, sizeof(int)*(g_nMaxLoginNum-1));
	// �ڑ��ς݃��[�U�̏�Ԃ�؂�ւ���
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// �F�؍ς݂�
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		// �`�[���ԍ������Z
		if (pSess->team_no != GALLERY_TEAM_NO)	// �ϐ�҈ȊO
			pTeamSeats[pSess->team_no]++;
	}

	// �`�[���ɐl�����Ȃ��ꍇ�~�X
	for (int i=0;i<nTeamCount;i++)
	{
		if (!pTeamSeats[i])
		{
			SafeDeleteArray(pTeamSeats);
			return FALSE;
		}
	}
/*
	// ���[�U�[���ɒ[�����o��ꍇFALSE
	if (nUserCount % nTeamCount)	return FALSE;

	int nMaxSeat = nUserCount / nTeamCount;
	int nUserNum = 0;

	// �ڑ��ς݃��[�U�̏�Ԃ�؂�ւ���
	for (int i=0;i<nTeamCount;i++)
	{
		int nSeat = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(nUserCount);
			pSess;
			pSess=p_pNWSess->GetSessionNext(nUserCount))
		{
			// �F�؍ς݂�
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
//< ����m�F

// ���[�h�����p�P�b�g����
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