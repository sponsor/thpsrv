#include "PacketMaker.h"
#include "ext.h"
#include "common.h"

INT PacketMaker::MakePacketData_SYN(BYTE* msg)
{
	WORD datIndex = 2;		// �ǉ�����f�[�^�̈ʒu
	if (!msg)	return 0;

	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_SYN);
	// 1
	datIndex += SetByteData(&msg[datIndex], 1);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);
	return datIndex;
}

INT PacketMaker::AddEndMarker(int datIndex, BYTE* msg)
{
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);
	return datIndex;
}

//> �p�P�b�g�w�b�_�쐬
// header1			: 1�o�C�g�ڃw�b�_
// header2			: 2�o�C�g�ڃw�b�_
// info_count		: �ȉ��̃I�u�W�F�N�g�̏��(0�̏ꍇ�ǉ����Ȃ�)
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketHeadData(BYTE header1, BYTE header2, int info_count , BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], header1);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], header2);
	// ���
	if (info_count)
		datIndex += SetByteData(&msg[datIndex], (BYTE)info_count);
	return datIndex;
}
//< �p�P�b�g�w�b�_�쐬

//> �F�،��ʃp�P�b�g 
INT PacketMaker::MakePacketData_Authentication(ptype_session sess, BYTE* msg, E_TYPE_AUTH_RESULT auth,int group, int id)
{
	int datIndex = 2;
	if (!sess || !msg)
		return 0;

	datIndex += SetByteData(&msg[datIndex], PK_USER_AUTH);
	// �F�،���
	datIndex += SetByteData(&msg[datIndex], (BYTE)auth);
	// �F�ؐ����̏ꍇ�A����ǉ�
	if (auth == AUTH_RESULT_SUCCESS)
	{
		// ���[�UNo
		datIndex += SetByteData(&msg[datIndex], sess->sess_index);
		// ���(�Q�[��������ԂȂ�)
		datIndex += SetDwordData(&msg[datIndex], sess->obj_state);
		// �}�X�^�[
		datIndex += SetByteData(&msg[datIndex], sess->master);
		// �`�[��No
		datIndex += SetByteData(&msg[datIndex], sess->team_no);
		// �l��
		datIndex += SetByteData(&msg[datIndex], (BYTE)g_nMaxLoginNum);
//		// ���[�U�[��
//		datIndex += SetByteData(&msg[datIndex], (BYTE)authed_user_count);
	}
	else if (auth == AUTH_RESULT_INVALID_HASH)
	{
		// �F�،���AUTH_RESULT_INVALID_HASH
		datIndex += SetByteData(&msg[datIndex], (BYTE)group);
		// �F�،���AUTH_RESULT_INVALID_HASH
		datIndex += SetWordData(&msg[datIndex], (WORD)id);
	}

	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< �F�،��ʃp�P�b�g

INT PacketMaker::MakePacketData_ReqHashList(ptype_session sess, BYTE* msg)
{
	int datIndex = 2;
	if (!sess || !msg)
		return 0;

	datIndex += SetByteData(&msg[datIndex], PK_REQ_HASH);
	int nCharaScrCount = g_mapCharaScrInfo.size();
	datIndex += SetWordData(&msg[datIndex], (WORD)nCharaScrCount);
	for (std::map < int, TCHARA_SCR_INFO >::iterator it = g_mapCharaScrInfo.begin();
		it != g_mapCharaScrInfo.end();
		++it)
	{
		datIndex += SetWordData(&msg[datIndex], (WORD)(*it).second.ID);
	}
	int nStageScrCount = g_mapStageScrInfo.size();
	datIndex += SetWordData(&msg[datIndex], (WORD)nStageScrCount);
	for (std::map < int, TSTAGE_SCR_INFO >::iterator it = g_mapStageScrInfo.begin();
		it != g_mapStageScrInfo.end();
		++it)
	{
		datIndex += SetWordData(&msg[datIndex], (WORD)(*it).second.ID);
	}

	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);
	return datIndex;
}

//> ���[�U�[���p�P�b�g
INT PacketMaker::MakePacketData_RoomInfoIn(ptype_session sess,BYTE* msg)
{
	WORD datIndex = 2;		// �ǉ�����f�[�^�̈ʒu
	if (!msg || !sess)
		return 0;

	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_NEW);
	// �I�u�W�F�N�gNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// �L�����^�C�v
	datIndex += SetByteData(&msg[datIndex], sess->chara_type );
	// �L��������
	datIndex += SetByteData(&msg[datIndex], sess->name_len);
	// �L������
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	// ���(�Q�[��������ԂȂ�)
	datIndex += SetDwordData(&msg[datIndex], sess->obj_state);
	// �}�X�^�[
	datIndex += SetByteData(&msg[datIndex], sess->master);
	// �`�[��No
	datIndex += SetByteData(&msg[datIndex], sess->team_no);
	// �������
	datIndex += SetByteData(&msg[datIndex], sess->game_ready);

	// ���W�lX
	datIndex += SetShortData(&msg[datIndex], sess->lx);
	// ���W�lY
	datIndex += SetShortData(&msg[datIndex], sess->ly);
	// �ړ��lX
	datIndex += SetShortData(&msg[datIndex], sess->vx);
	// �ړ��lY
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	// ����
	datIndex += SetByteData(&msg[datIndex], sess->dir);

	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);
#ifdef _DEBUG
	if ( datIndex > MAX_PACKET_SIZE)
	{
		AddMessageLog(L"PacketMaker::MakePacket_AddObj_CellObjects�p�P�b�g�T�C�Y����");
		return 0;
	}
#endif
	return datIndex;
}
//< ���[�U�[���p�P�b�g

// �����p�P�b�g�w�b�_�쐬(�p�P�b�g�w�b�_�A���[�����w�b�_�A���[�U��)
INT PacketMaker::MakePacketData_RoomInfoInHeader(BYTE nUserCount, BYTE* msg)
{
	WORD datIndex = 2;		// �ǉ�����f�[�^�̈ʒu
	if (!nUserCount || !msg)
		return 0;

	// header			:1
	// roomheader	:1
	// chara_count	:1

	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_IN);
	// ���[�U��
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], nUserCount);

	return datIndex;
}

//> �����p�P�b�g1���[�U���(�L�����^�C�v�Ȃǁc)
//> msg	: ����ǉ�����p�P�b�g�ւ̐擪�|�C���^
INT PacketMaker::MakePacketData_RoomInfoInChara(ptype_session sess, BYTE* msg)
{
	WORD datIndex = 0;		// �ǉ�����f�[�^�̈ʒu
	if (!msg || !sess)
		return 0;
	// chara_type...	:

	// �I�u�W�F�N�gNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// �L�����^�C�v
	datIndex += SetByteData(&msg[datIndex], sess->chara_type );
	// �L��������
	datIndex += SetByteData(&msg[datIndex], sess->name_len);
	// �L������
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	// ���(�Q�[��������ԂȂ�)
	datIndex += SetDwordData(&msg[datIndex], sess->obj_state);
	// �}�X�^�[
	datIndex += SetByteData(&msg[datIndex], sess->master);
	// �`�[��No
	datIndex += SetByteData(&msg[datIndex], sess->team_no);
	// �������
	datIndex += SetByteData(&msg[datIndex], sess->game_ready);

	// ���W�lX
	datIndex += SetShortData(&msg[datIndex], sess->lx);
	// ���W�lY
	datIndex += SetShortData(&msg[datIndex], sess->ly);
	// �ړ��lX
	datIndex += SetShortData(&msg[datIndex], sess->vx);
	// �ړ��lY
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	// ����
	datIndex += SetByteData(&msg[datIndex], sess->dir);

#ifdef _DEBUG
	if ( datIndex > MAX_PACKET_SIZE)
	{
		AddMessageLog(L"PacketMaker::MakePacket_AddObj_CellObjects�p�P�b�g�T�C�Y����");
		return 0;
	}
#endif
	return datIndex;
}
//< �����p�P�b�g1���[�U���(�L�����^�C�v�Ȃǁc)
//< msg	: ����ǉ�����p�P�b�g�ւ̐擪�|�C���^

//> �`���b�g�p�P�b�g
INT PacketMaker::MakePacketData_UserChat(BYTE *in_msg, ptype_session sess,BYTE *msg)
{
	int		datIndex = 2;
	if (!in_msg || !msg || !sess)
		return 0;
	//< IN
	// size			:	2
	// header		:	1
	//	chat_type	:	1
	//	msg_len	:	1
	//	msg			:	msg_len
	//> OUT
	// size			:	2
	// header		:	1
	// obj_no		:	1
	//	name_len	:	1
	//	name		:	name_len
	// user_no		:	1
	//	msg_len	:	1
	//	msg			:	msg_len

	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_CHAT);
	// �`���b�g�w�b�_
	datIndex += SetByteData(&msg[datIndex], in_msg[datIndex]);

	// ���[�UNo (sess == NULL�̓T�[�o���b�Z�[�W)
	if (sess == NULL)
		datIndex += SetByteData(&msg[datIndex], g_nMaxLoginNum);
	else
		datIndex += SetByteData(&msg[datIndex], sess->sess_index);

	// NULL�̏ꍇ�̓T�[�o���b�Z�[�W�쐬
	if (sess != NULL)
	{	// ���[�U�����擾
		// �L��������
		datIndex += SetByteData(&msg[datIndex], sess->name_len);
		// �L������
		datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	}

	// ���b�Z�[�W������
	BYTE	bytMessageLen = min(MAX_CHAT_MSG*sizeof(WCHAR), in_msg[4]);
	if (!bytMessageLen)
		return 0;
	datIndex += SetByteData(&msg[datIndex], bytMessageLen);
	// ���b�Z�[�W
	datIndex += SetMultiByteData(&msg[datIndex], &in_msg[5], bytMessageLen, MAX_CHAT_MSG*sizeof(WCHAR));

	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

#ifdef _DEBUG
	WCHAR	logmsg[MAX_PACKET_SIZE];
	WCHAR	chatmsg[MAX_CHAT_MSG+1];
	WCHAR	username[MAX_USER_NAME+1];
	ZeroMemory(chatmsg, MAX_CHAT_MSG+2);
	ZeroMemory(username, MAX_USER_NAME+2);
	ZeroMemory(logmsg, sizeof(logmsg));
	// �L������
	SetMultiByteData((BYTE*)&username[0], (BYTE*)sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	username[sess->name_len/sizeof(WCHAR)] = NULL;
	// Message
//	_tcsncpy_s(chatmsg, bytMessageLen/sizeof(WCHAR), (WCHAR*)(&in_msg[5]), MAX_CHAT_MSG);
	SetMultiByteData((BYTE*)&chatmsg[0], &in_msg[5], bytMessageLen, MAX_CHAT_MSG*sizeof(WCHAR));
	chatmsg[bytMessageLen/sizeof(WCHAR)] = NULL;
	SafePrintf(logmsg, MAX_PACKET_SIZE, L"PK_USER_CHAT:%s:%s", username, chatmsg);
	AddMessageLog(logmsg, FALSE);
#endif

	return datIndex;
}
//< �`���b�g�p�P�b�g

//> �p�P�b�g�t�b�^�쐬(�G���h�}�[�J�A�p�P�b�g�T�C�Y)
// wSize: �t�b�^���܂߂Ȃ��T�C�Y
// msg	: �p�P�b�g�ւ̃|�C���^
INT PacketMaker::MakePacketData_SetFooter(WORD wSize, BYTE* msg)
{
	int datIndex = wSize;
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[wSize]);
	SetWordData(&msg[0], datIndex);
	return 2;
}
//< �p�P�b�g�t�b�^�쐬(�G���h�}�[�J�A�p�P�b�g�T�C�Y)
// ���[���}�X�^�[���p�P�b�g�쐬
INT PacketMaker::MakePacketData_RoomInfoMaster(int sess_index, BYTE flag, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_MASTER);
	// �}�X�^�[����ύX���郆�[�UNo
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess_index);
	// �}�X�^�[����ύX�l
	datIndex += SetByteData(&msg[datIndex], flag);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> �L�����ړ����p�P�b�g�쐬
// sess			: ���[�U
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_RoomInfoRoomCharaMove(ptype_session sess , BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_MOVE);
	// ���[�UNo
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess->sess_index);
	// ���WX
	datIndex += SetShortData(&msg[datIndex], (WORD)sess->lx);
	// ���WY
	datIndex += SetShortData(&msg[datIndex], (WORD)sess->ly);
	// �ړ��lX
	datIndex += SetShortData(&msg[datIndex], (WORD)sess->vx);
	// �ړ��lY
	datIndex += SetShortData(&msg[datIndex], (WORD)sess->vy);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< �L�����ړ����p�P�b�g�쐬

INT PacketMaker::MakePacketData_RoomInfoGameReady(BYTE sess_index, BYTE game_ready, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_READY);
	// ���[�UNo
	datIndex += SetByteData(&msg[datIndex], sess_index);
	// �Q�[����ԕύX
	datIndex += SetByteData(&msg[datIndex], game_ready);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> �L�����I���p�P�b�g�쐬
// obj_no		: ���[�UNo
// chara_no	: �I���L����No
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_RoomInfoCharaSelect(BYTE sess_index, BYTE chara_no, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_CHARA_SEL);
	// ���[�UNo
	datIndex += SetByteData(&msg[datIndex], sess_index);
	// �L����No
	datIndex += SetByteData(&msg[datIndex], chara_no);
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< �L�����I���p�P�b�g�쐬

//> �ē����p�P�b�g�쐬
INT PacketMaker::MakePacketData_RoomInfoReEnter(ptype_session sess, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_RE_ENTER);
	// ���[�UNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// ���WX
	datIndex += SetShortData(&msg[datIndex], sess->lx);
	// ���WY
	datIndex += SetShortData(&msg[datIndex], sess->ly);
	// �ړ��lX
	datIndex += SetShortData(&msg[datIndex], sess->vx);
	// �ړ��lY
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	// ����
	datIndex += SetByteData(&msg[datIndex], sess->dir);
	// master
	datIndex += SetByteData(&msg[datIndex], sess->master);
	// obj state
	datIndex += SetDwordData(&msg[datIndex], sess->obj_state);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �ē����p�P�b�g�쐬

//> �ؒf�p�P�b�g
// sess	: �ؒf����Z�b�V����
// msg	: out�쐬�p�P�b�g
INT PacketMaker::MakePacketData_UserDiconnect(ptype_session sess, BYTE *msg)
{
	int		datIndex = 2;
	if (!msg || !sess)
		return 0;
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_DISCON);
	// �ؒf�Z�b�V�����̃��[�UNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index );	
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< �ؒf�p�P�b�g


//> �A�C�e���I���p�P�b�g�쐬
// item_index: �A�C�e���C���f�b�N�X
// item_flg		: �A�C�e���t���O
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_RoomInfoItemSelect(int index, DWORD item_flg, BYTE cost, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)
		return 0;
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_ITEM_SEL);
	// �A�C�e���C���f�b�N�X
	datIndex += SetByteData(&msg[datIndex], (BYTE)index );	
	// �A�C�e���t���O
	datIndex += SetDwordData(&msg[datIndex], item_flg );
	// �c��R�X�g
	datIndex += SetByteData(&msg[datIndex], cost );
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> �A�C�e���I���p�P�b�g�쐬
// item_index: �A�C�e���C���f�b�N�X
// item_flg		: �A�C�e���t���O
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoItemSelect(int index, DWORD item_flg, BYTE* msg, BOOL steal)
{
	int		datIndex = 2;
	if (!msg)
		return 0;

	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_ITEM_USE);
	// �A�C�e���C���f�b�N�X
	datIndex += SetByteData(&msg[datIndex], (BYTE)index );	
	// �A�C�e���t���O
	datIndex += SetDwordData(&msg[datIndex], item_flg );	
	// �A�C�e���C���f�b�N�X
	datIndex += SetByteData(&msg[datIndex], (BYTE)steal );	
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}


// �`�[�����p�P�b�g�쐬
// team_count: �`�[����
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_RoomInfoTeamCount(int team_count, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;

	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_TEAM_COUNT);
	// �A�C�e���C���f�b�N�X
	datIndex += SetByteData(&msg[datIndex], (BYTE)team_count );	
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> ���[���ύX�p�P�b�g�쐬
// rule_flg: �`�[����
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_RoomInfoRule(BYTE rule_flg, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;

	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_RULE);
	// �A�C�e���C���f�b�N�X
	datIndex += SetByteData(&msg[datIndex], rule_flg );	
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< ���[���ύX�p�P�b�g�쐬

//> �`�[�����p�P�b�g�쐬
// team_count: �`�[����
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_RoomInfoStageSelect(int stage_index, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;

	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_STAGE_SEL);
	// �A�C�e���C���f�b�N�X
	datIndex += SetByteData(&msg[datIndex], stage_index );	
	// �G���h�}�[�J
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< �`�[�����p�P�b�g�쐬

//> ���[�h���߃p�P�b�g�w�b�_�쐬
// team_count	: �`�[����
// rule				: ���[���t���O
// msg				: out �p�P�b�g
INT PacketMaker::MakePacketHeadData_Load(int teams, BYTE rule, int stage, int chara_count, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;

	//> OUT
	// size			:	2
	// header		:	1
	// items			:	4*MAX_ITEM_COUNT
	// teams		:	1
	// stage			:	1
	// rule			:	1
	// user_count:	users
	//> users
	// obj_no		:	1
	// obj_type	:	1
	// team_no	:	1
	//< users
	//	msg			:	msg_len

	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_LOAD);
	// �`�[����
	datIndex += SetByteData(&msg[datIndex], teams);
	// �X�e�[�W
	datIndex += SetByteData(&msg[datIndex], (BYTE)stage);
	// ���[��
	datIndex += SetByteData(&msg[datIndex], rule);
	// �L������
	datIndex += SetByteData(&msg[datIndex], (BYTE)chara_count);
	return datIndex;
}
//< ���[�h���߃p�P�b�g�w�b�_�쐬

//> ���[�h���߃p�P�b�g�i�L�����j�쐬
// nCharaType	: �L�����^�C�v
// blg				: �e
// msg			: out �p�P�b�g
INT PacketMaker::AddPacketData_Load(int datIndex, ptype_session sess, BYTE* msg)
{
	// ObjNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// ObjType
	datIndex += SetByteData(&msg[datIndex], sess->chara_type);
	// Team No
	datIndex += SetByteData(&msg[datIndex], sess->team_no);

	return datIndex;
}
//< ���[�h���߃p�P�b�g�i�L�����j�쐬

//> ���C���J�n�p�P�b�g�i�L�����j�쐬
// datIndex	: index
// sess			: �L����
// msg			: out �p�P�b�g
INT PacketMaker::AddPacketData_MainStart(int datIndex, ptype_session sess, BYTE* msg)
{
	// ObjIndex
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// x���W
	datIndex += SetShortData(&msg[datIndex], sess->ax);
	// y���W
	datIndex += SetShortData(&msg[datIndex], sess->ay);
	// ����
	datIndex += SetByteData(&msg[datIndex], sess->dir);
	// �p�x
	datIndex += SetShortData(&msg[datIndex], sess->angle);
	// HP
	datIndex += SetShortData(&msg[datIndex], sess->HP_c);
	// MV
	datIndex += SetShortData(&msg[datIndex], sess->MV_c);
	// delay
	datIndex += SetShortData(&msg[datIndex], sess->delay);
	// exp_c
	datIndex += SetShortData(&msg[datIndex], sess->EXP_c);

	return datIndex;
}

//> ���C���J�n�p�P�b�g�i�w�b�_�j�쐬
// msg				: out �p�P�b�g
INT PacketMaker::MakePacketHeadData_MainStart(int chara_count, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// �w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_START);
	// �L������
	datIndex += SetByteData(&msg[datIndex], (BYTE)chara_count);

	return datIndex;
}
//< ���C���J�n�p�P�b�g�i�w�b�_�j�쐬

// �I�u�W�F�N�g�폜�p�P�b�g�쐬
// obj_no	: �I�u�W�F�N�g�ԍ�
// msg		: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoRemoveObject(E_OBJ_RM_TYPE rm_type, ptype_obj obj ,BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_OBJECT_RM);
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], obj->obj_no);
	// ��������
	datIndex += SetByteData(&msg[datIndex], (BYTE)rm_type);
	// ���WX
	datIndex += SetShortData(&msg[datIndex], obj->ax);
	// ���WY
	datIndex += SetShortData(&msg[datIndex], obj->ay);
	// �ړ��lX
	datIndex += SetShortData(&msg[datIndex], obj->vx);
	// �ړ��lY
	datIndex += SetShortData(&msg[datIndex], obj->vy);
	// frame
	datIndex += SetWordData(&msg[datIndex], obj->frame_count);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

//> �L�����ړ��p�P�b�g(�f�[�^)�쐬
// msg				: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoMoveChara(ptype_session sess , BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_CHARA_MV);
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// ���WX
	datIndex += SetShortData(&msg[datIndex], sess->ax);
	// ���WY
	datIndex += SetShortData(&msg[datIndex], sess->ay);
	// ���WX (Y�����Ɉړ��l������A�����Ȃǂ���Ƃ�X��0�ɂ���)
	if (sess->vy == 0)
		datIndex += SetShortData(&msg[datIndex], sess->vx);
	else
		datIndex += SetShortData(&msg[datIndex], 0);
	// ���WY
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	// �����Ă����
	datIndex += SetByteData(&msg[datIndex], (BYTE)((ptype_session)sess)->dir);
	// �L�����̌X��
	datIndex += SetShortData(&msg[datIndex], ((ptype_session)sess)->angle);
	// ���݂̈ړ��c��l
	datIndex += SetShortData(&msg[datIndex], sess->MV_c);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �L�����X�V�p�P�b�g(�f�[�^)�쐬

//> �e�ړ��p�P�b�g(�f�[�^)�쐬
// blg				: �e
// msg			: out �p�P�b�g
INT PacketMaker::AddPacketData_MainInfoMoveBullet(int datIndex, ptype_blt blt , BYTE* msg)
{
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], blt->obj_no);
	// ���WX
	datIndex += SetShortData(&msg[datIndex], blt->ax);
	// ���WY
	datIndex += SetShortData(&msg[datIndex], blt->ay);
	// �ړ��lX
	datIndex += SetShortData(&msg[datIndex], blt->vx);
	// �ړ��lY
	datIndex += SetShortData(&msg[datIndex], blt->vy);
	// �����lX
	datIndex += SetCharData(&msg[datIndex], blt->adx);
	// �����lY
	datIndex += SetCharData(&msg[datIndex], blt->ady);
	// frame
	datIndex += SetWordData(&msg[datIndex], blt->frame_count);
	WCHAR log[32];
	SafePrintf(log, 32, L"MvBlt:#%d(%d,%d,%d,%d)", blt->obj_no, blt->ax, blt->ay, blt->vx,blt->vy);
	AddMessageLog(log);
	return datIndex;
}

//> �I�u�W�F�N�g�ړ��p�P�b�g(�f�[�^)�쐬
// blg				: �e
// msg			: out �p�P�b�g
INT PacketMaker::AddPacketData_MainInfoMoveObject(int datIndex, ptype_obj obj , BYTE* msg)
{
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], obj->obj_no);
	// ���WX
	datIndex += SetShortData(&msg[datIndex], obj->ax);
	// ���WY
	datIndex += SetShortData(&msg[datIndex], obj->ay);
	// �ړ��lX
	datIndex += SetShortData(&msg[datIndex], obj->vx);
	// �ړ��lY
	datIndex += SetShortData(&msg[datIndex], obj->vy);
	// frame
	datIndex += SetWordData(&msg[datIndex], (WORD)obj->frame_count);
	return datIndex;
}
// �e���˃p�P�b�g�쐬
// blt_count	: �쐬����e��
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoBulletShot(ptype_blt blt , BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_BULLET_SHOT);
	// �L�����I�u�W�F�N�g�ԍ�
	datIndex += SetShortData(&msg[datIndex], blt->chr_obj_no);
	// �L����
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->chara_type);
	// �e�̎��
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->bullet_type);
	// �e�ԍ�
	datIndex += SetShortData(&msg[datIndex], blt->obj_no);
	// �����^�C�v
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->proc_type);
	// �e�^�C�v
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->obj_type);
	// ���
	datIndex += SetDwordData(&msg[datIndex], (DWORD)blt->obj_state);
	// �e���WX
	datIndex += SetShortData(&msg[datIndex], blt->ax);
	// �e���WY
	datIndex += SetShortData(&msg[datIndex], blt->ay);
	// �e�ړ�X
	datIndex += SetShortData(&msg[datIndex], blt->vx);
	// �e�ړ�Y
	datIndex += SetShortData(&msg[datIndex], blt->vy);
	// �e����X
	datIndex += SetCharData(&msg[datIndex], blt->adx);
	// �e����Y
	datIndex += SetCharData(&msg[datIndex], blt->ady);
	// extdata1
	datIndex += SetDwordData(&msg[datIndex], blt->extdata1);
	// extdata2
	datIndex += SetDwordData(&msg[datIndex], blt->extdata2);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//> �L�������S�p�P�b�g�쐬
// obj_no	: �I�u�W�F�N�g�ԍ�
// msg		: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoDeadChara(E_TYPE_PACKET_MAININFO_HEADER dead_type, int obj_no, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], dead_type);
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], (short)obj_no);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

//> �A�N�e�B�u���p�P�b�g�쐬
// obj_no		: No
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoActive(ptype_session sess, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_ACTIVE);
	// �A�N�e�B�u���
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// ���݃^�[����
	datIndex += SetWordData(&msg[datIndex], sess->turn_count);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �A�N�e�B�u���p�P�b�g�쐬

//> �^�[���G���h���p�P�b�g�쐬
// act			: �A�N�e�B�u
// delay			: �f�B���C�l
// msg			: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoTurnEnd(ptype_session sess, int wind, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_TURNEND);
	// �f�B���C�l
	datIndex += SetShortData(&msg[datIndex], sess->delay);
	// �I�u�W�F�N�g���
	datIndex += SetDwordData(&msg[datIndex], (DWORD)sess->obj_state);
	// �ړ��l�̏�����
	datIndex += SetShortData(&msg[datIndex], sess->MV_m);
	// ������
	datIndex += SetCharData(&msg[datIndex], (char)wind);
	// �^�[����
	datIndex += SetWordData(&msg[datIndex], (WORD)sess->turn_count);
	// �����^�[����
	datIndex += SetWordData(&msg[datIndex], (WORD)sess->live_count);
	// EXP_c
	datIndex += SetShortData(&msg[datIndex], sess->EXP_c);	
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �A�N�e�B�u���p�P�b�g�쐬

//> �e�����p�P�b�g�쐬
// obj_no
// pos_x	: ���WX
// pos_y	: ���WY
// msg		: out �p�P�b�g
INT PacketMaker::MakePacketHeader_MainInfoBombObject(int scr_id,int blt_type,int blt_chr_no, int blt_no, int pos_x, int pos_y, int erase, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);	//3
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_OBJECT_BOMB);	//4
	// scr_id
	datIndex += SetByteData(&msg[datIndex], (BYTE)scr_id);//5
	// blt_type
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt_type);//6
	// blt_chr_no
	datIndex += SetShortData(&msg[datIndex], (short)blt_chr_no);//8
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)blt_no);//10
	// ���WX
	datIndex += SetShortData(&msg[datIndex], (short)pos_x);//12
	// ���WY
	datIndex += SetShortData(&msg[datIndex], (short)pos_y);//14
	// erase
	datIndex += SetByteData(&msg[datIndex], (BYTE)erase);//15
	// hit chara count 0 clear
	datIndex += SetByteData(&msg[datIndex], 0);
	
	return datIndex;
}
const int c_nhit_chara_count = 15;
INT PacketMaker::AddPacketData_MainInfoBombObject(int datIndex, int hit_chr_no, int power, BYTE* msg)
{
	msg[c_nhit_chara_count]++;	// ��
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], (short)hit_chr_no);
	// power(0-100)
	datIndex += SetByteData(&msg[datIndex], (BYTE)power);
	return datIndex;
}
//< �e�����p�P�b�g�쐬

//> �X�e�[�W�ɉ摜�\��t���p�P�b�g�쐬
INT PacketMaker::MakePacketData_MainInfoPasteImage(int chr_type, int stage_x, int stage_y, int image_x, int image_y, int image_w, int image_h, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_PASTE_IMAGE);
	// chr_type
	datIndex += SetByteData(&msg[datIndex], (BYTE)chr_type);
	// ���WX
	datIndex += SetShortData(&msg[datIndex], (short)stage_x);
	// ���WY
	datIndex += SetShortData(&msg[datIndex], (short)stage_y);
	// �摜�͈�X
	datIndex += SetShortData(&msg[datIndex], (short)image_x);
	// �摜�͈�Y
	datIndex += SetShortData(&msg[datIndex], (short)image_y);
	// �摜�͈�W
	datIndex += SetShortData(&msg[datIndex], (short)image_w);
	// �摜�͈�H
	datIndex += SetShortData(&msg[datIndex], (short)image_h);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �X�e�[�W�ɉ摜�\��t���p�P�b�g�쐬

//> �L�����X�e�[�^�X�X�V�p�P�b�g�쐬
// sess		: �L����
// msg		: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(ptype_session sess , BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_CHARA_UPDATE_STATUS);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// HP
	datIndex += SetShortData(&msg[datIndex], sess->HP_c);
	// MV max
	datIndex += SetShortData(&msg[datIndex], sess->MV_m);
	// MV
	datIndex += SetShortData(&msg[datIndex], sess->MV_c);
	// delay
	datIndex += SetShortData(&msg[datIndex], sess->delay);
	// exp_c
	datIndex += SetShortData(&msg[datIndex], sess->EXP_c);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �L�����X�e�[�^�X�X�V�p�P�b�g�쐬

//> ���C���Q�[���I���p�P�b�g�쐬
// msg		: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoGameEnd(std::vector<ptype_session>* vecCharacters, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;

	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_GAMEEND);

	int nCountDatIndex = datIndex;
	int nUserCount=0;
	datIndex++;

	for (std::vector<ptype_session>::iterator it = vecCharacters->begin();
		it != vecCharacters->end();
		it++)
	{
		// obj_no
		datIndex += SetShortData(&msg[datIndex], (short)(*it)->obj_no);
		// No
		datIndex += SetWordData(&msg[datIndex], (WORD)(*it)->frame_count);
		// live_count
		datIndex += SetWordData(&msg[datIndex], (WORD)(*it)->live_count);
		nUserCount++;
	}

	// UserCount
	SetByteData(&msg[nCountDatIndex], (BYTE)nUserCount);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< ���C���Q�[���I���p�P�b�g�쐬

//> ��ԍX�V�p�P�b�g�쐬
//	state	: ���
// msg		: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoUpdateObjectState(int obj_no, E_TYPE_OBJ_STATE state, WORD frame, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_OBJECT_UPDATE_STATE);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)obj_no);
	// state
	datIndex += SetDwordData(&msg[datIndex], (DWORD)state);
	// frame
	datIndex += SetDwordData(&msg[datIndex], (DWORD)frame);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< ��ԍX�V�p�P�b�g�쐬

//> �L������ԍX�V�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoUpdateCharaState(ptype_session sess, int nStateIndex, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_CHARA_UPDATE_STATE);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// index
	datIndex += SetByteData(&msg[datIndex], (BYTE)nStateIndex);
	// state
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess->chara_state[nStateIndex]);
	// angle
	datIndex += SetShortData(&msg[datIndex], sess->angle);
	// x
	datIndex += SetShortData(&msg[datIndex], sess->ax);
	// y
	datIndex += SetShortData(&msg[datIndex], sess->ay);
	// dir
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess->dir);
	// vx
	datIndex += SetShortData(&msg[datIndex], sess->vx);
	// vy
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �L������ԍX�V�p�P�b�g

//> �I�u�W�F�N�g�^�C�v�X�V�p�P�b�g�쐬
//	type : �^�C�v
// msg		: out �p�P�b�g
INT PacketMaker::MakePacketData_MainInfoUpdateObjectType(type_blt* blt, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_BULLET_UPDATE_TYPE);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], blt->obj_no);
	// type
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->obj_type);
	// x
	datIndex += SetShortData(&msg[datIndex], blt->ax);
	// y
	datIndex += SetShortData(&msg[datIndex], blt->ay);
	// vx
	datIndex += SetShortData(&msg[datIndex], blt->vx);
	// vy
	datIndex += SetShortData(&msg[datIndex], blt->vy);
	// adx
	datIndex += SetCharData(&msg[datIndex], blt->adx);
	// adx
	datIndex += SetCharData(&msg[datIndex], blt->ady);
	// frame
	datIndex += SetWordData(&msg[datIndex], blt->frame_count);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �I�u�W�F�N�g�^�C�v�X�V�p�P�b�g�쐬

//> �L������ԍX�V�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoTrigger(int nCharaIndex, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_BULLET_TRIGGER);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)nCharaIndex);
	// �X�N���v�g/�A�C�e���̃^�C�v
	datIndex += SetByteData(&msg[datIndex], (BYTE)nProcType);
	// ���o�̃^�C�v
	datIndex += SetByteData(&msg[datIndex], (BYTE)nBltType);
	// �p�x
	datIndex += SetShortData(&msg[datIndex], (short)nShotAngle);
	// �p���[
	datIndex += SetShortData(&msg[datIndex], (short)nShotPower);
	// Indicator�p�x
	datIndex += SetShortData(&msg[datIndex], (short)nShotIndicatorAngle);
	// Indicator�p���[
	datIndex += SetShortData(&msg[datIndex], (short)nShotIndicatorPower);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �L������ԍX�V�p�P�b�g

INT PacketMaker::MakePacketData_MainInfoReqTriggerEnd(int nCharaIndex, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_REQ_TRIGGER_END);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)nCharaIndex);

	return PacketMaker::AddEndMarker(datIndex, msg);
}

//> �e���˗v���p�P�b�g
INT PacketMaker::MakePacketData_MainInfoReqShot(int objno, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_REQ_MAININFO_BULLET_SHOT);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)objno);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �e���˗v���p�P�b�g

//> �e���ˋ��ۃp�P�b�g
INT PacketMaker::MakePacketData_MainInfoRejShot(int objno, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_REJ_MAININFO_BULLET_SHOT);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)objno);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �e���ˋ��ۃp�P�b�g

//> ���ʃp�P�b�g
INT PacketMaker::MakePacketData_Confirmed(ptype_session sess, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_CONFIRMED);
	// ���[�UNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);	
	// obj_state
	datIndex += SetDwordData(&msg[datIndex], sess->obj_state);
	// master
	datIndex += SetByteData(&msg[datIndex], sess->master);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< ���ʃp�P�b�g

//> �����ݒ�p�P�b�g
INT PacketMaker::MakePacketData_MainInfoSetWind(int wind, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_WIND);
	// ������
	datIndex += SetCharData(&msg[datIndex], (char)wind);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �����ݒ�p�P�b�g

//> �A�C�e���ǉ��p�P�b�g
INT PacketMaker::MakePacketData_MainInfoAddItem(int obj_no, int slot, DWORD item_flg, BYTE* msg,BOOL bSteal)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_ADD_ITEM);
	// �I�u�W�F�N�g�ԍ�
	datIndex += SetShortData(&msg[datIndex], (short)obj_no);
	// �X���b�g�ԍ�
	datIndex += SetByteData(&msg[datIndex], (BYTE)slot);
	// �A�C�e���t���O
	datIndex += SetDwordData(&msg[datIndex], item_flg);
	// �A�C�e���X�e�B�[��
	datIndex += SetByteData(&msg[datIndex], (BYTE)bSteal);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �A�C�e���ǉ��p�P�b�g

//> ���^�[���p�X����
INT PacketMaker::MakePacketData_MainInfoRejTurnPass(ptype_session sess, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ���C�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_REJ_MAININFO_TURN_PASS);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);

	return PacketMaker::AddEndMarker(datIndex, msg);
}

//< ���^�[���p�X����

// ���[�h�����v��
INT PacketMaker::MakePacketData_LoadReqLoadComplete(ptype_session sess, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_REQ_LOAD_COMPLETE);
	// obj_no
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess->sess_index);
	// frame
	datIndex += SetWordData(&msg[datIndex], (WORD)sess->frame_count);

	return PacketMaker::AddEndMarker(datIndex, msg);
}

//> �n�b�V���l�v���p�P�b�g�쐬
// msg		: out �p�P�b�g
INT PacketMaker::MakePacketData_AuthReqHash(int hash_group, int hash_id , BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_REQ_HASH);
	// group
	datIndex += SetByteData(&msg[datIndex], (BYTE)hash_group);
	// id
	datIndex += SetWordData(&msg[datIndex], (WORD)hash_id);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �L�����X�e�[�^�X�X�V�p�P�b�g�쐬

//> �����^�[�����p�P�b�g
INT PacketMaker::MakePacketData_RoomInfoTurnLimit(int turn, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_RULE_TURN_LIMIT);
	// �^�[����
	datIndex += SetWordData(&msg[datIndex], (short)turn);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< �����^�[�����p�P�b�g

// �������ԃp�P�b�g
INT PacketMaker::MakePacketData_RoomInfoActTimeLimit(int time, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_RULE_ACT_TIME_LIMIT);
	// �^�[����
	datIndex += SetByteData(&msg[datIndex], (BYTE)time);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

// �t�@�C���n�b�V�����M�p�P�b�g
INT PacketMaker::MakePacket_FileInfoSendHash(BOOL bCharaScr, int id, int fileno, char* md5, WCHAR* path, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO_OPEN);
	// �L�����X�N���v�g
	datIndex += SetByteData(&msg[datIndex], (BYTE)bCharaScr);
	// �X�N���v�gID
	datIndex += SetWordData(&msg[datIndex], (WORD)id);
	// FileNO
	datIndex += SetByteData(&msg[datIndex], (BYTE)fileno);
	// md5
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)md5, MD5_LENGTH, MD5_LENGTH);
	WORD wLen = wcslen(path);
	// path len
	datIndex += SetWordData(&msg[datIndex], wLen);
	// path
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)path, sizeof(WCHAR)*wLen, _MAX_PATH*2);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

// �t�@�C�����M�p�P�b�g
INT PacketMaker::MakePacket_FileInfoSendData(BOOL bCharaScr, int id, int fileno ,BOOL eof , int dat_index, int size, BYTE* buffer, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO_SEND);
	// �L�����X�N���v�g
	datIndex += SetByteData(&msg[datIndex], (BYTE)bCharaScr);
	// �X�N���v�gID
	datIndex += SetWordData(&msg[datIndex], (WORD)id);
	// FileNO
	datIndex += SetByteData(&msg[datIndex], (BYTE)fileno);
	// EOF
	datIndex += SetByteData(&msg[datIndex], (BYTE)eof);
	// dat index
	datIndex += SetDwordData(&msg[datIndex], (DWORD)dat_index);
	// dat size
	datIndex += SetDwordData(&msg[datIndex], (DWORD)size);
	// dat buffer
	if (size)
		datIndex += SetMultiByteData(&msg[datIndex], buffer, size, MAX_SEND_BUFFER_SIZE);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

// �t�@�C���f�[�^���M�I���p�P�b�g
INT PacketMaker::MakePacket_FileInfoSendClose(BOOL bCharaScr, int id, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO_CLOSE);
	// �L�����X�N���v�g
	datIndex += SetByteData(&msg[datIndex], (BYTE)bCharaScr);
	// �X�N���v�gID
	datIndex += SetWordData(&msg[datIndex], (WORD)id);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

INT PacketMaker::MakePacketData_TeamRandomHeader(BYTE* msg, BYTE teams)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	�w�b�_�[�R�[�h(�Œ�ʒu)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ���[�����w�b�_
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_TEAM_RAND);
	// �`�[����
	datIndex += SetByteData(&msg[datIndex], teams);

	return datIndex;
}

INT PacketMaker::MakePacketData_TeamRandomAddData(BYTE* msg, std::wstring& wstr)
{
//	const int c_maxMsgSize = MAX_CHAT_MSG - 10;	// �`�[���@:
	INT datIndex = SetWStringData(msg, wstr);
	return datIndex;
}

