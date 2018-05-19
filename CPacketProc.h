#ifndef H_CLASS_PACKET_PROC___
#define H_CLASS_PACKET_PROC___

#include "windows.h"
#include "TCHAR.h"
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "../common/CPacketQueue.h"
#include "CNetworkSession.h"

class CPacketProc
{
public:
	CPacketProc()
	{	p_pNWSess = NULL;
		ZeroMemory(&m_tQueue, sizeof(type_queue));
		InitializeCriticalSection(&m_CriticalSectionPacket);
	};
	virtual ~CPacketProc()
	{	ClearQueue(m_tQueue.next);	
		DeleteCriticalSection(&m_CriticalSectionPacket);
	};

	virtual BOOL Init(CNetworkSession *pNWSess)
	{	p_pNWSess = pNWSess;
		ZeroMemory(&m_tQueue, sizeof(type_queue));
		return TRUE;
	};
	// パケット処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL PacketProc(BYTE *data, ptype_session sess) = 0;
	// 切断処理 / return TRUE/FALSE:送信パケット有り/無し
	virtual BOOL DisconnectSession(ptype_session sess) = 0;
	type_queue*	DequeuePacket()
	{
		EnterCriticalSectionPacket();
		type_queue* ret = m_tQueue.next;
		m_tQueue.next = NULL;
		LeaveCriticalSectionPacket();
		return ret;
	};

protected:

	//> セッションへ送るパケット作成
	BOOL AddPacket(ptype_session sess, BYTE* data, WORD size)
	{
		if (!sess || !data || size > MAX_PACKET_SIZE)
			return FALSE;

		ptype_packet ppkt = NewPacket();
		if (!ppkt) return FALSE;

		ppkt->cli_sock = sess->sock;
		ppkt->session = sess;
		ppkt->size = size;

		ZeroMemory(ppkt->data,sizeof(char)*MAX_PACKET_SIZE);
		CopyMemory(ppkt->data,data,size);
		this->EnterCriticalSectionPacket();
		EnqueuePacket(&m_tQueue, ppkt);
		this->LeaveCriticalSectionPacket();
		return TRUE;
	};
	//< セッションへ送るパケット作成

	//> 見えてるユーザに送るパケット作成
	BOOL AddPacketNoBlindUser(ptype_session send_sess, BYTE* data, WORD size)
	{
		BOOL ret = FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'1');
		EnterCriticalSectionPacket();
		//> 接続済みユーザとの処理
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			// 認証済みか
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			// 飛ばすセッションか
			if (send_sess != pSess
			&& pSess->chara_state[CHARA_STATE_BLIND_INDEX])	continue;
			ret |=AddPacket(pSess, data, size);						// キュー追加
			if (!ret)	break;
		}
		LeaveCriticalSectionPacket();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< 見えてるユーザに送るパケット作成

	//> 全セッションへ送るパケット作成
	BOOL AddPacketAllUser(ptype_session ignore_sess, BYTE* data, WORD size)
	{
		BOOL ret = FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'2');
		EnterCriticalSectionPacket();
		//> 接続済みユーザとの処理
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			// 認証済みか
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (pSess == ignore_sess)	continue;						// 飛ばすセッションか
			ret |=AddPacket(pSess, data, size);						// キュー追加
			if (!ret)	break;
		}
		LeaveCriticalSectionPacket();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< 全セッションへ送るパケット作成

	//> チームセッションへ送るパケット作成
	// team_no	: チーム番号
	BOOL AddPacketTeamUser(int team_no, BYTE* data, WORD size)
	{
		BOOL ret=FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'3');
		EnterCriticalSectionPacket();
		//> 接続済みユーザとの処理
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->team_no != GALLERY_TEAM_NO			// 観戦
			&& pSess->team_no != team_no)	continue;			// 飛ばすセッションか
			ret |= AddPacket(pSess, data, size);						// キュー追加
			if (!ret)	break;
		}
		LeaveCriticalSectionPacket();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< チームセッションへ送るパケット作成

	//> チームセッションへ送るパケット作成
	// team_no	: チーム番号
	BOOL AddPacketTeamNoBlindUser(int team_no, BYTE* data, WORD size)
	{
		BOOL ret=FALSE;
		g_pCriticalSection->EnterCriticalSection_Session(L'4');
		EnterCriticalSectionPacket();
		//> 接続済みユーザとの処理
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if ( pSess->team_no != GALLERY_TEAM_NO
			&& ((pSess->team_no != team_no)
			|| pSess->chara_state[CHARA_STATE_BLIND_INDEX]))	continue;			// 飛ばすセッションか
			ret |= AddPacket(pSess, data, size);						// キュー追加
			if (!ret)	break;
		}
		LeaveCriticalSectionPacket();
		g_pCriticalSection->LeaveCriticalSection_Session();
		return ret;
	};
	//< チームセッションへ送るパケット作成

	CNetworkSession	*p_pNWSess;
	type_queue	m_tQueue;			// パケット作る用

//> パケット
	inline void EnterCriticalSectionPacket()
	{
		EnterCriticalSection(&m_CriticalSectionPacket);
	};
	inline void LeaveCriticalSectionPacket()
	{
		LeaveCriticalSection(&m_CriticalSectionPacket);
	};
	CRITICAL_SECTION	m_CriticalSectionPacket;
//< パケット
};

#endif
