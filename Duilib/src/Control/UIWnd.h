#ifndef _UI_WND_H_
#define _UI_WND_H_

#include "UIlib.h"

namespace DuiLib
{
	class UILIB_API CWndUI : public CControlUI
	{
		DECLARE_DUICONTROL(CWndUI)
	public:
		CWndUI();
		~CWndUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		virtual void SetVisible(bool bVisible = true);
		virtual void SetInternVisible(bool bVisible = true);

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);

	public:
		BOOL Attach(HWND hWndNew);
		HWND Detach();
		HWND GetHWND();

	protected:
		HWND m_hwnd;
	};
}

#endif // !_UI_WND_H_
