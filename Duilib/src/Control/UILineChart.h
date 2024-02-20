#ifndef __UILINECHART_H__
#define __UILINECHART_H__

#include "UIlib.h"

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include <vector>

namespace DuiLib
{
	class UILIB_API CLineChartUI : public CControlUI
	{
		DECLARE_DUICONTROL(CLineChartUI)
	public:
		CLineChartUI();
		~CLineChartUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoEvent(TEventUI& event);
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	public:
		void ResetChart();
		void AddPoint(PointF point);
		void SetCurvePenColor(DWORD dwColor);
		void SetMaxPointNum(unsigned int max_point_num);

	private:
		PointF* m_ptr_point_array;
		std::vector<PointF> m_original_point_vector;
		std::vector<PointF> m_result_point_vector;

		RECT m_line_chart_rect;

		DWORD m_curve_pen_color;

		float min_point_x;
		float max_point_x;
		float min_point_y;
		float max_point_y;

		unsigned int m_max_point_num;
	};
}

#endif // !__UILINECHART_H__
