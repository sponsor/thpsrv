#include "main.h"
#include "ext.h"

HANDLE g_hThreadChat = NULL;
HANDLE g_hThreadSync = NULL;
HANDLE g_hThreadPacketReceiver = NULL;
HANDLE g_hThreadPacketSender = NULL;

int StartServer(void)
{
	DWORD		dwThreadId;

	OutputDebugStr(L">StartServer\n");

	if (!g_pCriticalSection)
		DEBUG_NEW(g_pCriticalSection,CCriticalSection(), L"StartServer" );

	g_pCriticalSection->EnterCriticalSection_Object(L'=');
	g_pCriticalSection->EnterCriticalSection_Packet(L'Q');
	g_pCriticalSection->EnterCriticalSection_Session(L'A');
	g_pCriticalSection->EnterCriticalSection_Lua(L'1');

	if (!g_pNetSess)
		DEBUG_NEW(g_pNetSess,CNetworkSession(), L"StartServer" );
	g_pNetSess->InitSesssionArray();

	if (!g_pPacketQueue)
	{
		OutputDebugStr(L"StartServer(queue new)\n");
		DEBUG_NEW(g_pPacketQueue, CPacketQueue(), L"StartServer");
	}
	else
	{
		OutputDebugStr(L"StartServer(ClearQueue)\n");
		g_pCriticalSection->EnterCriticalSection_Packet(L'W');
		g_pPacketQueue->ClearQueue();
		g_pCriticalSection->LeaveCriticalSection_Packet();
	}
	OutputDebugStr(L"StartServer(Init)\n");

	// ReceivePacketProc
	g_pPPAuth->Init(g_pNetSess);
	g_pPPChat->Init(g_pNetSess);
	g_pPPRoom->Init(g_pNetSess, g_pPPRoom, g_nDefaultStageID, g_bytIniRuleFlg, g_nActTimeLimit);
	g_pPPLoad->Init(g_pNetSess);
	g_pPPMain->Init(g_pNetSess);

	g_pPPPhase = g_pPPRoom;

	// SyncProc
	g_pSyncRoom->Init(g_pNetSess );
	g_pSyncLoad->Init(g_pNetSess);

	// ソケット作成
	if (!g_pNetSess->InitSock())
	{
		AddMessageLog(L"ソケット初期化失敗");
		g_pCriticalSection->LeaveCriticalSection_Lua();
		g_pCriticalSection->LeaveCriticalSection_Session();
		g_pCriticalSection->LeaveCriticalSection_Packet();
		g_pCriticalSection->LeaveCriticalSection_Object();
		MessageBox(g_hWnd, L"ソケット初期化失敗", L"server start error", MB_OK);
		return 0;
	}

	g_eGamePhase = GAME_PHASE_ROOM;

	dwThreadId=1;
	g_hThreadPacketReceiver=CreateThread(NULL,0, (LPTHREAD_START_ROUTINE)Thread_PacketReceiver,g_pCriticalSection,0,&dwThreadId);
	Sleep(100);

	dwThreadId=2;
	g_hThreadPacketSender=CreateThread(NULL,0,Thread_PacketSender,g_pCriticalSection,0,&dwThreadId);
	Sleep(100);

	dwThreadId=3;
	g_hThreadSync=CreateThread(NULL,0,Thread_Synchronizer,g_pCriticalSection,0,&dwThreadId);
	Sleep(100);

	g_pCriticalSection->LeaveCriticalSection_Lua();
	g_pCriticalSection->LeaveCriticalSection_Session();
	g_pCriticalSection->LeaveCriticalSection_Packet();
	g_pCriticalSection->LeaveCriticalSection_Object();

	UpdateWMCopyData();
	OutputDebugStr(L"<StartServer\n");
	AddMessageLog(L"StartServer");
	return 0;
}

void StopThread(HANDLE handle)
{
	DWORD dwParam;
	if (GetExitCodeThread(handle, &dwParam))
	{
		if (dwParam == STILL_ACTIVE) 
		{
			TerminateThread(handle , FALSE);
			WaitForSingleObject(handle, 100);        /* プロセスの終了を待つ */
			CloseHandle(handle);
		}
	}
}

void StopServer(void)
{
	AddMessageLog(L"StopServer");
	OutputDebugStr(L"StopPacketReceiverThread\n");
	if (g_hThreadPacketReceiver)
		StopThread(g_hThreadPacketReceiver);

	OutputDebugStr(L"StopPacketSenderThread\n");
	if (g_hThreadPacketSender)
		StopThread(g_hThreadPacketSender);

	OutputDebugStr(L"StopSyncThread\n");
	if (g_hThreadSync)
		StopThread(g_hThreadSync);

	g_pPPPhase = NULL;
	UpdateWMCopyData();
	Sleep(100);

	DEBUG_DELETE(g_pNetSess, L"StopServer");
	DEBUG_DELETE(g_pCriticalSection, L"StopServer");
	WSACleanup();
}


int GetUserIndexFromUserName(WCHAR* name)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'S');
	t_sessionInfo* sesstbl = g_pNetSess->GetSessionTable();
	for (int i=0;i<g_nMaxLoginNum;i++)
	{
		if (sesstbl[i].flag && sesstbl[i].s.connect_state == CONN_STATE_AUTHED)
		{
			WCHAR sess_name[MAX_USER_NAME+1];
			common::session::GetSessionName(&sesstbl[i].s, sess_name);
			if (wcscmp(name, sess_name) == 0)
			{
				g_pCriticalSection->LeaveCriticalSection_Session();
				return i;
			}
		}
	}
	g_pCriticalSection->LeaveCriticalSection_Session();
	return -1;
}

void KickUser(int nCharaIndex)
{
	DisconnectSession(g_pNetSess->GetSessionFromIndex(nCharaIndex));
}

void SetMaster(int nCharaIndex)
{
	if (g_pPPRoom)
	{
		ptype_session pMaster = g_pPPPhase->GetMasterSession();
		if (pMaster && pMaster->sess_index != nCharaIndex)
		{
			BYTE pkt[MAX_PACKET_SIZE];
			WORD	packetSize = PacketMaker::MakePacketData_RoomInfoMaster(pMaster->sess_index, FALSE, pkt);
			g_pCriticalSection->EnterCriticalSection_Session(L'D');
			g_pCriticalSection->EnterCriticalSection_Packet(L'E');
			// マスター設定を変更
			ptype_session pNewMaster = g_pNetSess->GetSessionFromIndex(nCharaIndex);
			pMaster->master = 0;
			pMaster->game_ready = 0;
			g_pPPRoom->SetMasterSession(pNewMaster);
			g_pPPLoad->SetMasterSession(pNewMaster);
			g_pPPMain->SetMasterSession(pNewMaster);
			g_pPPResult->SetMasterSession(pNewMaster);
			
			AddPacketAllUser(g_pPacketQueue, NULL, g_pNetSess->GetSessionTable(), pkt, packetSize);
			packetSize = PacketMaker::MakePacketData_RoomInfoMaster(nCharaIndex, TRUE, pkt);
			AddPacketAllUser(g_pPacketQueue, NULL, g_pNetSess->GetSessionTable(), pkt, packetSize);
	
			g_pCriticalSection->LeaveCriticalSection_Packet();
			g_pCriticalSection->LeaveCriticalSection_Session();
		}
	}
}

