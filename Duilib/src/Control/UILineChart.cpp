#include "UILineChart.h"
#include "Core/UIDefine.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CLineChartUI)

	CLineChartUI::CLineChartUI()
		:m_ptr_point_array(NULL),
		min_point_x(0.0),
		max_point_x(0.0),
		min_point_y(0.0),
		max_point_y(0.0),
		m_max_point_num(1000)
	{

	}

	CLineChartUI::~CLineChartUI()
	{
		if (m_ptr_point_array != nullptr)
		{
			delete[] m_ptr_point_array;
			m_ptr_point_array = nullptr;
		}
	}

	LPCTSTR CLineChartUI::GetClass() const
	{
		return _T("LineChart");
	}

	LPVOID CLineChartUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, _T("LineChart")) == 0) return static_cast<CLineChartUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CLineChartUI::DoEvent(TEventUI& event)
	{
		CControlUI::DoEvent(event);
	}

	bool CLineChartUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		PointF* temp_ptr_point_array = new PointF[m_result_point_vector.size()];
		for (int i = 0; i < m_result_point_vector.size(); ++i)
		{
			temp_ptr_point_array[i] = m_result_point_vector[i];
		}


		Pen pen(m_curve_pen_color);

		Graphics graphics(hDC);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);

		Status restatus = graphics.DrawCurve(&pen, (const PointF*)temp_ptr_point_array, m_result_point_vector.size());

		if (temp_ptr_point_array != nullptr)
			delete[] temp_ptr_point_array;

		return true;
	}

	void CLineChartUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("curvepencolor")) == 0)
		{
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetCurvePenColor(clrColor);
		}
		else if (_tcscmp(pstrName, _T("maxpointnum")) == 0)
		{
			SetMaxPointNum(_ttoi(pstrValue));
		}
		else
		{
			CControlUI::SetAttribute(pstrName, pstrValue);
		}
	}

	void CLineChartUI::ResetChart()
	{
		m_original_point_vector.clear();
		m_original_point_vector.shrink_to_fit();

		m_result_point_vector.clear();
		m_result_point_vector.shrink_to_fit();
	}

	void CLineChartUI::AddPoint(PointF point)
	{
		m_original_point_vector.emplace_back(point);

		if (point.X < min_point_x)
			min_point_x = point.X;
		if(point.X > max_point_x)
			max_point_x = point.X;

		if (point.Y < min_point_y)
			min_point_y = point.Y;
		if (point.Y > max_point_y)
			max_point_y = point.Y;

		int line_chart_width = this->GetWidth();
		int line_chart_height = this->GetHeight();

		float data_width = max_point_x - min_point_x;
		float data_height = max_point_y - min_point_y;
		
		m_result_point_vector.clear();
		for (int i = 0; i < m_original_point_vector.size(); ++i)
		{
			PointF temp_point = m_original_point_vector[i];
			PointF result_point;

			result_point.X = (temp_point.X - min_point_x) / data_width * (float)line_chart_width;
			result_point.Y = line_chart_height - ((temp_point.Y - min_point_y) / data_height * (float)line_chart_height);

			m_result_point_vector.emplace_back(result_point);
		}
	}

	void CLineChartUI::SetCurvePenColor(DWORD dwColor)
	{
		m_curve_pen_color = dwColor;
	}

	void CLineChartUI::SetMaxPointNum(unsigned int max_point_num)
	{
		m_max_point_num = max_point_num;
	}
}