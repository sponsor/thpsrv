#ifndef H_CLASS_SYNC_PROC___
#define H_CLASS_SYNC_PROC___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include <vector>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"

class CSyncProc
{
public:
	CSyncProc(){
		m_bPacket = FALSE;
		p_pNWSess = NULL;
		ZeroMemory(&m_tQueue, sizeof(type_queue));
		InitializeCriticalSection(&m_CriticalSectionPacket);
	};
	virtual ~CSyncProc()
	{	ClearQueue(m_tQueue.next);	
		DeleteCriticalSection(&m_CriticalSectionPacket);
	};

	virtual BOOL Init(CNetworkSession* pNetSess)
	{	p_pNWSess = pNetSess;
		ZeroMemory(&m_tQueue, sizeof(type_queue));
		return TRUE;
	};

	virtual void Clear(){};

	// �߂�l�F�p�P�b�g����Ȃ�
	virtual BOOL Frame(){	return FALSE;	};

	type_queue*	DequeuePacket()
	{
		EnterCriticalSectionPacket();
		type_queue* ret = m_tQueue.next;
		m_tQueue.next = NULL;
		m_bPacket = FALSE;
		LeaveCriticalSectionPacket();
		return ret;
	};
	
	virtual BOOL HasPacket()
	{
		EnterCriticalSectionPacket();
		BOOL ret = m_bPacket;
		LeaveCriticalSectionPacket();
		return m_bPacket;
	};
protected:
	//> �Z�b�V�����֑���p�P�b�g�쐬
	BOOL AddPacket(ptype_session sess, BYTE* data, WORD size)
	{
		if (!sess || !data || size > MAX_PACKET_SIZE)
			return FALSE;

		ptype_packet ppkt = NewPacket();
		if (!ppkt) return FALSE;

		ppkt->cli_sock = sess->sock;
		ppkt->session = sess;
		ppkt->size = size;

		ZeroMemory(ppkt->data,sizeof(char)*MAX_PACKET_SIZE);
		CopyMemory(ppkt->data,data,size);
		EnterCriticalSectionPacket();
		EnqueuePacket(&m_tQueue, ppkt);
		m_bPacket = TRUE;
		LeaveCriticalSectionPacket();
		return TRUE;
	};
	//< �Z�b�V�����֑���p�P�b�g�쐬

	//> �����Ă郆�[�U�ɑ���p�P�b�g�쐬
	// send_sess :BLIND�ł�����L����
	BOOL AddPacketNoBlindUser(ptype_session send_sess, BYTE* data, WORD size)
	{
		BOOL ret = FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'=');
		EnterCriticalSectionPacket();
		//> �ڑ��ς݃��[�U�Ƃ̏���
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			// �F�؍ς݂�
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			// ��΂��Z�b�V������
			if (send_sess != pSess
			&& pSess->chara_state[CHARA_STATE_BLIND_INDEX])	continue;
			ret |=AddPacket(pSess, data, size);						// �L���[�ǉ�
			if (!ret)	break;
		}
		LeaveCriticalSectionPacket();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< �����Ă郆�[�U�ɑ���p�P�b�g�쐬

	//> �`�[���Z�b�V�����֑���p�P�b�g�쐬
	// team_no	: �`�[���ԍ�
	BOOL AddPacketTeamNoBlindUser(int team_no, BYTE* data, WORD size)
	{
		BOOL ret=FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'~');
		EnterCriticalSectionPacket();
		//> �ڑ��ς݃��[�U�Ƃ̏���
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if ( pSess->team_no != GALLERY_TEAM_NO
			&& ((pSess->team_no != team_no)
			|| pSess->chara_state[CHARA_STATE_BLIND_INDEX]))	continue;			// ��΂��Z�b�V������
			ret |= AddPacket(pSess, data, size);						// �L���[�ǉ�
			if (!ret)	break;
		}
		LeaveCriticalSectionPacket();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< �`�[���Z�b�V�����֑���p�P�b�g�쐬

	//> �S�Z�b�V�����֑���p�P�b�g�쐬
	BOOL AddPacketAllUser(ptype_session ignore_sess, BYTE* data, WORD size)
	{
		BOOL ret = FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'|');
		EnterCriticalSectionPacket();
		//> �ڑ��ς݃��[�U�Ƃ̏���
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			// �F�؍ς݂�
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (pSess == ignore_sess)	continue;						// ��΂��Z�b�V������
			ret |=AddPacket(pSess, data, size);						// �L���[�ǉ�
			if (!ret)	break;
		}
		LeaveCriticalSectionPacket();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< �S�Z�b�V�����֑���p�P�b�g�쐬

	//> �`�[���Z�b�V�����֑���p�P�b�g�쐬
	// team_no	: �`�[���ԍ�
	BOOL AddPacketTeamUser(int team_no, BYTE* data, WORD size)
	{
		BOOL ret=FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'Q');
		this->EnterCriticalSectionPacket();
		//> �ڑ��ς݃��[�U�Ƃ̏���
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (pSess->team_no != GALLERY_TEAM_NO			// �ϐ�
			&& pSess->team_no != team_no)	continue;			// ��΂��Z�b�V������
			ret |= AddPacket(pSess, data, size);						// �L���[�ǉ�
			if (!ret)	break;
		}
		this->LeaveCriticalSectionPacket();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< �`�[���Z�b�V�����֑���p�P�b�g�쐬
	CNetworkSession	*p_pNWSess;
	type_queue	m_tQueue;			// �p�P�b�g���p
	BOOL m_bPacket;

//> �p�P�b�g
	inline void EnterCriticalSectionPacket()
	{
		EnterCriticalSection(&m_CriticalSectionPacket);
	};
	inline void LeaveCriticalSectionPacket()
	{
		LeaveCriticalSection(&m_CriticalSectionPacket);
	};
	CRITICAL_SECTION	m_CriticalSectionPacket;
//< �p�P�b�g
};

#endif