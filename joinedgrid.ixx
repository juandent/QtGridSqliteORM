module;

#pragma once
#include <memory>
#include <optional>
#include <QHeaderView>
#include <vector>
#include <string>
#include <QTableWidget>

#include <QTableView>

//#include "pch.h"
//#include <JDGridCtrl.h>


export module joinedgrid;

import idisplayer;
import util;


export
{
	inline double& extractValue(const std::unique_ptr<double>& val)
	{
		static double null_value = 0.0;
		if (val)
		{
			return *val;
		}
		return null_value;
	}
	template<typename T>
	inline T& extractValue(std::optional<T>& opt)
	{
		if (opt)
			return *opt;
		return T{};
	}

	template<typename T>
	inline T& extractValue(T& val)
	{
		return val;
	}


	// template<typename T> colonesList;

	template<typename T, typename ColonesCols, typename DollarsCols>
	class JoinedGridDisplayer : public IDisplayer
	{
		using Container = std::vector<std::remove_reference_t<T>>;
		using RowType = typename Container::value_type;
		inline static constexpr size_t NumCols = std::tuple_size<RowType>::value;
		QTableWidget& grid;
		Container					lines;
		std::vector<std::string>	headers;
	public:
		JoinedGridDisplayer(QTableWidget& grid,
			Container&& lines_,
			std::vector<std::string> headers_)
			: grid{ grid }, lines{ std::move(lines_) },  headers{ std::move(headers_) }
		{
			// reset grid
			grid.setRowCount(0);
			grid.setColumnCount(0);
			// new grid
			grid.setColumnCount(NumCols + 1); // headers.size() + 1);
			grid.setRowCount(lines.size() + 1);
			grid.setSortingEnabled(true);
			//grid.setSelectionBehavior(QAbstractItemView::SelectRows);
			//grid.setSelectionMode(QAbstractItemView::SingleSelection);
			grid.horizontalHeader()->hide();
			grid.verticalHeader()->hide();
			//grid.setEditTriggers();

			/*grid.SetFixedRowCount();
			grid.SetFixedColumnCount();
			grid.SetHeaderSort(true);
			grid.SetSingleRowSelection(true);
			*/
			//grid.

			//grid.m_sortingFunctions.resize(NumCols + 1);
			//grid.m_sortingFunctions[0] = util::Comparison::Text;

			int col = 1;
			for (auto& str : headers)
			{
				grid.setItem(0, col, new QTableWidgetItem{ str.c_str() });
				++col;
			}
			//static_assert(NumCols == MaxCol, "");
		}

		void display() override
		{
			//RECT rect;
			//grid.GetWindowRect(&rect);

			for (int i = 0; i < lines.size(); ++i)
			{
				PrintDataInGrid<0, Container, NumCols>::Apply(i, lines, grid);
			}
			grid.horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

			//grid.SetColumnWidth(0, 100);
			// int width = grid.GetColumnWidth(0);
			//int width{};
			for (int i = 0; i <= headers.size(); ++i)
			{
				grid.resizeColumnToContents(i + 1);
				//width += grid.GetColumnWidth(i);
			}
			//auto frame = grid.GetParentFrame();
			//width += GetSystemMetrics(SM_CXVSCROLL);
			//grid.SetWindowPos(frame, 10, 10, width, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);
		}
		const Container& getLines() const
		{
			return lines;
		}
	private:
		template<int Col, typename FieldType, typename T>
		static QString FormatCol(T& value, QTableWidget& grid)
		{
			QString cs;
			if constexpr ((std::is_integral_v<FieldType> || std::is_floating_point_v<FieldType>) && !std::is_same_v<std::decay<FieldType>::type, bool>)
			{
				//grid.m_sortingFunctions[Col + 1] = util::Comparison::Money;
#if 1
				if (ColonesCols::template found<Col + 1>())
				{
					util::Colones c(value);
					cs = format(c);
				}
				else if (DollarsCols::template found<Col + 1>())
				{
					util::Dolares d(value);
					cs = format(d);
				}
				else
				{
					cs = format(value);
				}
#else
				cs = format(value);
#endif
			}
			else
			{
				//grid.m_sortingFunctions[Col + 1] = util::Comparison::Text;
				cs = format(value);
			}
			return cs;
		}
		template<int Col, typename Container, unsigned NumCols>
		struct PrintDataInGrid
		{
			static void Apply(int row, const Container& z, QTableWidget & grid)
			{
				QString cs;

				using FieldType = std::remove_reference_t<decltype(std::get<Col>(z[row]))>;
				FieldType* pT = nullptr;
				if constexpr (std::is_same_v<FieldType, const std::unique_ptr<double>>)
				{
					auto&& value = extractValue(std::move(std::get<Col>(z[row])));
					cs = FormatCol<Col, double>(value, grid);
				}
				else
				{
					auto&& value = std::get<Col>(z[row]);
					// auto val = extractValue(value);	// JDH

					cs = FormatCol<Col, FieldType>(value, grid);
				}
				//grid.SetItemText(row + 1, Col + 1, cs);
				grid.setItem(row+1, Col+1, new QTableWidgetItem{ cs});
				PrintDataInGrid<Col + 1, Container, NumCols>::Apply(row, z, grid);
			}
		};

		template<typename Container>
		struct PrintDataInGrid<NumCols, Container, NumCols>
		{
			static void Apply(int row, const Container& z, QTableWidget& grid)
			{
			}
		};


		template<typename T>
		static QString format(T&& t)
		{
			static_assert(std::is_same_v<decltype(t), T>);
			return util::to_qstring(std::forward<T>(t));
		}

		template<typename T>
		static QString format(const std::shared_ptr<T>& t)
		{
			if (t)
			{
				return util::to_qstring(*t);
			}
			return "";
		}

		template<typename T>
		static QString format(const std::unique_ptr<T>& t)
		{
			if (t)
			{
				return util::to_qstring(*t);
			}
			return "";
		}

		template<typename T>
		static QString format(const std::optional<T>& t)
		{
			if (t)
			{
				return util::to_qstring(*t);
			}
			return "";
		}

	};
}

