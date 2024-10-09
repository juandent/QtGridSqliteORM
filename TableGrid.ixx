module;

#include <memory>
#include <optional>
#include <QHeaderView>
#include <vector>
#include <string>
#include <QTableWidget>



export module TableGrid;

import util;

export
{

	template<typename T>
	class GridDisplayer
	{
		QTableWidget& grid;
		using Container = std::vector<std::remove_reference_t<T>>;
		Container lines;
		using ValueType = typename Container::value_type;

		// std::tuple<DataTypes...> tuple;
		std::vector<std::string>	headers;
	public:
		// template<typename ...DataTypes>
		GridDisplayer(QTableWidget& grid,
			Container&& lines_,
			std::vector<std::string>&& headers_)
			: grid{ grid }, lines{ std::move(lines_) },  headers{ std::move(headers_) }
		{
			grid.setColumnCount(headers.size());
			grid.setRowCount(lines.size() + 1);
			grid.setSortingEnabled(true);
			grid.setSelectionBehavior(QAbstractItemView::SelectRows);
			grid.setSelectionMode(QAbstractItemView::SingleSelection);
			grid.horizontalHeader()->hide();
			//grid.verticalHeader()->hide();

			/*grid.SetFixedRowCount();
			grid.SetFixedColumnCount();
			grid.SetHeaderSort(true);
			grid.SetSingleRowSelection(true);*/

			int col = 0;
			for (auto& str : headers)
			{
				grid.setItem(0, col, new QTableWidgetItem{ str.c_str() });
				++col;
			}
		}

		template<typename ...DataTypes>
		void display(DataTypes... dts)
		{
			//RECT rect;
			//grid.GetWindowRect(&rect);

			for (int i = 0; i < lines.size(); ++i)
			{
				printDataInGrid(i, 0, lines[i], dts...);
			}
			grid.horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

			//grid.SetColumnWidth(0, 100);
			//int width = grid.GetColumnWidth(0);
			for (int i = 0; i < headers.size(); ++i)
			{
				grid.resizeColumnToContents(i + 1); //  AutoSizeColumn(i + 1);	// skip vertical headers
				//width += grid.GetColumnWidth(i + 1);
			}
			//auto frame = grid.GetParentFrame();
			//width += GetSystemMetrics(SM_CXVSCROLL);
			//grid.SetWindowPos(frame, 10, 10, width, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);
		}

	private:
		template<typename DataType>
		void printDataInGrid(int row, int col, T& z, DataType p)
		{
			auto value = z.*p;
			auto cs = format(value);
			grid.setItem(row+1, col, new QTableWidgetItem{ cs });
			//grid.SetItemText(row + 1, col, cs);
		}


		template<typename DataType, typename ... RestDataTypes>
		void printDataInGrid(int row, int col, T& z, DataType p, RestDataTypes...qs)
		{
			printDataInGrid(row, col, z, p);
			printDataInGrid(row, ++col, z, qs...);
		}
		template<typename T>
		QString format(const T& t)
		{
			return util::to_qstring(t);
		}
		template<typename T>
		QString format(const std::shared_ptr<T>& t)
		{
			if (t)
			{
				return util::to_qstring(*t);
			}
			return "";
		}
	};
}
