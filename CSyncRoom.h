#ifndef H_CLASS_SYNC_ROOM___
#define H_CLASS_SYNC_ROOM___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include <vector>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CSyncProc.h"
#include "CNetworkSession.h"

class CSyncRoom : public CSyncProc
{
public:
	CSyncRoom();
	virtual ~CSyncRoom();

	virtual BOOL Init(CNetworkSession* pNetSess);

	virtual void Clear()
	{};

	// 戻り値：パケットありなし
	virtual BOOL Frame();

//	type_queue*	DequeuePacket()
protected:
	//> セッションへ送るパケット作成
//	BOOL AddPacket(ptype_session sess, BYTE* data, WORD size)

	//> 全セッションへ送るパケット作成
//	BOOL AddPacketAllUser(ptype_session ignore_sess, BYTE* data, WORD size)

	//> チームセッションへ送るパケット作成
	// team_no	: チーム番号
//	BOOL AddPacketTeamUser(int team_no, BYTE* data, WORD size)
	//< チームセッションへ送るパケット作成
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// パケット作る用

};

#endif
