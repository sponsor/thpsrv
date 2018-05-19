#ifndef H_CLASS_SYNC_MAIN___
#define H_CLASS_SYNC_MAIN___

#include "windows.h"
#include "TCHAR.h"
#include <algorithm>
#include <assert.h>
#include <vector>
#include <map>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include <lua.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dxerr9.h>
#include "tolua++.h"
#include "tolua_glue/thg_glue.h"
#include "LuaHelper.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CSyncProc.h"
#include "CPacketProcPhase.h"
#include "CMainStage.h"
#include "../common/common.h"

class CSyncMain : public CSyncProc
{
public:
	CSyncMain();
	virtual ~CSyncMain();

	virtual BOOL Init(CNetworkSession* pNetSess, CFiler* pFiler, CPacketProcPhase* pPPPhase);

	virtual void Clear();

	// ゲーム強制終了
	virtual void KillGame()	{	m_bKillGame = TRUE;	};

	virtual int GetStageWidth(){ return m_pMainStage->GetStageWidth();	};
	virtual int GetStageHeight(){ return m_pMainStage->GetStageHeight();	};
	virtual int GetBulletCount(){ 	return m_mapObjects.size();	};
	// Lua
	virtual int AddObject(type_obj* obj);	// 戻り値:obj_no
	virtual bool RemoveObject(int nIndex, E_OBJ_RM_TYPE rm_type);
	virtual bool IsRemovedObject(int nIndex);
	virtual bool BombObject(int scr_id, int blt_type, int blt_chr_no, int blt_no, int pos_x, int pos_y, int erase);
	virtual bool UpdateCharaStatus(int obj_no, int hp, int mv, int delay, int exp);
	virtual bool UpdateCharaPos(int obj_no, int x,int y);
	virtual bool SetBulletExtData1(int obj_no, DWORD extdata1);
	virtual bool SetBulletExtData2(int obj_no, DWORD extdata2);
	virtual unsigned int GetCharaExtData1(int obj_no);
	virtual unsigned int GetCharaExtData2(int obj_no);

	virtual bool SetBulletOptionData(int obj_no, int index, DWORD data);
	virtual unsigned int GetBulletOptionData(int obj_no, int index);

	virtual bool SetCharaOptionData(int obj_no, int index, DWORD data);
	virtual unsigned int GetCharaOptionData(int obj_no, int index);

	virtual bool SetCharaExtData1(int obj_no, DWORD extdata1);
	virtual bool SetCharaExtData2(int obj_no, DWORD extdata2);
	virtual bool AddCharaItem(int obj_no, DWORD item_flg);
	virtual bool SetCharacterState(int obj_no, int chr_stt, int val);
	virtual int GetCharacterState(int obj_no, int chr_stt);
	virtual unsigned long GetCharacterItemInfo(int obj_no, int item_index);
	virtual bool UpdateObjectState(int obj_no, E_TYPE_OBJ_STATE state);
	virtual bool UpdateBulletPositoin(int obj_no, double px, double py, double vx, double vy, double adx, double ady);
	virtual bool UpdateBulletVector(int obj_no, double vx, double vy, double adx, double ady);
	virtual BOOL PasteTextureOnStage(int scr_id, int sx,int sy, int tx,int ty,int tw,int th);
	virtual bool DamageCharaHP(int assailant_no, int victim_no, int hp);
	virtual int GetEntityCharacters();
	virtual int GetLivingCharacters();
	virtual type_session* GetCharacterAtVector(int index);
	virtual int GetWind() { return m_nWind; };
	virtual int GetWindDirection() { return m_nWindDirection;	};
	virtual void SetWind(int nValue, int nDir);
	virtual bool UpdateObjType(int obj_no, BYTE type);
	virtual type_blt* GetBulletInfo(int obj_no);
	virtual int GetBulletAtkValue(int obj_no);
	virtual void SetShotPowerStart(ptype_session sess, int type);
	BOOL GetRandomStagePos(POINT* pnt);

	// 戻り値：パケットありなし
	virtual BOOL Frame();
	std::vector< type_session* > m_vecCharacters;
	CMainStage*	m_pMainStage;

//	CLinkedList<type_obj>*	m_pLLObjects;
	std::map<int, type_obj*>	m_mapObjects;
	std::vector< int > m_vecObjectNo;

	E_STATE_GAME_MAIN_PHASE	GetPhase()
	{	return m_eGameMainPhase;	};
	void SetPhase(E_STATE_GAME_MAIN_PHASE phase);
	void SetCheckPhase(E_STATE_GAME_MAIN_PHASE next_phase )
	{
		SetPhase(GAME_MAIN_PHASE_CHECK);
		m_eGameNextPhase = next_phase;
	};
	
	void SetSyncPhase(E_STATE_GAME_MAIN_PHASE next_phase );

	ptype_session GetActiveSession()
	{	return p_pActiveSession;	};

	void SetStealSession(ptype_session sess)
	{	p_pStealSess = sess;	};
	ptype_session GetStealSession()
	{	return p_pStealSess;	};

	// Luaからの処理フラグを立てる
	void SetObjectLuaFlg(int obj_no, DWORD flg, BOOL on);

	// disconnect session
	void OnDisconnectSession(ptype_session sess);

	BOOL LoadStage();
	void PutCharacter(std::vector<int>* vecGrounds, ptype_session sess, BOOL PutOnly=FALSE);

	BOOL IsShotPowerWorking(){ return m_bShotPowerWorking;	};
	int GetPhaseTimeCount(){	return m_nPhaseTimeCounter;	};
//	type_queue*	DequeuePacket()

	// 終了
	void GameEnd();
protected:
	//> セッションへ送るパケット作成
//	BOOL AddPacket(ptype_session sess, BYTE* data, WORD size)

	//> 全セッションへ送るパケット作成
//	BOOL AddPacketAllUser(ptype_session ignore_sess, BYTE* data, WORD size)

	//> チームセッションへ送るパケット作成
	// team_no	: チーム番号
//	BOOL AddPacketTeamUser(int team_no, BYTE* data, WORD size);
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// パケット作る用

	int AddBullet(ptype_blt blt);
	inline int AddCharacter(type_session* sess)
	{
		m_vecCharacters.push_back(sess);
		return m_vecCharacters.size()-1;
	};

	std::map<int, type_obj*>::iterator EraseMapObject(std::map<int, type_obj*>::iterator it);
	void UpdateObjectNo();
	// return TRUE : 送信パケットあり
	// GAME_MAIN_PHASE_ACT
	BOOL FrameCharacters();							// RET:TRUE = 移動した
	BOOL FrameActiveCharacter();					// RET:TRUE = アクティブ終了
	BOOL FrameNoneActiveCharacter();			// RET:TRUE = 移動した
	BOOL MoveCharacter(ptype_session sess);	// RET:TRUE = 移動した

	BOOL FrameActCharacters();
	BOOL FrameObjects();	// RETURN:TRUE=ACTIVEなし

	BOOL FrameSync();

	// GAME_MAIN_PHASE_TRIGGER //
	void FrameTrigger();

	// GAME_MAIN_PHASE_SHOT //
	BOOL OnHitStageItemBullet(type_blt* blt);
	BOOL BombItemBulletReverse(type_blt* blt);			// 逆さ
	BOOL BombItemBulletBlind(type_blt* blt);				// 暗くなる
	BOOL BombItemBulletRepair(type_blt* blt);			// 回復弾
	BOOL BombItemBulletTeleport(type_blt* blt);			// 移動弾
	BOOL BombItemBulletDrain(type_blt* blt);				// 吸収弾
	BOOL BombItemBulletFetch(type_blt* blt);				// 引き寄せ弾
	BOOL BombItemBulletExchange(type_blt* blt);		// 位置替え弾
	BOOL BombItemBulletNoAngle(type_blt* blt);			// 角度変更不可
	BOOL BombItemBulletNoMove(type_blt* blt);			// 移動不可

	// GAME_MAIN_PHASE_CHECK //

	// GAME_MAIN_PHASE_TURNEND //
	BOOL FrameTurnEnd();
	void NotifyTurnEndCharacter(ptype_session active_sess);
	void NotifyTurnStartCharacter(ptype_session next_sess);
	void NotifyTurnEndBullet(ptype_session active_sess);
	void NotifyTurnStartBullet(ptype_session next_sess);
	void NotifyTurnEndStage(ptype_session active_sess, ptype_session next_sess);

	// GAME_MAIN_PHASE_RETURN //
	BOOL FrameReturn();
	ptype_session GetNextActiveChara();
	BOOL CountCharacterState(type_session* sess);
	BOOL UpdateCharacterState(type_session* sess);

	// 終了判定
	BOOL IsGameEnd();
	BOOL IsGameEndOfIndividualMatch();
	BOOL IsGameEndOfTeamBattle();
	// 死亡通知
	BOOL NotifyCharaDead(ptype_session sess, E_TYPE_CHARA_DEAD type, ptype_session ignore_sess=NULL);
	// キャラ順位設定
	void SetRankOrder();
	void SetRankOrderOfIndividualMatch();
	void SetRankOrderOfTeamBattle();
	// 操作可能なキャラ判定
	inline BOOL IsControlChara(type_session* sess)
	{
		if (!sess)
			return FALSE;
		else if (sess->connect_state != CONN_STATE_AUTHED
			|| !sess->entity
			|| !(sess->obj_state & OBJ_STATE_GAME)
			|| (sess->obj_state & (OBJ_STATE_MAIN_NOLIVE_FLG|OBJ_STATE_MAIN_GALLERY_FLG))
			)
			return FALSE;
		return TRUE;
	};

	// 風
	int m_nWind;
	int m_nWindDirection;
	int m_nWindDirValancer;

	int m_nPhaseTimeCounter;						// カウンタ
	int m_nPhaseTime;								// 時間

	int m_nPhaseReturnIndex;
	int m_nPhaseReturnTimeTotal;

	int m_nPhaseSyncIndex;

	void ClearGameReady();

	int m_nObjectNoCounter;

	E_STATE_GAME_MAIN_PHASE m_eGameMainPhase;
	E_STATE_GAME_MAIN_PHASE m_eGameNextPhase;
	ptype_session	p_pActiveSession;

	BYTE m_bytRule;			// ルールフラグ
	BOOL m_bReqShotFlg;	// 発射要求フラグ
	BOOL m_bDoubleShotFlg;	// 連射済フラグ
	BYTE m_nTurnLimit;			// 制限ターン数

	TSTAGE_SCR_INFO*	m_pStageScrInfo;
	int m_nTurnCount;
	int m_nLivingTeamCountTable[MAXUSERNUM];
	BOOL m_bShotPowerWorking;
	ptype_session p_pStealSess;

	BOOL m_bKillGame;			// ゲームの強制終了
	BOOL m_bStartIntervalEnd;		// 初期インターバル

};

#endif
