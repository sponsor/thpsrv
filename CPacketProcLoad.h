#ifndef H_CLASS_PACKET_PROC_LOAD___
#define H_CLASS_PACKET_PROC_LOAD___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CPacketProcPhase.h"
#include "CPacketProcRoom.h"
#include "../common/common.h"

class CPacketProcLoad	: public CPacketProcPhase
{
public:
	CPacketProcLoad():CPacketProcPhase()
	{
	};
	virtual ~CPacketProcLoad()
	{
	};

	// ������
	virtual BOOL Init(CNetworkSession *pNWSess)
	{
		BOOL ret = CPacketProcPhase::Init(pNWSess);
		m_eGamePhase = GAME_PHASE_LOAD;
		return ret;
	};
	// ������
	BOOL Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase);

	// �p�P�b�g���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// �ؒf���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL DisconnectSession(ptype_session sess);


protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// �p�P�b�g���p
	// ���[�h�����p�P�b�g����
	BOOL SetLoadComplete(ptype_session sess);

	// ������Ă��邩�m�F
	BOOL CheckTeam(int nTeamCount);
};

#endif
