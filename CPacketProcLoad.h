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

	// 初期化
	virtual BOOL Init(CNetworkSession *pNWSess)
	{
		BOOL ret = CPacketProcPhase::Init(pNWSess);
		m_eGamePhase = GAME_PHASE_LOAD;
		return ret;
	};
	// 初期化
	BOOL Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase);

	// パケット処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// 切断処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL DisconnectSession(ptype_session sess);


protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// パケット作る用
	// ロード完了パケット処理
	BOOL SetLoadComplete(ptype_session sess);

	// 分かれているか確認
	BOOL CheckTeam(int nTeamCount);
};

#endif
