#ifndef H_CLASS_SYNC_LOAD___
#define H_CLASS_SYNC_LOAD___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include <vector>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CSyncProc.h"
#include "CNetworkSession.h"

class CSyncLoad : public CSyncProc
{
public:
	CSyncLoad();
	virtual ~CSyncLoad();

	virtual BOOL Init(CNetworkSession* pNetSess);

	virtual void Clear(){};

	// �߂�l�F�p�P�b�g����Ȃ�
	virtual BOOL Frame();

	virtual BOOL IsNextPhase()	{	return m_bNextPhase;	};

//	type_queue*	DequeuePacket()
protected:
	//> �Z�b�V�����֑���p�P�b�g�쐬
//	BOOL AddPacket(ptype_session sess, BYTE* data, WORD size)

	//> �S�Z�b�V�����֑���p�P�b�g�쐬
//	BOOL AddPacketAllUser(ptype_session ignore_sess, BYTE* data, WORD size)

	//> �`�[���Z�b�V�����֑���p�P�b�g�쐬
	// team_no	: �`�[���ԍ�
//	BOOL AddPacketTeamUser(int team_no, BYTE* data, WORD size)
	//< �`�[���Z�b�V�����֑���p�P�b�g�쐬
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// �p�P�b�g���p
	int m_nLoadCompleteCheckIndex;
	int m_nLoadingWaitTimeCounter;
	int m_nLoadingTimeCounter;
	int m_nLoadingCheckUserCount;
	BOOL m_bNextPhase;
};

#endif
