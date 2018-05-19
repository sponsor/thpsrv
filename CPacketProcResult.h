#ifndef H_CLASS_PACKET_PROC_RESULT___
#define H_CLASS_PACKET_PROC_RESULT___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CPacketProcPhase.h"

class CPacketProcResult	: public CPacketProcPhase
{
public:
	CPacketProcResult():CPacketProcPhase()
	{
	};
	virtual ~CPacketProcResult()
	{
	};

	// ������
	virtual BOOL Init(CNetworkSession *pNWSess)
	{
		BOOL ret = CPacketProcPhase::Init(pNWSess);

		m_eGamePhase = GAME_PHASE_RESULT;
		return ret;
	};

	// ������
	BOOL Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase);

	// �p�P�b�g���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// �ؒf���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL DisconnectSession(ptype_session sess);
	
	BOOL SetConfirmed(ptype_session sess);

protected:
	// CNetworkSession	p_pNWSess;
	// type_queue	m_tQueue;			 // �p�P�b�g���p

};

#endif
