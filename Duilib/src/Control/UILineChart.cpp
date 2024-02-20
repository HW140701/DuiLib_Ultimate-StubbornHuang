#include "UILineChart.h"
#include "Core/UIDefine.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CLineChartUI)

	CLineChartUI::CLineChartUI()
		:m_ptr_point_array(NULL),
		min_value(0.0),
		max_value(0.0),
		m_max_point_num(1000),
		m_curve_num(1)
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
		CControlUI::DoPaint(hDC, rcPaint, pStopControl);

		// 将压入的数据进行坐标换算
		int line_chart_width = this->GetWidth();
		int line_chart_height = this->GetHeight();

		float data_width = 0.0;
		for (int i = 0; i < m_original_curve_data_map.size(); ++i)
		{
			if (m_original_curve_data_map[i].size() > data_width)
				data_width = float(m_original_curve_data_map[i].size());
		}

		float data_height = max_value - min_value;

		RECT line_chart_rect = this->GetPos();

		m_result_curve_point_map.clear();
		for (int i = 0; i < m_original_curve_data_map.size(); ++i)
		{
			m_result_curve_point_map[i] = std::vector<PointF>();
		}

		// 遍历所有曲线
		for (int i = 0; i < m_original_curve_data_map.size(); ++i)
		{
			std::vector<float> temp_curve_data = m_original_curve_data_map[i];

			// 遍历单条曲线并进行坐标转换
			for (int j = 0; j < temp_curve_data.size(); ++j)
			{
				float temp_value = temp_curve_data[j];
				PointF result_point;

				result_point.X = (j) / data_width * (float)line_chart_width;
				result_point.Y = (float)line_chart_height - ((temp_value - min_value) / data_height * (float)line_chart_height);

				result_point.X += line_chart_rect.left;
				result_point.Y += line_chart_rect.top;

				m_result_curve_point_map[i].emplace_back(result_point);
			}

			// 将数据赋值给数组
			PointF* temp_ptr_point_array = new PointF[m_result_curve_point_map[i].size()];
			for (int k = 0; k < m_result_curve_point_map[i].size(); ++k)
			{
				temp_ptr_point_array[k] = m_result_curve_point_map[i][k];
			}

			// 画曲线
			Pen pen(m_curve_pen_color);

			Graphics graphics(hDC);
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);

			Status restatus = graphics.DrawCurve(&pen, (const PointF*)temp_ptr_point_array, m_result_curve_point_map[i].size());

			// 画点
			//SolidBrush green_brush(Color::Green);
			//for (int k = 0; k < m_result_curve_point_map[i].size(); ++k)
			//{
			//	graphics.FillEllipse(&green_brush, temp_ptr_point_array[k].X - 2.5, temp_ptr_point_array[k].Y - 2.5, 5.0, 5.0);
			//}

			if (temp_ptr_point_array != nullptr)
				delete[] temp_ptr_point_array;
		}

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
		else if (_tcscmp(pstrName, _T("curvenum")) == 0)
		{
			SetCurveNum(_ttoi(pstrValue));
		}
		else
		{
			CControlUI::SetAttribute(pstrName, pstrValue);
		}
	}

	void CLineChartUI::ResetChart()
	{
		m_original_curve_data_map.clear();
		m_result_curve_point_map.clear();

		for (int i = 0; i < m_curve_num; ++i)
		{
			m_original_curve_data_map[i] = std::vector<float>();
			m_result_curve_point_map[i] = std::vector<PointF>();
		}
	}

	void CLineChartUI::AddCurveData(int curve_index, float value)
	{
		if (curve_index > m_original_curve_data_map.size() - 1)
			return;

		if (m_original_curve_data_map[curve_index].size() >= m_max_point_num)
		{
			for (int i = 0; i < m_original_curve_data_map[curve_index].size() - 1; ++i)
			{
				m_original_curve_data_map[curve_index][i] = m_original_curve_data_map[curve_index][i + 1];
			}

			m_original_curve_data_map[curve_index][m_original_curve_data_map[curve_index].size() - 1] = value;
		}
		else
		{
			m_original_curve_data_map[curve_index].emplace_back(value);
		}


		if (m_original_curve_data_map[curve_index].size() == 0)
		{
			min_value = value;
			max_value = value;
		}
		else
		{
			if (value < min_value)
				min_value = value;
			if (value > max_value)
				max_value = value;
		}
	}

	void CLineChartUI::SetCurveData(int curve_index, const std::vector<float>& curve_data)
	{
		if (curve_index > m_original_curve_data_map.size() - 1)
			return;


	}

	void CLineChartUI::SetCurvePenColor(DWORD dwColor)
	{
		m_curve_pen_color = dwColor;
	}

	void CLineChartUI::SetMaxPointNum(unsigned int max_point_num)
	{
		m_max_point_num = max_point_num;
	}

	void CLineChartUI::SetCurveNum(unsigned int curve_num)
	{
		m_curve_num = curve_num;

		m_original_curve_data_map.clear();
		m_result_curve_point_map.clear();

		for (int i = 0; i < m_curve_num; ++i)
		{
			m_original_curve_data_map[i] = std::vector<float>();
			m_result_curve_point_map[i] = std::vector<PointF>();
		}
	}

	int CLineChartUI::GetCurveCount()
	{
		return m_original_curve_data_map.size();;
	}
}