#ifndef H_CLASS_PACKET_PROC_AUTH___
#define H_CLASS_PACKET_PROC_AUTH___

#include "windows.h"
#include "TCHAR.h"
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "../common/CPacketQueue.h"
#include "../common/common.h"
#include "CNetworkSession.h"
#include "PacketMaker.h"
#include "CPacketProc.h"
#include "CPacketProcRoom.h"

class CPacketProcAuth: public CPacketProc
{
public:
	CPacketProcAuth()
	{
		p_pPacketProcRoom = NULL;
	};
	virtual ~CPacketProcAuth()
	{
		p_pPacketProcRoom = NULL;
	};

	inline void SetRelatePacketProcRoom(CPacketProcRoom* p){
		p_pPacketProcRoom = p;
	};
	inline E_STATE_GAME_PHASE GetRoomPhase(){
		if (!p_pPacketProcRoom)
			return GAME_PHASE_INVALID;
		return p_pPacketProcRoom->GetGamePhase();
	};
	// パケット処理
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	virtual BOOL DisconnectSession(ptype_session sess);

protected:
	void InitSession(ptype_session sess);
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// パケット作る用
	BOOL PacketProcConfirm(ptype_session sess, BYTE* data);
	BOOL CheckNextHash(ptype_session sess);
	BOOL PacketProcRetHash(ptype_session sess, BYTE* data);
	BOOL PacketProcRetHashCheck(ptype_session sess, BYTE* data);

	BOOL PacketReqFileHash(ptype_session sess, BYTE* data);
	BOOL PacketReqFileData(ptype_session sess, BYTE* data);
	BOOL PacketSendFileData(ptype_session sess, WCHAR* path, BOOL bCharaScrInfo, int id, int nFileNo, int nDatIndex);
	
	CPacketProcRoom* p_pPacketProcRoom;

};

#endif
