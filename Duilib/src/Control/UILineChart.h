#ifndef __UILINECHART_H__
#define __UILINECHART_H__

#include "UIlib.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include <vector>
#include <map>

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
		void AddCurveData(int curve_index, float value);
		void SetCurveData(int curve_index, const std::vector<float>& curve_data);
		void SetCurvePenColor(DWORD dwColor);
		void SetMaxPointNum(unsigned int max_point_num);
		void SetCurveNum(unsigned int curve_num);

		int GetCurveCount();

	private:
		PointF* m_ptr_point_array;

		std::map<int, std::vector<float>> m_original_curve_data_map;
		std::map<int, std::vector<PointF>> m_result_curve_point_map;

		DWORD m_curve_pen_color;

		float min_value;
		float max_value;

		unsigned int m_max_point_num;
		unsigned int m_curve_num;
	};
}

#endif // !__UILINECHART_H__
