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

	// パケット処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// 切断処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL DisconnectSession(ptype_session sess);

protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// パケット作る用

	BOOL AddNewUser(int obj_no);
};

#endif
