// ===================================================================
/** main
 @brief ���C���Fmain.cpp
*/
// ===================================================================

#include "ext.h"
#include "main.h"

#pragma once
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "zlib.lib")

#define SRV_VER		L"v2018_12_08"
#define SRV_VER_Y	2018
#define SRV_VER_M	12
#define SRV_VER_D	8
#if TRIAL
#define SRV_VER_T 'T'
#define NAME_CAPTION	L"���������Q �̌���(server)"
#else
#define SRV_VER_T 'R'
#define NAME_CAPTION	L"���������Q(server)"
#endif

/* =================================== */
/* =====      �O���[�o���ϐ�     ===== */
/* =================================== */
CPacketQueue* g_pPacketQueue;
CNetworkSession* g_pNetSess = NULL;
WCHAR	g_pSrvPassWord[MAX_SRV_PASS+1];
BOOL	g_bLogFile = FALSE;
CTextFile*		g_pLogFile=NULL;
int			g_nTcpPort = MY_TCP_PORT;
BYTE		g_bytIniRuleFlg = GAME_RULE_DEFAULT;
int		g_nActTimeLimit = GAME_TURN_ACT_COUNT;
int	g_nMaxCost = GAME_ITEM_COST_MAX;

CCriticalSection*	g_pCriticalSection = NULL;
CFiler* g_pFiler = NULL;
LuaHelper* g_pLuah = NULL;
E_STATE_GAME_PHASE g_eGamePhase = GAME_PHASE_ROOM;
// CPacketProc //
CPacketProcPhase*	g_pPPPhase = NULL;
CPacketProcChat*	g_pPPChat = NULL;
CPacketProcAuth*	g_pPPAuth = NULL;
CPacketProcRoom*	g_pPPRoom = NULL;
CPacketProcLoad*	g_pPPLoad = NULL;
CPacketProcMain*	g_pPPMain = NULL;
CPacketProcResult*	g_pPPResult = NULL;

type_wmCopyData g_wmCopyData;
HWND	g_hWndLobby = NULL;
// CSyncProc
CSyncRoom*		g_pSyncRoom = NULL;
CSyncLoad*		g_pSyncLoad = NULL;
CSyncMain*		g_pSyncMain = NULL;

/*
 *	===================================
 *	=====      �ÓI�ϐ�     =====
 *	===================================
*/
HWND		g_hWnd	= NULL;
static HINSTANCE g_hInst = NULL;
static BOOL		g_bInit	= FALSE;
static BOOL	g_bActive = TRUE;
CDlgMain*	g_pDlg = NULL;
BOOL g_bOneClient = FALSE;
int g_nMaxSendPacketOneLoop = 12 * 8;
// public �ϐ�	//

#if USE_LUA
lua_State *g_L = NULL;
std::map < int, TCHARA_SCR_INFO > g_mapCharaScrInfo;
std::map < int, TSTAGE_SCR_INFO > g_mapStageScrInfo;
int g_nDefaultCharaID = -1;
int g_nDefaultStageID = -1;
#endif
/*
	===================================
	=====        �v���O����       =====
	===================================
*/

/*
 * -------------------------------------------------------------------
 * ��Еt��
 * -------------------------------------------------------------------
*/
void EndProgram()
{
	g_hWndLobby = NULL;
#ifdef DEBUGCS
		if (g_pCriticalSection)
		{
			WCHAR log[16];
			std::stack<WCHAR> ss;
			WCHAR w=NULL;
			ss = g_pCriticalSection->GetSessionWord();
			while (!ss.empty())
			{
				SafePrintf(log,16,L"cs_sess(%C)", ss.top());
				AddMessageLog(log);
				ss.pop();
			}
			ss = g_pCriticalSection->GetObjectWord();
			while (!ss.empty())
			{
				SafePrintf(log,16,L"cs_obj(%C)", ss.top());
				AddMessageLog(log);
				ss.pop();
			}
		}
#endif
	g_bActive = FALSE;
	StopServer();

	DEBUG_DELETE(g_pDlg, L"EndProgram");
	DEBUG_DELETE(g_pFiler, L"EndProgram");

	DEBUG_DELETE(g_pCriticalSection, L"EndProgram");

	DEBUG_DELETE(g_pPPChat, L"EndProgram");
	DEBUG_DELETE(g_pPPAuth, L"EndProgram");
	DEBUG_DELETE(g_pPPRoom, L"EndProgram");
	DEBUG_DELETE(g_pPPLoad, L"EndProgram");
	DEBUG_DELETE(g_pPPMain, L"EndProgram");
	DEBUG_DELETE(g_pPPResult, L"EndProgram");

	DEBUG_DELETE(g_pSyncRoom, L"EndProgram");
	DEBUG_DELETE(g_pSyncLoad, L"EndProgram");
	DEBUG_DELETE(g_pSyncMain, L"EndProgram");

	g_mapCharaScrInfo.clear();
	g_mapStageScrInfo.clear();
	SafeDelete(g_pLuah);
}

BOOL UpdateWMCopyData()
{
	if (!g_hWndLobby) return FALSE;
	if (!IsWindow(g_hWndLobby))
	{
		g_hWndLobby = NULL;
		return FALSE;
	}
	// �ڑ��l��
	g_pCriticalSection->EnterCriticalSection_Session('G');
	g_wmCopyData.client_num = (BYTE)g_pNetSess->GetConnectUserCount();
	if (!g_pPPPhase)
	{
		g_wmCopyData.state = gsNone;
	}
	else
	{
		switch (g_pPPPhase->GetGamePhase())
		{
		case GAME_PHASE_ROOM:			// ��������
		case GAME_PHASE_RESULT:		// ���ʉ��
		case GAME_PHASE_RETURN:		// �߂�
			g_wmCopyData.state = gsRoom;
			break;
		case GAME_PHASE_LOAD:			// ���[�f�B���O
		case GAME_PHASE_MAIN:			// ���C��
			g_wmCopyData.state = gsGame;
			if (g_pSyncMain)
				g_wmCopyData.live_num = (BYTE)g_pSyncMain->GetLivingCharacters();
			break;
		default:
			g_wmCopyData.state = gsNone;
			break;
		}
	}
	g_pCriticalSection->LeaveCriticalSection_Session();

	COPYDATASTRUCT data;
//	data.dwData = MY_TCP_PORT;
	data.dwData = g_nTcpPort;
	data.cbData = sizeof(type_wmCopyData);
	data.lpData = &g_wmCopyData;
	SendMessage(g_hWndLobby, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
	return TRUE;
}

// Ini
void ReadIni()
{
	CIniFile* pIniFile = new CIniFile();
	WCHAR pTemp[255];
	int nLen = pIniFile->ReadString(L"CONFIG", L"PASS", L"", pTemp, 255);
	memcpy(g_pSrvPassWord, pTemp, sizeof(WCHAR)*MAX_SRV_PASS);
	if (nLen > MAX_SRV_PASS)
	{
		g_pSrvPassWord[MAX_SRV_PASS] = NULL;
		pIniFile->WriteString(L"CONFIG", L"PASS", g_pSrvPassWord);
		MessageBox(g_hWnd, L"�ݒ肳�ꂽ�p�X���[�h�����߂������߁A\n16�����ȍ~��؂�̂Ă܂���", L"info", MB_OK);
	}

	g_nTcpPort = pIniFile->ReadInt(L"CONFIG", L"PORT", MY_TCP_PORT);
	g_bOneClient = pIniFile->ReadBool(L"CONFIG", L"ONE_CLIENT", FALSE);
	g_nMaxLoginNum = min(MAXLOGINUSER, max(MAXUSERNUM, pIniFile->ReadInt(L"CONFIG", L"MAX_LOGIN", MAXLOGINNUM)));
	g_nMaxSendPacketOneLoop = g_nMaxLoginNum*8;
#if ONE_CLIIENT
	g_bOneClient = TRUE;
#endif
	g_bytIniRuleFlg = 0x0;
	if (pIniFile->ReadBool(L"RULE", L"ITEM", GAME_RULE_01_DEFAULT))
		g_bytIniRuleFlg |= GAME_RULE_01;
	if (pIniFile->ReadBool(L"RULE", L"WIND", GAME_RULE_02_DEFAULT))
		g_bytIniRuleFlg |= GAME_RULE_02;
	if (pIniFile->ReadBool(L"RULE", L"TEAM_DAMAGE", GAME_RULE_03_DEFAULT))
		g_bytIniRuleFlg |= GAME_RULE_03;
	if (pIniFile->ReadBool(L"RULE", L"START_CARE", GAME_RULE_04_DEFAULT))
		g_bytIniRuleFlg |= GAME_RULE_04;

	g_nActTimeLimit = min(25,max(15,pIniFile->ReadInt(L"RULE", L"ACT_TIME_LIMIT", GAME_TURN_ACT_COUNT)));		
#ifdef _DEBUG
	g_bLogFile = pIniFile->ReadBool(L"CONFIG", L"DEBUG", TRUE);
#else
	g_bLogFile = FALSE; // pIniFile->ReadBool(L"CONFIG", L"DEBUG", FALSE);
#endif
	g_nMaxCost = min(1000, max(0, pIniFile->ReadInt(L"RULE", L"COST", DEFAULT_GAME_ITEM_COST_MAX)));

	SafeDelete(pIniFile);
}

// -------------------------------------------------------------------
// �V�X�e��������
// input:	INSTANCE	: �C���X�^���X�n���h��
// output:	BOOL = TRUE : ����������
// -------------------------------------------------------------------
BOOL InitSystem(HINSTANCE hInst)
{
	// �t�@�C���̓f�t�H���g�Ńo�C�i�����[�h
	_set_fmode(_O_BINARY);
	setlocale(LC_ALL,"japanese");

	ZeroMemory(&g_wmCopyData, sizeof(type_wmCopyData));
	g_wmCopyData.vtype = SRV_VER_T;
	g_wmCopyData.vy = SRV_VER_Y;
	g_wmCopyData.vm = SRV_VER_M;
	g_wmCopyData.vd = SRV_VER_D;

	ReadIni();

	// �_�C�A���O
	DEBUG_NEW(g_pDlg, CDlgMain(), L"InitSystem");
	// System
	DEBUG_NEW(g_pFiler, CFiler(), L"InitSystem");
	// �p�P�b�g�����N���X����
	DEBUG_NEW(g_pPPChat, CPacketProcChat(), L"InitSystem");
	DEBUG_NEW(g_pPPAuth, CPacketProcAuth(), L"InitSystem");
	DEBUG_NEW(g_pPPRoom, CPacketProcRoom(), L"InitSystem");
	DEBUG_NEW(g_pPPLoad, CPacketProcLoad(), L"InitSystem");
	DEBUG_NEW(g_pPPMain, CPacketProcMain(), L"InitSystem");
	DEBUG_NEW(g_pPPResult, CPacketProcResult(), L"InitSystem");

	// �֘A�t��
	g_pPPAuth->SetRelatePacketProcRoom(g_pPPRoom);

	// ���������N���X����
	DEBUG_NEW(g_pSyncRoom, CSyncRoom(),  L"InitSystem");
	DEBUG_NEW(g_pSyncLoad, CSyncLoad(),  L"InitSystem");
	DEBUG_NEW(g_pSyncMain, CSyncMain(),  L"InitSystem");

	WNDCLASSEX wc;
	// �E�B���h�E�N���X�̏���ݒ�
	wc.cbSize = sizeof(wc);               // �\���̃T�C�Y
	wc.style = CS_HREDRAW | CS_VREDRAW;   // �X�^�C��
	wc.lpfnWndProc = WndProc;             // �E�B���h�E�v���V�[�W��
	wc.cbClsExtra = 0;                    // �g�����P
	wc.cbWndExtra = 0;                    // �g�����Q
	wc.hInstance = hInst;                 // �C���X�^���X�n���h��
	wc.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
#if 0	// Icon
	wc.hIcon = (HICON)LoadImage(          // �A�C�R��
		NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON,
		0, 0, LR_DEFAULTSIZE | LR_SHARED
	);
	wc.hIconSm = wc.hIcon;                // �q�A�C�R��
	wc.hCursor = (HCURSOR)LoadImage(      // �}�E�X�J�[�\��
		NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
		0, 0, LR_DEFAULTSIZE | LR_SHARED
	);
#else
	wc.hCursor =  wc.hIconSm = wc.hIcon = NULL;
#endif

	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // �E�B���h�E�w�i
	wc.lpszMenuName = NULL;                     // ���j���[��
	wc.lpszClassName = NAME_WIN_CLASS_SRV;// �E�B���h�E�N���X��

	WCHAR caption[64];
	if (g_bOneClient)
		SafePrintf(caption, 64, L"%s ONE CLIENT", NAME_CAPTION);
	else
		SafePrintf(caption, 64, L"%s %s", NAME_CAPTION, SRV_VER);

	// �E�B���h�E�N���X��o�^����
	if( RegisterClassEx( &wc ) == 0 ){ return NULL; }

	// �E�B���h�E���쐬����
	g_hWnd = CreateWindow(
		wc.lpszClassName,      // �E�B���h�E�N���X��
		caption,  // �^�C�g���o�[�ɕ\�����镶����
		WS_OVERLAPPEDWINDOW,   // �E�B���h�E�̎��
		WIN_LEFT,              // �E�B���h�E��\������ʒu�iX���W�j
		WIN_TOP,              // �E�B���h�E��\������ʒu�iY���W�j
		WIN_WIDTH,          // �E�B���h�E�̕�
		WIN_HEIGHT,         // �E�B���h�E�̍���
		NULL,                  // �e�E�B���h�E�̃E�B���h�E�n���h��
		NULL,                  // ���j���[�n���h��
		hInst,                 // �C���X�^���X�n���h��
		NULL                   // ���̑��̍쐬�f�[�^
	);

//	BaseVector::InitBaseVector();
	// randomize
#if RANDOMIZE
	time_t aclock;
	time(&aclock);
	init_genrand((ULONG)aclock);
#else
	init_genrand(1);
#endif
	// logfile
	if (g_bLogFile)
	{
		static WCHAR szDate[32];
		static WCHAR szTime[32];
//		GetLocalTime(&sysTime);                        // ���݂̎��Ԃ����߂�

//		GetDateFormat( LOCALE_USER_DEFAULT,TIME_FORCE24HOURFORMAT,&sysTime,L"yyMMdd",szDate,sizeof(szDate)); 
//		GetTimeFormat(LOCALE_USER_DEFAULT,0, &sysTime,L"HHmm",szTime,sizeof(szTime));
		if ( GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, L"HHmm", szTime, 32)
		&& GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0,NULL, L"yyMMdd", szDate, 32))
		{
			WCHAR logfile[_MAX_PATH*2+1];
			SafePrintf(logfile, _MAX_PATH*2, L"slog%s%s.txt", szDate, szTime);
			g_pLogFile = new CTextFile(logfile, L"a+");
			if (!g_pLogFile->IsOpened())
			{
				MessageBox(NULL, L"���O�t�@�C���̃I�[�v���Ɏ��s���܂����B",L"error", MB_OK);
				EndProgram();
				return FALSE;
			}
		}
	}

	g_bInit = (g_hWnd != NULL);
	return g_bInit;
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch( msg )
	{
	case WM_NULL:
		AddMessageLog(L"<RestartServer>");
		if (g_bRestartFlg)
		{
			g_bRestartFlg = FALSE;
			StopServer();
			StartServer();
		}
//		OutputDebugStr(L"<RestartServer");
		break;
	case WM_CREATE:			// �E�B���h�E���쐬���ꂽ�Ƃ�
		// ���[�h���X�_�C�A���O�{�b�N�X���쐬����
		g_pDlg->Create(g_hInst, _T("IDD_DIALOG1"), hWnd);
		return 0;
	case WM_DESTROY:        // �E�B���h�E���j�������Ƃ�
		// �_�C�A���O�{�b�N�X��j������
		g_pDlg->Destroy();
		return 0;

	case WM_APP+1:
		if (wp != NULL && lp == MY_TCP_PORT)
		{
			g_hWndLobby = (HWND)wp;
			UpdateWMCopyData();
		}
		break;
	case WM_APP+2:
		g_hWndLobby = NULL;
		break;
	}

	return DefWindowProc( hWnd, msg, wp, lp );
}

// -------------------------------------------------------------------
// ���C��
// -------------------------------------------------------------------
INT APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	timeBeginPeriod(1);										// �^�C�}�[�̐��x��1ms�ɂ���
	g_pSrvPassWord[0] = NULL;

	// ����������
	if (!InitSystem(hInst)){
		EndProgram();
		return E_FAIL;
	}

	MSG msg;
	g_hInst = hInst;

#if USE_LUA
	// Lua��VM�𐶐�����
	g_L = lua_open();
	// Lua�̕W�����C�u�������J��
	luaL_openlibs(g_L);
	// bit���Z���C�u����
	luaopen_bit(g_L);
	// �O���[�R�[�h�����s
	tolua_thg_open(g_L);

	g_pLuah = new LuaHelper();
	g_pLuah->SetLua(g_L);

	if (!common::scr::LoadLoaderScript(g_L, LUA_FILE))
	{
		MessageBox(NULL, L"���[�_�[�X�N���v�g�G���[", L"error", MB_OK);
		EndProgram();
		return 0;
	}

	if (!common::scr::LoadAllCharaScript(g_L, g_pLuah, &g_mapCharaScrInfo))
	{
		MessageBox(NULL, L"�L�����X�N���v�g���[�h�G���[\n", L"error", MB_OK);
		EndProgram();
		return 0;
	}
	g_nDefaultCharaID = g_mapCharaScrInfo.begin()->first;

	if (!common::scr::LoadAllStageScript(g_L, g_pLuah, &g_mapStageScrInfo))
	{
		MessageBox(NULL, L"�X�e�[�W�X�N���v�g���[�h�G���[", L"error", MB_OK);
		EndProgram();
		return 0;
	}
	g_nDefaultStageID = g_mapStageScrInfo.begin()->first;
#endif

	// �T�[�o�J�n
	StartServer();

	HWND hDlg = g_pDlg->GetDlgHwnd();
	if (hDlg)
	{
		WCHAR caption[64];
		GetWindowText(g_hWnd, caption, 64);
		SetWindowText(hDlg, caption);
	}

	// ���b�Z�[�W���[�v
	while (hDlg = g_pDlg->GetDlgHwnd())
	{																			// ���b�Z�[�W����������Ȃ烋�[�v
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // ���b�Z�[�W���擾����
		if( ret == 0 || ret == -1 )
		{			// �A�v���P�[�V�������I�������郁�b�Z�[�W
			break;
		}
		else
		{
			// �_�C�A���O�{�b�N�X�ɑ΂��郁�b�Z�[�W��
			if( hDlg == NULL || !IsDialogMessage( hDlg, &msg ) )
			{	// �_�C�A���O�ւ̃��b�Z�[�W�łȂ���΁A�E�B���h�E�ɑ���
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
	}
	timeEndPeriod(1);										// �^�C�}�[�̐��x��߂�
	// ��Еt��
	EndProgram();
	
	CHECK_DEBUG_MEMORY_LEAK;
	return 0;
}


void KillGame()
{
	if (g_pSyncMain && g_eGamePhase == GAME_PHASE_MAIN)
		g_pSyncMain->KillGame();
}

#if USE_LUA
// -------------------------------------------------------------------
// Lua
// -------------------------------------------------------------------

#endif
