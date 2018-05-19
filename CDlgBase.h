#ifndef H_CLASS_DLG_BASE___
#define H_CLASS_DLG_BASE___
// ===================================================================
// main.h
//	ダイアログ・ヘッダー
// ===================================================================
#include <windows.h>
#include <TCHAR.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "resource.h"


class CDlgBase
{
public:
	CDlgBase();
	virtual ~CDlgBase();

	virtual HWND Create(HINSTANCE hInst, const TCHAR* lpTemplateName, HWND hWnd);
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

protected:
	// ダイアログプロシージャ
	static BOOL CALLBACK StaticDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	// ダイアログプロシージャ
	virtual BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

	CDlgBase*		m_pSelf;
	HINSTANCE		m_hInst;
	HWND				m_hWnd;
	HWND				m_hDlg;

	BOOL				m_bCreated;

};

#endif