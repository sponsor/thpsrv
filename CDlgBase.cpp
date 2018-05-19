#include "CDlgBase.h"
#include "ext.h"

CDlgBase::CDlgBase()
{
	m_bCreated = FALSE;
	m_pSelf = NULL;
	m_hInst = NULL;
	m_hWnd = NULL;
	m_hDlg = NULL;
	
}

CDlgBase::~CDlgBase()
{
	m_hDlg = NULL;
	m_bCreated = FALSE;
	m_hInst = NULL;
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
}

HWND CDlgBase::Create(HINSTANCE hInst, const TCHAR* lpTemplateName, HWND hWnd)
{
	m_hDlg = CreateDialogParam(hInst, lpTemplateName, hWnd, StaticDlgProc, (LPARAM)this);
	if (!m_hDlg)	return FALSE;
	m_pSelf = this;
	m_hInst = hInst;
	m_hWnd = hWnd;
	m_bCreated = TRUE;

	return hWnd;
}

/*static*/BOOL CALLBACK CDlgBase::StaticDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	// 自身のポインタを取得
	CDlgBase* pSelf = (CDlgBase*)GetWindowLongPtr( hDlg, GWL_USERDATA );

	switch( msg ){
	case WM_INITDIALOG:  // ダイアログボックスが作成されたとき
		// 自身のポインタを設定
		SetWindowLongPtr(hDlg, GWL_USERDATA, (LONG)lp );
		break;
	case  WM_DESTROY:
//		SetWindowLongPtr(hDlg, GWL_USERDATA, 0L);
		break;
	}
	if (pSelf)
		return pSelf->MyDlgProc(hDlg, msg, wp, lp);
	return FALSE;
}

BOOL CALLBACK CDlgBase::MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	return DefWindowProc( hDlg, msg, wp, lp );
}

