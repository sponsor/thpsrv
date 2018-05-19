#include "ext.h"
#include "CDlgMain.h"


WCHAR g_msgbuf[MAX_LOG_BUFFER+1];
HWND g_hDlg = NULL;

CDlgMain::CDlgMain()
{
	m_bCreated = FALSE;
	m_bCheckGameNoLog = FALSE;
	m_pSelf = NULL;
	m_hInst = NULL;
	m_hWnd = NULL;
	m_hDlg = NULL;
	m_hMenuUserListContext = NULL;	
}

CDlgMain::~CDlgMain()
{
	DestroyMenu(m_hMenuUserListContext);
//	m_hDlg = NULL;
//	m_bCreated = FALSE;
//	m_hInst = NULL;
//	m_hWnd = NULL;
}

HWND CDlgMain::Create(HINSTANCE hInst, const TCHAR* lpTemplateName, HWND hWnd)
{
	HWND ret = CDlgBase::Create(hInst, lpTemplateName, hWnd);
	
	m_hMenuMain = CreateMenu();
	m_hMenuFile = CreatePopupMenu();
	AppendMenu(m_hMenuFile, MF_ENABLED | MF_STRING ,IDC_MENU_FILE_EXIT, L"exit");
	AppendMenu(m_hMenuMain, MF_ENABLED | MF_POPUP| MF_STRING , (UINT)m_hMenuFile, L"file");

	m_hMenuServer = CreatePopupMenu();
	AppendMenu(m_hMenuServer, MF_ENABLED | MF_STRING  ,IDC_MENU_SRV_RESTART, L"restart");
	AppendMenu(m_hMenuServer, MF_ENABLED | MF_SEPARATOR ,NULL, L"");
	AppendMenu(m_hMenuServer, MF_ENABLED | MF_STRING | MF_CHECKED ,IDC_MENU_SRV_START, L"start");
	AppendMenu(m_hMenuServer, MF_ENABLED | MF_STRING, IDC_MENU_SRV_STOP, L"stop");

	AppendMenu(m_hMenuMain, MF_ENABLED | MF_POPUP| MF_STRING , (UINT)m_hMenuServer, L"server");

	m_hMenuGame = CreatePopupMenu();
	AppendMenu(m_hMenuGame, MF_ENABLED | MF_STRING  ,IDC_MENU_GAME_NOLOG, L"ゲーム中のログを表示しない");
	AppendMenu(m_hMenuGame, MF_ENABLED | MF_STRING  ,IDC_MENU_GAME_END, L"ゲームを終了させる");

	AppendMenu(m_hMenuMain, MF_ENABLED | MF_POPUP| MF_STRING , (UINT)m_hMenuGame, L"game");

	SetMenu(m_hDlg, m_hMenuMain);
	DrawMenuBar(m_hDlg);

	m_hMenuUserListContext = CreatePopupMenu();
	AppendMenu(m_hMenuUserListContext, MF_ENABLED | MF_STRING  ,IDC_MENU_USERLIST_KICK, L"選択中のユーザをキック");
	AppendMenu(m_hMenuUserListContext, MF_ENABLED | MF_STRING  ,IDC_MENU_USERLIST_MASTER, L"選択中のユーザをマスターに設定");
	
	return ret;
}

BOOL CALLBACK CDlgMain::MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	g_hDlg = hDlg;
	switch( msg )
	{
	case WM_SHOWWINDOW:
		SetWindowPos(hDlg,NULL, 0,0,IDC_MAIN_EDIT_DEFAULT_W+IDC_MAIN_LIST_W,100,SWP_NOZORDER);
		break;
	case WM_WINDOWPOSCHANGED:
		UpdateResize();
		return TRUE;
	case WM_CONTEXTMENU:
		OnRightClick(	LOWORD(lp), HIWORD(lp));
		break;
	case WM_COMMAND:     // ダイアログボックス内の何かが選択されたとき
		switch( LOWORD( wp ) )
		{
			case IDCANCEL:   // 「キャンセル」ボタンが選択された
				// ダイアログボックスを消す
				if (EndDialog( hDlg, IDCANCEL ))
					SetDlgHwnd(NULL);
				break;
			case IDC_MENU_FILE_EXIT:
				if (EndDialog( hDlg, IDC_MENU_FILE_EXIT ))
					SetDlgHwnd(NULL);
				break;
			case IDC_MENU_SRV_RESTART:
				OnStopServerClick();
				Sleep(100);
				OnStartServerClick();
				break;
			case IDC_MENU_SRV_START:
				OnStartServerClick();
				break;
			case IDC_MENU_SRV_STOP:
				OnStopServerClick();
				break;
			case IDC_MENU_GAME_NOLOG:
				if (m_bCheckGameNoLog)
				{
					CheckMenuItem(m_hMenuGame, IDC_MENU_GAME_NOLOG, MF_UNCHECKED);
					m_bCheckGameNoLog = FALSE;
				}
				else
				{
					CheckMenuItem(m_hMenuGame, IDC_MENU_GAME_NOLOG, MF_CHECKED);
					m_bCheckGameNoLog = TRUE;
				}
				break;
			case IDC_MENU_GAME_END:
				OnKillGame();
				break;
			case IDC_MENU_USERLIST_KICK:
				OnUserKick();
				break;
			case IDC_MENU_USERLIST_MASTER:
				OnUserSetMaster();
				break;
			default:
				return FALSE;
        }
		return TRUE;
	}

	return FALSE;  // FALSEを返す
}

void CDlgMain::OnStartServerClick()
{
//	HWND hSSWnd = GetDlgItem(m_hDlg, IDC_MENU_SRV_START);
	UINT uState = GetMenuState( m_hMenuServer, IDC_MENU_SRV_START, MF_BYCOMMAND );
	if( uState & MFS_CHECKED )
	{
		return;
	}
	else
	{
		CheckMenuItem( m_hMenuServer, IDC_MENU_SRV_STOP, MF_BYCOMMAND | MFS_UNCHECKED );
		CheckMenuItem( m_hMenuServer, IDC_MENU_SRV_START, MF_BYCOMMAND | MFS_CHECKED );
		StartServer();
	}
}

void CDlgMain::OnStopServerClick()
{
//	HWND hSSWnd = GetDlgItem(m_hDlg, IDC_MENU_SRV_START);
	UINT uState = GetMenuState( m_hMenuServer, IDC_MENU_SRV_STOP, MF_BYCOMMAND );
	if( uState & MFS_CHECKED )
	{
		return;
	}
	else
	{
		CheckMenuItem( m_hMenuServer, IDC_MENU_SRV_STOP, MF_BYCOMMAND | MFS_CHECKED );
		CheckMenuItem( m_hMenuServer, IDC_MENU_SRV_START, MF_BYCOMMAND | MFS_UNCHECKED );
		StopServer();
		SendMessage(GetDlgItem(g_hDlg, IDC_MAIN_LIST), LB_RESETCONTENT, 0, NULL);
	}
}


void CDlgMain::UpdateResize()
{
	if (!m_hDlg)
		return;
	RECT WinRect, ListRect, wr;
	GetClientRect(m_hDlg, &WinRect);
	HWND hEditWnd = GetDlgItem(m_hDlg, IDC_MAIN_EDIT);
	if (!hEditWnd) return;

	HWND hListWnd = GetDlgItem(m_hDlg, IDC_MAIN_LIST);
	if (!hListWnd) return;
	GetClientRect(hListWnd, &ListRect);
	wr.right = WinRect.right-WinRect.left;
	wr.bottom = WinRect.bottom-WinRect.top;

	MoveWindow(hEditWnd, 0,0, wr.right-IDC_MAIN_LIST_W, wr.bottom, TRUE);
	MoveWindow(hListWnd, wr.right-IDC_MAIN_LIST_W,0, IDC_MAIN_LIST_W, wr.bottom, TRUE);

}

void CDlgMain::OnRightClick(int x,int y)
{
	RECT WinRect,ListRect;
	POINT pt;	pt.x = x;	pt.y = y;
	ScreenToClient(m_hDlg, &pt);
	GetClientRect(m_hDlg, &WinRect);
	HWND hListWnd = GetDlgItem(m_hDlg, IDC_MAIN_LIST);
	GetClientRect(hListWnd, &ListRect);
	if (WinRect.right-IDC_MAIN_LIST_W < pt.x && WinRect.right-2 > pt.x
	&& ListRect.top < pt.y && ListRect.bottom > pt.y)
	{
		LONG lLbSel=SendMessage(hListWnd,LB_GETCURSEL,0,0);
		if(lLbSel==LB_ERR)	return;
		int nNameLen=SendMessage(hListWnd,LB_GETTEXTLEN,(WPARAM)lLbSel,0);
		BOOL bEnable = (nNameLen > MAX_USER_NAME || nNameLen < MIN_USER_NAME);
		EnableMenuItem(m_hMenuUserListContext,IDC_MENU_USERLIST_KICK, bEnable);
		TrackPopupMenu(m_hMenuUserListContext, 0, x, y, 0, m_hDlg, NULL);
	}
}

void CDlgMain::OnUserKick()
{
	WCHAR name[MAX_USER_NAME+1];
	HWND hListWnd = GetDlgItem(m_hDlg, IDC_MAIN_LIST);
	LONG lLbSel=SendMessage(hListWnd,LB_GETCURSEL,0,0);
	if(lLbSel==LB_ERR)	return;
	int nNameLen=SendMessage(hListWnd,LB_GETTEXTLEN,(WPARAM)lLbSel,0);
	if (nNameLen > MAX_USER_NAME || nNameLen < MIN_USER_NAME)	return;
	SendMessage(hListWnd,LB_GETTEXT,(WPARAM)lLbSel,(LPARAM)&name[0]);
	int nUserIndex = GetUserIndexFromUserName(name);
	if (nUserIndex != -1)
	{
		int nListCount = SendMessage(hListWnd,LB_GETCOUNT,0,0);
		g_pCriticalSection->EnterCriticalSection_Lua(L'K');
		g_pCriticalSection->EnterCriticalSection_Packet(L'K');
		g_pCriticalSection->EnterCriticalSection_Object(L'K');
		g_pCriticalSection->EnterCriticalSection_Log(L'K');
		g_pCriticalSection->EnterCriticalSection_Session(L'K');
		WCHAR log[64];
		SafePrintf(log, 64, L"Kick:%s", name);
		AddMessageLog(log);
		KickUser(nUserIndex);
		if (nListCount>1)
		{
			g_pCriticalSection->LeaveCriticalSection_Session();
			g_pCriticalSection->LeaveCriticalSection_Log();
			g_pCriticalSection->LeaveCriticalSection_Object();
			g_pCriticalSection->LeaveCriticalSection_Packet();
			g_pCriticalSection->LeaveCriticalSection_Lua();
		}
	}
}

void CDlgMain::OnKillGame()
{
	if (!m_hDlg)	return;
	KillGame();
}

void CDlgMain::OnUserSetMaster()
{
	WCHAR name[MAX_USER_NAME+1];
	HWND hListWnd = GetDlgItem(m_hDlg, IDC_MAIN_LIST);
	LONG lLbSel=SendMessage(hListWnd,LB_GETCURSEL,0,0);
	if(lLbSel==LB_ERR)	return;
	int nNameLen=SendMessage(hListWnd,LB_GETTEXTLEN,(WPARAM)lLbSel,0);
	if (nNameLen > MAX_USER_NAME || nNameLen < MIN_USER_NAME)	return;
	SendMessage(hListWnd,LB_GETTEXT,(WPARAM)lLbSel,(LPARAM)&name[0]);
	int nUserIndex = GetUserIndexFromUserName(name);
	if (nUserIndex == -1)	return;
	SetMaster(nUserIndex);
}

void InsertUserList(int nIndex, const WCHAR* user)
{
	SendMessage(GetDlgItem(g_hDlg, IDC_MAIN_LIST), LB_ADDSTRING, 0, (LPARAM)user);
}

void DeleteUserList(ptype_session sess)
{
	WCHAR username[MAX_USER_NAME+1];
	ZeroMemory(username, (MAX_USER_NAME+1)*sizeof(WCHAR));
	SafeMemCopy(username, sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	int nIndex = SendMessage(GetDlgItem(g_hDlg, IDC_MAIN_LIST), LB_FINDSTRINGEXACT, (WPARAM)0, (LPARAM)username);
	if (nIndex != LB_ERR)
		SendMessage(GetDlgItem(g_hDlg, IDC_MAIN_LIST), LB_DELETESTRING, nIndex, NULL);	
}


void ClearUserList()
{
	SendMessage(GetDlgItem(g_hDlg, IDC_MAIN_LIST), LB_RESETCONTENT, 0, NULL);
}

void AddMessageLog(const WCHAR* msglog, BOOL logf)
{
	WCHAR msgtemp[MAX_LOG_BUFFER+1];
	if (g_pCriticalSection) g_pCriticalSection->EnterCriticalSection_Log(L'1');

	// ダイアログへのログ出力
	DWORD bGameNoLog = g_pDlg->GetCheckedGameNoLog();
	if (bGameNoLog)
		bGameNoLog = (g_eGamePhase == GAME_PHASE_MAIN);	// メインゲーム中か確認

	if (bGameNoLog && !(g_pLogFile && logf))
	{
		if (g_pCriticalSection) g_pCriticalSection->LeaveCriticalSection_Log();
		return;
	}
	std::wstring ws = msglog;
	if (!bGameNoLog)
	{
		_tcscpy_s(msgtemp, (size_t)MAX_LOG_BUFFER, g_msgbuf);
		SafePrintf(g_msgbuf, MAX_LOG_BUFFER, L"%s\r\n%s", msglog, msgtemp);

		SetWindowText(GetDlgItem(g_hDlg, IDC_MAIN_EDIT), g_msgbuf);
	}

	if (g_pLogFile && logf)
	{
		SYSTEMTIME sysTime;
		WCHAR pMsg[MAX_LOG_BUFFER];
		static WCHAR szDate[32];
		static WCHAR szTime[32];
		GetLocalTime(&sysTime);                        // 現在の時間を求める

		GetDateFormat( LOCALE_USER_DEFAULT,
								0,	&sysTime,
								L"yyyy'/'MM'/'dd",
								szDate,sizeof(szDate)); 
		GetTimeFormat(LOCALE_USER_DEFAULT,
								0, 	&sysTime,
								L"HH':'mm':'ss''",
								szTime,sizeof(szTime));
		SafePrintf(pMsg, MAX_LOG_BUFFER, L"[%s-%s] %s", szDate, szTime, msglog);
		g_pLogFile->WriteLine(pMsg);
	}
	if (g_pCriticalSection) g_pCriticalSection->LeaveCriticalSection_Log();
}

