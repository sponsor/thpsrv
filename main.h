#ifndef H_MAIN___
#define H_MAIN___
// ===================================================================
// main.h
//	画面管理クラス動作テスト(with ファイル分割)・ヘッダー
// ===================================================================

#include <windows.h>
#include <time.h>
#include <winsock.h>
#include <stdio.h>
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAPALLOC
#include <crtdbg.h>

#include "../include/define.h"
#include "util.h"
#include "dxerr9.h"
#include "CDlgMain.h"

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "ext.h"
#include "../include/types.h"

#if USE_LUA
#include <lua.hpp>
#include "tolua++.h"
#include "tolua_glue/thg_glue.h"
#include "LuaHelper.h"
#endif

#include "CIniFile.h"

#include "../common/CPacketQueue.h"
#include "../common/common.h"
#include "CNetworkSession.h"

#include "CPacketProc.h"
#include "CPacketProcPhase.h"
#include "CPacketProcAuth.h"
#include "CPacketProcChat.h"
#include "CPacketProcRoom.h"
#include "CPacketProcLoad.h"
#include "CPacketProcMain.h"
#include "CPacketProcResult.h"

#include "CSyncProc.h"
#include "CSyncRoom.h"
#include "CSyncLoad.h"
#include "CSyncMain.h"

#include "resource.h"

#ifdef _DEBUG
#pragma comment(lib, "util_d.lib")
#pragma comment(lib, "IniFile_d.lib")
#else
#pragma comment(lib, "util.lib")
#pragma comment(lib, "IniFile.lib")
#endif

/* =================================== */
/* =====  外部参照グローバル変数 ===== */
/* =================================== */

class CSyncMain;
/* =================================== */
/* ===== 外部参照関数プロトタイプ===== */
/* =================================== */
extern CPacketQueue* g_pPacketQueue;
extern CNetworkSession* g_pNetSess;

extern CPacketProcAuth*	g_pPPAuth;
extern CPacketProcChat*	g_pPPChat;
extern CPacketProcRoom*	g_pPPRoom;
extern CPacketProcLoad*	g_pPPLoad;
extern CPacketProcMain*	g_pPPMain;
extern CPacketProcResult*	g_pPPResult;
extern CPacketProcPhase*	g_pPPPhase;

extern CSyncRoom*		g_pSyncRoom;
extern CSyncLoad*		g_pSyncLoad;
extern CSyncMain*		g_pSyncMain;
extern int				g_nMaxSendPacketOneLoop;

/* =================================== */
/* ===== Lua 参照関数プロトタイプ===== */
/* =================================== */

/* =================================== */
/* =====     プロトタイプ宣言    ===== */
/* =================================== */
BOOL InitSystem(HINSTANCE hInst);
void EndProgram();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
BOOL InitSystem(HINSTANCE hInst);

DWORD __stdcall Thread_PacketReceiver(PVOID param);
DWORD __stdcall Thread_Synchronizer(LPVOID param);
DWORD __stdcall Thread_PacketSender(PVOID param);

int NewUserSession(fd_set *rfds, int nTcpSock, CNetworkSession *pNWSess, pptype_session ppSess);
int ErrorUserSession(int nConnectSocket, fd_set *efds, int nRecvSize, CNetworkSession *pNWSess, ptype_session sess);
void DisconnectSession(ptype_session sess);
BOOL PacketProc(CNetworkSession *pNWSess, BYTE* msg, ptype_session sess, INT msgsize);
void PacketEnqueue();
void MoveMainstage(ptype_session sess);

enum E_COPYDATA_RESULT_GAME_STATE:BYTE
{
	gsNone,
	gsRoom,
	gsGame,
};

typedef struct TWM_COPYDATA
{
	BYTE client_num;
	BYTE live_num;
	E_COPYDATA_RESULT_GAME_STATE	state;
	char	vtype;
	WORD vy;
	BYTE vm;
	BYTE vd;
} type_wmCopyData;

#endif
