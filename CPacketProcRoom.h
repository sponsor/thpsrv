#ifndef H_CLASS_PACKET_PROC_ROOM___
#define H_CLASS_PACKET_PROC_ROOM___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include "util.h"
#include "../common/CCriticalSection.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CPacketProcPhase.h"

#define PACKET_ROOM_MASTER_FLG_INDEX					(3)
#define PACKET_ROOM_GAME_READY_FLG_INDEX			(3)
#define PACKET_ROOM_CHARA_SEL_FLG_INDEX			(3)
#define PACKET_ROOM_MV_INDEX								(3)

class CPacketProcRoom	: public CPacketProcPhase
{
public:
	CPacketProcRoom():CPacketProcPhase()
	{
		m_bytRuleFlg = GAME_RULE_DEFAULT;
		m_eGamePhase = GAME_PHASE_ROOM;
		m_nStageIndex = 0;
		m_nTeamCount = 1;
		m_pMasterSession = NULL;
		m_nActTimeLimit = GAME_TURN_ACT_COUNT;
	};
	virtual ~CPacketProcRoom()
	{
	};

	// 初期化
	virtual BOOL Init(CNetworkSession *pNWSess)
	{
		BOOL ret = CPacketProcPhase::Init(pNWSess);
		m_bytRuleFlg = GAME_RULE_DEFAULT;
		m_eGamePhase = GAME_PHASE_ROOM;
		m_nStageIndex = 0;
		m_nTeamCount = 1;
		m_pMasterSession = NULL;
		m_nTurnLimit = 0;
		m_nActTimeLimit = GAME_TURN_ACT_COUNT;
		return ret;
	};
	// 初期化
	BOOL Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase, int nStageIndex, BYTE bytRule, int nActTimeLimit)
	{
		m_nStageIndex = nStageIndex;
		BOOL ret = CPacketProcPhase::Init(pNWSess, pPrevPhase);
		m_bytRuleFlg = bytRule;
		m_nActTimeLimit = nActTimeLimit;
		return ret;
	};

	// 戻る
	BOOL Reset(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase);

	// パケット処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// 切断処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL DisconnectSession(ptype_session sess);

protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// パケット作る用
	
	//> セッションへ送るパケット作成
	BOOL AddPacket(ptype_session sess, BYTE* data, WORD size)
	{
		if (!sess || !data || size > MAX_PACKET_SIZE)
			return FALSE;
		if ( !(sess->obj_state & OBJ_STATE_ROOM))
			return FALSE;
		ptype_packet ppkt = NewPacket();
		if (!ppkt) return FALSE;

		ppkt->cli_sock = sess->sock;
		ppkt->session = sess;
		ppkt->size = size;

		ZeroMemory(ppkt->data,sizeof(char)*MAX_PACKET_SIZE);
		CopyMemory(ppkt->data,data,size);
		g_pCriticalSection->EnterCriticalSection_Packet(L'7');
		EnqueuePacket(&m_tQueue, ppkt);
		g_pCriticalSection->LeaveCriticalSection_Packet();
		return TRUE;
	};
	//< セッションへ送るパケット作成

	//> 全セッションへ送るパケット作成
	BOOL AddPacketAllUser(ptype_session ignore_sess, BYTE* data, WORD size)
	{
		BOOL ret = FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'9');
		g_pCriticalSection->EnterCriticalSection_Packet(L'8');
		//> 接続済みユーザとの処理
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			// 認証済みか
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if ( !(pSess->obj_state & OBJ_STATE_ROOM)) continue;
			if (pSess == ignore_sess)	continue;						// 飛ばすセッションか
			ret |=AddPacket(pSess, data, size);						// キュー追加
			if (!ret)	break;
		}
		g_pCriticalSection->LeaveCriticalSection_Packet();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< 全セッションへ送るパケット作成

	// 参加済みユーザに新規ユーザを周知する
	BOOL FamiliariseNewUser(ptype_session sess);
	// 新規ユーザに参加済み全ユーザ情報を送る
	BOOL TellNewUserToEntryUserList(ptype_session sess);

	BOOL AddNewUser(ptype_session sess);
	// チーム数を知らせる
	BOOL TellTeamCount(ptype_session sess, BOOL bAll, ptype_session ignore_sess=NULL);

	// マスター権限パケット処理
	BOOL SetRoomMaster(ptype_session sess, BYTE* data);
	// ゲーム準備パケット処理
	BOOL SetGameReady(ptype_session sess, BYTE* data);
	// キャラ選択パケット処理
	BOOL SetCharacter(ptype_session sess, BYTE* data);
	// キャラ移動パケット処理
	BOOL SetMove(ptype_session sess, BYTE* data);
	// キャラアイテム選択パケット処理
	BOOL SetItem(ptype_session sess, BYTE* data);
	// チーム数変更パケット処理
	BOOL SetTeamCount(ptype_session sess, BYTE data);
	// ルール変更パケット処理
	BOOL SetRule(ptype_session sess, BYTE data);
	// ステージ選択パケット処理
	BOOL SetStageSelect(ptype_session sess, BYTE data);
	// 制限ターン数パケット処理
	BOOL SetTurnLimit(ptype_session sess, short data);
	// 制限時間パケット処理
	BOOL SetActTimeLimit(ptype_session sess, BYTE data);
	// チームランダム
	BOOL SetTeamRandom(ptype_session sess, BYTE data);

	BOOL SetTeamNo();

	BOOL ReEnter(ptype_session sess);

//	E_STATE_GAME_PHASE m_eGamePhase;
//	ptype_session m_pMasterSession;
//	int m_nTeamCount;
//	BYTE m_bytRuleFlg;
//	int m_nStageIndex;
};

#endif
