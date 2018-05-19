#ifndef H_CLASS_PACKET_PROC_MAIN___
#define H_CLASS_PACKET_PROC_MAIN___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CPacketProcPhase.h"
#include <lua.hpp>
#include "tolua++.h"
#include "tolua_glue/thg_glue.h"
#include "LuaHelper.h"
#include "BaseVector.h"
#include "CSyncMain.h"

#define PACKET_MAIN_MV_INDEX				(3)

class CPacketProcMain	: public CPacketProcPhase
{
public:
	CPacketProcMain():CPacketProcPhase()
	{
		m_eGamePhase = GAME_PHASE_MAIN;
	};
	virtual ~CPacketProcMain()
	{
	};

	// 初期化
	virtual BOOL Init(CNetworkSession *pNWSess)
	{
		BOOL ret = CPacketProcPhase::Init(pNWSess);
		m_eGamePhase = GAME_PHASE_MAIN;
		return ret;
	};
	// 初期化
	virtual BOOL Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase);

	// パケット処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// 切断処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL DisconnectSession(ptype_session sess);

	virtual INT MakeTriggerPacket(ptype_session sess, BYTE* data);
	// 発射処理
	virtual BOOL ShootingBullet(ptype_session sess, int nFrame);
	
	// 発射コマンド
	virtual bool ShootingCommand(ptype_session sess, int nProcType,int nCharaID, int nBltType,int nAngle,int nPower, int nChrObjNo, int nFrame, int indicator_angle, int indicator_short);

	// 連射するか確認
	virtual BOOL IsDoubleShot(ptype_session sess);

	// 発射のパワー設定
	virtual void SetSaveShotPower(int value){	m_nSaveShotPower = value;	};

protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// パケット作る用
/*
	E_STATE_GAME_PHASE m_eGamePhase;
	ptype_session m_pMasterSession;
	int m_nTeamCount;
	BYTE m_bytRuleFlg;
	int m_nStageIndex;
*/
	// 一発目の情報を記録
	short m_sSaveShotAngle;				// 弾角度
	int m_nSaveShotPower;					// 弾初速
	int m_nSaveBltType;						// 弾の種類
	int m_nSaveCharaType;					// キャラの種類
	int m_nSaveProcType;					// 処理の種類
	int m_nSaveCharaObjNo;				// キャラオブジェクト番号
	short m_sSaveIndicatorAngle;
	int m_nSaveIndicatorPower;
	TCHARA_SCR_INFO* m_pSaveScrInfo;

	BOOL LoadStage();
	BOOL PutCharacter(std::vector<int>* vecGrounds, ptype_session sess);
	BOOL SetMove(ptype_session sess, BYTE* data);
	BOOL SetFlip(ptype_session sess, BYTE* data);
	BOOL UpdatePos(ptype_session sess, BYTE* data);
	BOOL SetShot(ptype_session sess, BYTE* data);
	BOOL SetAck(ptype_session sess);
	BOOL TurnPass(ptype_session sess);
	BOOL OrderItem(ptype_session sess,BYTE* data);
	BOOL UseItem(ptype_session sess, DWORD item_flg);
	BOOL StealItem(ptype_session sess, DWORD item_flg);
	BOOL SetTrigger(ptype_session sess,BYTE* data);
	BOOL RcvTriggerEnd(ptype_session sess,BYTE* data);
	BOOL SetTurnPass(ptype_session sess,BYTE* data);
	BOOL SetShotPowerStart(ptype_session sess, BYTE* data);
};

#endif
