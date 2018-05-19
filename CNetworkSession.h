#ifndef H_CLASS_NETWORK_SESSION___
#define H_CLASS_NETWORK_SESSION___

#include <windows.h>
#include "TCHAR.h"
#include "util.h"
#include "../common/CPacketQueue.h"
#include "../common/common.h"
#include "ext.h"

class CNetworkSession
{
public:
	CNetworkSession()
	{
	};
	virtual ~CNetworkSession()
	{
		ClearAllSession();
	};

// 関数
	// ソケット初期化
	BOOL InitSock();

	// 接続セッションの配列初期化
	void InitSesssionArray();
	void InitSession(ptype_session psession);
	void ResetSession(WORD idx);

	inline ptype_session GetSessionFromIndex(int index)
	{
		if(index>=g_nMaxLoginNum || index < 0) return NULL;
			return &m_SessionArray[index].s;
	};

	inline ptype_session GetSessionFromUserNo(int userNo)
	{
		if(userNo>=MAXUSERNUM || userNo < 0) return NULL;
		for (int i=0;i<g_nMaxLoginNum;i++)
		{
			if (m_SessionArray[i].s.obj_no ==userNo)
				return &m_SessionArray[i].s;
		}
		return NULL;
	}

	inline ptype_session GetSessionFromSockNo(int sockNo)
	{
		if(sockNo>=g_nMaxLoginNum || sockNo < 0) return NULL;
		return (&m_SessionArray[SockNoToUserNo[sockNo]].s);
	}


	// セッション情報用のテーブルの先頭ポインタを取得
	t_sessionInfo* GetSessionTable(){	return &m_SessionArray[0];	};
	ptype_session GetSessionFirst(int* pFirstIndex);
	ptype_session GetSessionNext(int* pSearchIndex);
	ptype_session CreateSession(int sock, INT addr, WORD port);
	void ClearSession(ptype_session psession);
	void ClearAllSession();

	int SetConnectUserName(ptype_session session, char* msg, int len);

	int	GetConnectUserCount()
	{	return m_dwUserCount;	};

	int CalcAuthedUserCount()
	{
		int ret =0;
		for (int i=0;i<g_nMaxLoginNum;i++)
		{
			if (m_SessionArray[i].s.connect_state == CONN_STATE_AUTHED)
				ret ++;
		}
		return ret;
	};

	char GetFlag(int nIndex)
	{
		if (nIndex<g_nMaxLoginNum && nIndex>= 0)
			return m_SessionArray[nIndex].flag;
		return 0;
	};

	int CalcEntityUserCount()
	{
		int ret =0;
		for (int i=0;i<g_nMaxLoginNum;i++)
		{
			if (m_SessionArray[i].s.entity)
				ret ++;
		}
		return ret;
	};

	int CalcMainStageUserCount()
	{
		int ret =0;
		for (int i=0;i<g_nMaxLoginNum;i++)
		{
			if (m_SessionArray[i].s.obj_state & OBJ_STATE_GAME)
				ret ++;
		}
		return ret;
	};

	int GetSockListener()
	{	return sockListener;	};

	BOOL IsUniqueUserName(WCHAR* name);

	t_sessionInfo	m_SessionArray[MAXLOGINUSER];

protected:
	
private:
//	DWORD			m_dwArrayIndex;
//	DWORD			m_dwArrayCount;

	int			m_dwUserCount;

	SOCKET sockListener;
	SOCKADDR_IN	svrAddr;

	type_queue*	GetLast();

	/* 受信パケット処理 */
//	BOOL ProcPK_CMD_MV(BYTE *data, ptype_session sess);		// PK_CMD_MV

	// ソケット番号順の配列にUserNoの値が入ってる
	int	SockNoToUserNo[MAXLOGINUSER];

};

#endif
