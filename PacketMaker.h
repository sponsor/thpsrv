#ifndef H_PACKET_MAKER___
#define H_PACKET_MAKER___

#include "windows.h"
#include <cassert>
#include "../include/define.h"
#include "../include/types.h"
#include <vector>
#include "util.h"

namespace PacketMaker
{

	// �F�،��ʃp�P�b�g
	// sess	: �Z�b�V����
	// msg	: out�쐬�p�P�b�g
	// bauth: �F�،��� 0/0�ȊO :����/�G���[�ԍ�
	INT MakePacketData_Authentication(ptype_session sess, BYTE* msg, E_TYPE_AUTH_RESULT auth,int group = 0, int id = 0);

	INT MakePacketData_ReqHashList(ptype_session sess, BYTE* msg);

	// �����p�P�b�g�쐬(1���[�U���p)
	INT MakePacketData_RoomInfoIn(ptype_session sess, BYTE* msg);
	// �����p�P�b�g�w�b�_�쐬(�p�P�b�g�w�b�_�A���[�����w�b�_�A�L������)
	// msg	: �p�P�b�g�ւ̃|�C���^(�擪�̃T�C�Y���͓����Ŕ�΂�)
	INT MakePacketData_RoomInfoInHeader(BYTE nUserCount, BYTE* msg);
	// �����p�P�b�g1���[�U���(�L�����^�C�v�Ȃǁc)
	// msg	: ����ǉ�����p�P�b�g�ւ̐擪�|�C���^
	INT MakePacketData_RoomInfoInChara(ptype_session sess, BYTE* msg);
	// ���[���}�X�^�[���p�P�b�g�쐬
	INT MakePacketData_RoomInfoMaster(int obj_no, BYTE flag, BYTE* msg);

	// �Q�[��������ԕύX�p�P�b�g�쐬
	// obj_no		: ���[�UNo
	// game_ready	: ����OK/NG
	// msg			: out �p�P�b�g
	INT MakePacketData_RoomInfoGameReady(BYTE obj_no, BYTE game_ready, BYTE* msg);

	// �L�����I���p�P�b�g�쐬
	// obj_no		: ���[�UNo
	// chara_no	: �I���L����No
	// msg			: out �p�P�b�g
	INT MakePacketData_RoomInfoCharaSelect(BYTE obj_no, BYTE chara_no, BYTE* msg);

	// �L�����ړ����p�P�b�g�쐬
	// sess			: ���[�U
	// msg			: out �p�P�b�g
	INT MakePacketData_RoomInfoRoomCharaMove(ptype_session sess , BYTE* msg);

	// �p�P�b�g�t�b�^�쐬(�G���h�}�[�J�A�p�P�b�g�T�C�Y)
	// wSize: �t�b�^���܂߂Ȃ��T�C�Y
	// msg	: �p�P�b�g�ւ̃|�C���^
	INT MakePacketData_SetFooter(WORD wSize, BYTE* msg);

	// �`���b�g�p�P�b�g
	// in_msg	: ��M�����`���b�g�p�P�b�g
	// sess		: ��M���Z�b�V����
	// msg		: out�쐬�p�P�b�g
	INT MakePacketData_UserChat(BYTE *in_msg, ptype_session sess,BYTE *msg);

	// �ؒf�p�P�b�g
	// sess	: �ؒf����Z�b�V����
	// msg	: out�쐬�p�P�b�g
	INT MakePacketData_UserDiconnect(ptype_session sess, BYTE *msg);

	// �A�C�e���I���p�P�b�g�쐬
	// item_index: �A�C�e���C���f�b�N�X
	// item_flg		: �A�C�e���t���O
	// msg			: out �p�P�b�g
	INT MakePacketData_RoomInfoItemSelect(int index, DWORD item_flg, WORD cost, BYTE* msg);

	// �`�[�����p�P�b�g�쐬
	// team_count: �`�[����
	// msg			: out �p�P�b�g
	INT MakePacketData_RoomInfoTeamCount(int team_count, BYTE* msg);

	// ���[���ύX�p�P�b�g�쐬
	// rule_flg: �`�[����
	// msg			: out �p�P�b�g
	INT MakePacketData_RoomInfoRule(BYTE rule_flg, BYTE* msg);

	// ���[���ύX�p�P�b�g�쐬
	// stage_index: �X�e�[�W�C���f�b�N�X
	// msg			: out �p�P�b�g
	INT MakePacketData_RoomInfoStageSelect(int stage_index, BYTE* msg);

	INT MakePacketData_TeamRandomHeader(BYTE* msg, BYTE teams);
	INT MakePacketData_TeamRandomAddData(BYTE* msg, std::wstring& wstr);

	// ���[�h���߃p�P�b�g�쐬
	// team_count	: �`�[����
	// rule				: ���[���t���O
	// msg				: out �p�P�b�g
	INT MakePacketData_Load(int teams, BYTE rule, int stage, BYTE* msg);

	// ���[�h���߃p�P�b�g�i�w�b�_�j�쐬
	// team_count	: �`�[����
	// rule				: ���[���t���O
	// msg				: out �p�P�b�g
	INT MakePacketHeadData_Load(int teams, BYTE rule, int stage, int chara_count, BYTE* msg);
	// ���[�h���߃p�P�b�g�i�L�����j�쐬
	// nCharaType	: �L�����^�C�v
	// blg				: �e
	// msg			: out �p�P�b�g
	INT AddPacketData_Load(int datIndex, ptype_session sess, BYTE* msg);

	// ���C���J�n�p�P�b�g�쐬
	// pNWSess		: �ڑ����
	// msg				: out �p�P�b�g
	INT MakePacketData_MainStart(BYTE* msg);
	// ���C���J�n�p�P�b�g�i�w�b�_�j�쐬
	// team_count	: �`�[����
	// rule				: ���[���t���O
	// msg				: out �p�P�b�g
	INT MakePacketHeadData_MainStart(int chara_count, BYTE* msg);
	// ���C���J�n�p�P�b�g�i�L�����j�쐬
	// datIndex	: index
	// sess			: �L����
	// msg			: out �p�P�b�g
	INT AddPacketData_MainStart(int datIndex, ptype_session sess, BYTE* msg);

	// �e���˃p�P�b�g�쐬
	// blt_count	: �쐬����e��
	// msg			: out �p�P�b�g
	INT MakePacketData_MainInfoBulletShot(ptype_blt blt , BYTE* msg);

	// �L�����ړ��p�P�b�g�쐬
	// blt_count	: �X�V����e��
	// msg			: out �p�P�b�g
	INT MakePacketData_MainInfoMoveChara(ptype_session sess , BYTE* msg);

	// �A�N�e�B�u���p�P�b�g�쐬
	// obj_no		: No
	// msg			: out �p�P�b�g
	INT MakePacketData_MainInfoActive(ptype_session sess, BYTE* msg);

	// �^�[���G���h���p�P�b�g�쐬
	// sess			: �L����
	// msg			: out �p�P�b�g
	INT MakePacketData_MainInfoTurnEnd(ptype_session sess, int wind, BYTE* msg);

	// �I�u�W�F�N�g�폜�p�P�b�g�쐬
	// obj_no	: �I�u�W�F�N�g�ԍ�
	// msg		: out �p�P�b�g
	INT MakePacketData_MainInfoRemoveObject(E_OBJ_RM_TYPE rm_type, ptype_obj obj , BYTE* msg);

	// �L�������S�p�P�b�g�쐬
	// obj_no	: �I�u�W�F�N�g�ԍ�
	// msg		: out �p�P�b�g
	INT MakePacketData_MainInfoDeadChara(E_TYPE_PACKET_MAININFO_HEADER dead_type, int obj_no, BYTE* msg);

	// �I�u�W�F�N�g�ړ��p�P�b�g(�w�b�_)�쐬
	// blt_count	: �X�V����e��
	// msg			: out �p�P�b�g
	//	INT MakePacketHeadData_MainInfoMoveObject(int obj_count , BYTE* msg);
	// �I�u�W�F�N�g�ړ��p�P�b�g(�f�[�^)�쐬
	// nCharaType	: �L�����^�C�v
	// blg				: �e
	// msg			: out �p�P�b�g
	INT AddPacketData_MainInfoMoveObject(int datIndex, ptype_obj obj , BYTE* msg);
	INT AddPacketData_MainInfoMoveBullet(int datIndex, ptype_blt blt , BYTE* msg);

	// �e�����p�P�b�g�쐬
	INT MakePacketHeader_MainInfoBombObject(int scr_id,int blt_type,int blt_chr_no, int blt_no, int pos_x, int pos_y, int erase, BYTE* msg);
	INT AddPacketData_MainInfoBombObject(int datIndex, int hit_chr_no, int power, BYTE* msg);

	// �X�e�[�W�ɉ摜�\��t���p�P�b�g�쐬
	INT MakePacketData_MainInfoPasteImage(int chr_type, int stage_x, int stage_y, int image_x, int image_y, int image_w, int image_h, BYTE* msg);

	// ��ԍX�V�p�P�b�g�쐬
	//	state	: ���
	// msg		: out �p�P�b�g
	INT MakePacketData_MainInfoUpdateObjectState(int obj_no, E_TYPE_OBJ_STATE state, WORD frame, BYTE* msg);

	// �I�u�W�F�N�g�^�C�v�X�V�p�P�b�g�쐬
	//	type_obj : object
	// msg		: out �p�P�b�g
	INT MakePacketData_MainInfoUpdateObjectType(type_blt* blt, BYTE* msg);

	// �L�����X�e�[�^�X�X�V�p�P�b�g�쐬
	// sess		: �L����
	// msg		: out �p�P�b�g
	INT MakePacketData_MainInfoUpdateCharacterStatus(ptype_session sess , BYTE* msg);

	// ���C���Q�[���I���p�P�b�g�쐬
	// sess		: �L����
	// msg		: out �p�P�b�g
	INT MakePacketData_MainInfoGameEnd(std::vector<ptype_session>* vecCharacters, BYTE* msg);

	// �L������ԍX�V�p�P�b�g
	INT MakePacketData_MainInfoUpdateCharaState(ptype_session sess, int nStateIndex, BYTE* msg);

	INT MakePacketData_MainInfoItemSelect(int index, DWORD item_flg, BYTE* msg, BOOL steal);

	// �g���K�[�p�P�b�g
	INT MakePacketData_MainInfoTrigger(int nCharaIndex, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* msg);
	// 
	INT MakePacketData_MainInfoReqTriggerEnd(int nCharaIndex, BYTE* msg);
	// �e���˗v��
	INT MakePacketData_MainInfoReqShot(int objno, BYTE* msg);
	// �e���ˋ���
	INT MakePacketData_MainInfoRejShot(int objno, BYTE* msg);

	INT MakePacketData_Confirmed(ptype_session sess, BYTE* msg);

	// ���ʉ�ʂ��烍�r�[�֖߂����Ƃ��̃p�P�b�g
	INT MakePacketData_RoomInfoReEnter(ptype_session sess, BYTE* msg);

	// �n�b�V���l�v��
	INT MakePacketData_AuthReqHash(int hash_group, int hash_id , BYTE* msg);

	// ���^�[���p�X����
	INT MakePacketData_MainInfoRejTurnPass(ptype_session sess, BYTE* msg);

	// ���[�h�����v��
	INT MakePacketData_LoadReqLoadComplete(ptype_session sess, BYTE* msg);

	// �����^�[�����p�P�b�g
	INT MakePacketData_RoomInfoTurnLimit(int turn, BYTE* msg);

	// �������ԃp�P�b�g
	INT MakePacketData_RoomInfoActTimeLimit(int time, BYTE* msg);


	// �����ݒ�p�P�b�g
	INT MakePacketData_MainInfoSetWind(int wind, BYTE* msg);

	// �A�C�e���ǉ��p�P�b�g
	INT MakePacketData_MainInfoAddItem(int obj_no, int slot, DWORD item_flg, BYTE* msg,BOOL bSteal);

	// �t�@�C���n�b�V�����M�p�P�b�g
	INT MakePacket_FileInfoSendHash(BOOL bCharaScr, int id, int fileno, char* md5, WCHAR* path, BYTE* msg);
	// �t�@�C���f�[�^���M�p�P�b�g
	INT MakePacket_FileInfoSendData(BOOL bCharaScr, int id, int fileno, BOOL eof, int dat_index, int size, BYTE* buffer, BYTE* msg);
	// �t�@�C���f�[�^���M�I���p�P�b�g
	INT MakePacket_FileInfoSendClose(BOOL bCharaScr, int id, BYTE* msg);



	// �L�����w�b�_�p�P�b�g�쐬
	// header1			: 1�o�C�g�ڃw�b�_
	// header2			: 2�o�C�g�ڃw�b�_
	// info_count		: �ȉ��̃I�u�W�F�N�g�̏��(0�̏ꍇ�ǉ����Ȃ�)
	// msg			: out �p�P�b�g
	INT MakePacketHeadData(BYTE header1, BYTE header2, int info_count , BYTE* msg);
	INT AddEndMarker(int datIndex, BYTE* msg);
	INT MakePacketData_SYN(BYTE* msg);
};



#endif