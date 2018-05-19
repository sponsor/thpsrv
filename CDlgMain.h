#ifndef H_CLASS_DLG_MAIN___
#define H_CLASS_DLG_MAIN___
// ===================================================================
// main.h
//	ダイアログ・ヘッダー
// ===================================================================
#include <windows.h>
#include <TCHAR.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "CDlgBase.h"
#include "resource.h"
#include "../include/types.h"
#include "../include/define.h"


class CDlgMain : public CDlgBase
{
public:
	CDlgMain();
	virtual ~CDlgMain();
	virtual HWND Create(HINSTANCE hInst, const TCHAR* lpTemplateName, HWND hWnd);
/*
	virtual void Destroy()
	{
		if (m_hWnd)
			DestroyWindow(m_hWnd);
	};

	// Get
	inline HWND GetWndHwnd()
	{	return m_hWnd;	};
	inline HWND GetDlgHwnd()
	{ return m_hDlg;	};
	inline void SetDlgHwnd(HWND hWnd)
	{ m_hDlg = hWnd;	};
	inline HINSTANCE GetHInstance()
	{	return m_hInst;	};
	inline BOOL IsCreated()
	{ return m_bCreated;	};
	inline BOOL SetCreated(BOOL b)
	{ m_bCreated = b;	};
*/
	BOOL GetCheckedGameNoLog(){	return m_bCheckGameNoLog;	};

protected:

	// ダイアログプロシージャ
//	static BOOL CALLBACK StaticDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	// ダイアログプロシージャ
	virtual BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void UpdateResize();
	void OnStartServerClick();
	void OnStopServerClick();
	void OnRightClick(int x,int y);
	void OnUserKick();
	void OnUserSetMaster();
	void OnKillGame();
/*
	CDlgBase*		m_pSelf;
	HINSTANCE		m_hInst;
	HWND				m_hWnd;
	HWND				m_hDlg;

	BOOL				m_bCreated;
*/
	HMENU m_hMenuMain;
	HMENU m_hMenuFile;
	HMENU m_hMenuServer;
	HMENU m_hMenuGame;

	BOOL m_bCheckGameNoLog;		// ログを表示しない

	HMENU m_hMenuUserListContext;

};

#endif