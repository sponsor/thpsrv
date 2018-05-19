#ifndef H_CLASS_PACKET_PROC_CHAT___
#define H_CLASS_PACKET_PROC_CHAT___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CPacketProc.h"

#define PACKET_CHAT_HEADER_INDEX		(3)

class CPacketProcChat	: public CPacketProc
{
public:
	CPacketProcChat()
	{
	};
	virtual ~CPacketProcChat()
	{
	};

	// �p�P�b�g���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// �ؒf���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL DisconnectSession(ptype_session sess);

protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// �p�P�b�g���p

	BOOL AddNewUser(int obj_no);
};

#endif
