#ifndef H_CLASS_PACKET_PROC_PHASE___
#define H_CLASS_PACKET_PROC_PHASE___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "../common/common.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CPacketProc.h"

class CPacketProcPhase	: public CPacketProc
{
public:
	CPacketProcPhase() : CPacketProc()
	{
		m_eGamePhase = GAME_PHASE_ROOM;
	};
	virtual ~CPacketProcPhase()
	{
	};

	// 初期化
	virtual BOOL Init(CNetworkSession *pNWSess)
	{
		m_eGamePhase = GAME_PHASE_ROOM;
		return CPacketProc::Init(pNWSess);
	};
	// 初期化
	virtual BOOL Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase)
	{
		BOOL ret = this->Init(pNWSess);
		m_pMasterSession = pPrevPhase->GetMasterSession();
		m_nTeamCount = pPrevPhase->GetTeamCount();
		m_nStageIndex = pPrevPhase->GetStageIndex();
		m_bytRuleFlg = pPrevPhase->GetRuleFlg();
		m_nTurnLimit = pPrevPhase->GetTurnLimit();
		m_nActTimeLimit = pPrevPhase->GetActTimeLimit();
		return ret;
	};

	virtual E_STATE_GAME_PHASE GetGamePhase() { return m_eGamePhase;	};
	virtual void SetGamePhase(E_STATE_GAME_PHASE value) { m_eGamePhase = value;	};

	ptype_session GetMasterSession() { return	m_pMasterSession; };
	void SetMasterSession(ptype_session sess)
	{
		sess->master = 1;
		sess->game_ready = 0;
		m_pMasterSession = sess;
	};

	int GetTeamCount() { return m_nTeamCount;	};
	int GetStageIndex() { return m_nStageIndex;	};
	BYTE GetRuleFlg() { return m_bytRuleFlg;	};
	int GetTurnLimit() { return m_nTurnLimit;	};
	int GetActTimeLimit() { return m_nActTimeLimit;	};
	// 切断処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL DisconnectSession(ptype_session sess);

protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// パケット作る用

	E_STATE_GAME_PHASE m_eGamePhase;
	ptype_session m_pMasterSession;
	int m_nTeamCount;
	BYTE m_bytRuleFlg;
	int m_nStageIndex;
	int m_nTurnLimit;
	int m_nActTimeLimit;
};

#endif
