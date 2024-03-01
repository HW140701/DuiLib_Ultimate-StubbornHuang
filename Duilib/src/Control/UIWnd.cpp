#include "UIWnd.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CWndUI)

	CWndUI::CWndUI()
	{
	}

	CWndUI::~CWndUI()
	{
	}

	LPCTSTR CWndUI::GetClass() const
	{
		return _T("WndUI");
	}

	LPVOID CWndUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, _T("Wnd")) == 0) return static_cast<CWndUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CWndUI::SetVisible(bool bVisible)
	{
		__super::SetVisible(bVisible);
		::ShowWindow(m_hwnd, bVisible);
	}

	void CWndUI::SetInternVisible(bool bVisible)
	{
		__super::SetInternVisible(bVisible);
		::ShowWindow(m_hwnd, bVisible);
	}

	void CWndUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		__super::SetPos(rc, bNeedInvalidate);
		::SetWindowPos(m_hwnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL CWndUI::Attach(HWND hWndNew)
	{
		if (!::IsWindow(hWndNew))
		{
			return FALSE;
		}

		m_hwnd = hWndNew;
		return TRUE;
	}

	HWND CWndUI::Detach()
	{
		HWND hWnd = m_hwnd;
		m_hwnd = NULL;
		return hWnd;
	}

	HWND CWndUI::GetHWND()
	{
		return m_hwnd;
	}
}