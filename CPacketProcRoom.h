#ifndef H_CLASS_PACKET_PROC_ROOM___
#define H_CLASS_PACKET_PROC_ROOM___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include "util.h"
#include "../common/CCriticalSection.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CPacketProcPhase.h"

#define PACKET_ROOM_MASTER_FLG_INDEX					(3)
#define PACKET_ROOM_GAME_READY_FLG_INDEX			(3)
#define PACKET_ROOM_CHARA_SEL_FLG_INDEX			(3)
#define PACKET_ROOM_MV_INDEX								(3)

class CPacketProcRoom	: public CPacketProcPhase
{
public:
	CPacketProcRoom():CPacketProcPhase()
	{
		m_bytRuleFlg = GAME_RULE_DEFAULT;
		m_eGamePhase = GAME_PHASE_ROOM;
		m_nStageIndex = 0;
		m_nTeamCount = 1;
		m_pMasterSession = NULL;
		m_nActTimeLimit = GAME_TURN_ACT_COUNT;
	};
	virtual ~CPacketProcRoom()
	{
	};

	// ������
	virtual BOOL Init(CNetworkSession *pNWSess)
	{
		BOOL ret = CPacketProcPhase::Init(pNWSess);
		m_bytRuleFlg = GAME_RULE_DEFAULT;
		m_eGamePhase = GAME_PHASE_ROOM;
		m_nStageIndex = 0;
		m_nTeamCount = 1;
		m_pMasterSession = NULL;
		m_nTurnLimit = 0;
		m_nActTimeLimit = GAME_TURN_ACT_COUNT;
		return ret;
	};
	// ������
	BOOL Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase, int nStageIndex, BYTE bytRule, int nActTimeLimit)
	{
		m_nStageIndex = nStageIndex;
		BOOL ret = CPacketProcPhase::Init(pNWSess, pPrevPhase);
		m_bytRuleFlg = bytRule;
		m_nActTimeLimit = nActTimeLimit;
		return ret;
	};

	// �߂�
	BOOL Reset(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase);

	// �p�P�b�g���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// �ؒf���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL DisconnectSession(ptype_session sess);

protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// �p�P�b�g���p
	
	//> �Z�b�V�����֑���p�P�b�g�쐬
	BOOL AddPacket(ptype_session sess, BYTE* data, WORD size)
	{
		if (!sess || !data || size > MAX_PACKET_SIZE)
			return FALSE;
		if ( !(sess->obj_state & OBJ_STATE_ROOM))
			return FALSE;
		ptype_packet ppkt = NewPacket();
		if (!ppkt) return FALSE;

		ppkt->cli_sock = sess->sock;
		ppkt->session = sess;
		ppkt->size = size;

		ZeroMemory(ppkt->data,sizeof(char)*MAX_PACKET_SIZE);
		CopyMemory(ppkt->data,data,size);
		g_pCriticalSection->EnterCriticalSection_Packet(L'7');
		EnqueuePacket(&m_tQueue, ppkt);
		g_pCriticalSection->LeaveCriticalSection_Packet();
		return TRUE;
	};
	//< �Z�b�V�����֑���p�P�b�g�쐬

	//> �S�Z�b�V�����֑���p�P�b�g�쐬
	BOOL AddPacketAllUser(ptype_session ignore_sess, BYTE* data, WORD size)
	{
		BOOL ret = FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'9');
		g_pCriticalSection->EnterCriticalSection_Packet(L'8');
		//> �ڑ��ς݃��[�U�Ƃ̏���
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			// �F�؍ς݂�
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if ( !(pSess->obj_state & OBJ_STATE_ROOM)) continue;
			if (pSess == ignore_sess)	continue;						// ��΂��Z�b�V������
			ret |=AddPacket(pSess, data, size);						// �L���[�ǉ�
			if (!ret)	break;
		}
		g_pCriticalSection->LeaveCriticalSection_Packet();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< �S�Z�b�V�����֑���p�P�b�g�쐬

	// �Q���ς݃��[�U�ɐV�K���[�U�����m����
	BOOL FamiliariseNewUser(ptype_session sess);
	// �V�K���[�U�ɎQ���ςݑS���[�U���𑗂�
	BOOL TellNewUserToEntryUserList(ptype_session sess);

	BOOL AddNewUser(ptype_session sess);
	// �`�[������m�点��
	BOOL TellTeamCount(ptype_session sess, BOOL bAll, ptype_session ignore_sess=NULL);

	// �}�X�^�[�����p�P�b�g����
	BOOL SetRoomMaster(ptype_session sess, BYTE* data);
	// �Q�[�������p�P�b�g����
	BOOL SetGameReady(ptype_session sess, BYTE* data);
	// �L�����I���p�P�b�g����
	BOOL SetCharacter(ptype_session sess, BYTE* data);
	// �L�����ړ��p�P�b�g����
	BOOL SetMove(ptype_session sess, BYTE* data);
	// �L�����A�C�e���I���p�P�b�g����
	BOOL SetItem(ptype_session sess, BYTE* data);
	// �`�[�����ύX�p�P�b�g����
	BOOL SetTeamCount(ptype_session sess, BYTE data);
	// ���[���ύX�p�P�b�g����
	BOOL SetRule(ptype_session sess, BYTE data);
	// �X�e�[�W�I���p�P�b�g����
	BOOL SetStageSelect(ptype_session sess, BYTE data);
	// �����^�[�����p�P�b�g����
	BOOL SetTurnLimit(ptype_session sess, short data);
	// �������ԃp�P�b�g����
	BOOL SetActTimeLimit(ptype_session sess, BYTE data);
	// �`�[�������_��
	BOOL SetTeamRandom(ptype_session sess, BYTE data);

	BOOL SetTeamNo();

	BOOL ReEnter(ptype_session sess);

//	E_STATE_GAME_PHASE m_eGamePhase;
//	ptype_session m_pMasterSession;
//	int m_nTeamCount;
//	BYTE m_bytRuleFlg;
//	int m_nStageIndex;
};

#endif
