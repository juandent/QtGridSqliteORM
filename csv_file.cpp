module;


//#include "pch.h"
//#include <afx.h>
#include <cassert>

#include <Windows.h>
#ifdef max
#undef max
#undef min
#endif

#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <chrono>


module csv_file;


import fixed_point;
import util;


using namespace std;
using namespace std::chrono;
using namespace util;

namespace csv
{

	CSV_File::CSV_File()
	{
	}

	// load from file into matrix
	void CSV_File::load(const std::string& fileName)
	{
		ifstream in{ fileName, ios_base::_Nocreate };

		if (!in)
		{
			ostringstream os;
			os << "Cannot open file '" << fileName << "'";
			throw ios_base::failure{ os.str() };
		}

		//char buffer[100'000];

		string strBuffer;

		int rowNumber = 0;
		m_matrix.resize(0);
		in.seekg(ios::beg);
		getline(in, strBuffer);
		//in.getline(buffer, sizeof buffer);

		while (in.good())
		{
			vector<string> columns;
			//string strBuffer{ buffer };
			diag::display(strBuffer);


			regex sep("[ \t\n]*[,][ \t\n]*");  // separated by , and spaces

			sregex_token_iterator p(strBuffer.cbegin(), strBuffer.cend(),  // sequence
				sep,														// separator
				-1);														// -1: values between separators

			sregex_token_iterator e;
			for (; p != e; ++p) {
				auto str = p->str();
				diag::display(str);
				if (str[0] == '"')
				{
					//str = str.substr(1);	// skip the double quote '"'
					do {
						++p;
						str += "," + p->str();
						diag::display(str);
						//auto pos_of_double_quote = str.find("\"");
						auto lastCharIter = str.rbegin();
						if (*lastCharIter == '"')	// if last char is the closing double quote
						{
							//*lastCharIter = '\0';
							diag::display(str);
							break;
						}
					} while (true);
				}
				columns.push_back(str);
			}

			m_matrix.push_back(columns);
			// read next line
			// in.getline(buffer, sizeof buffer);
			getline(in, strBuffer);
			rowNumber++;
		}
	}

	void CSV_File::saveAs(std::string outputFileName)
	{
		ofstream out;
		out.exceptions(ios::failbit | ios::badbit);

		try
		{
			out.open( /*"5555-****"*/ outputFileName, ios_base::trunc);
		}
		catch (ios_base::failure& exc)
		{
			string msg = util::ProcessCodeException(exc);

			auto err = ::GetLastError();
			char buff[1000];
			strerror_s(buff, err);

			ostringstream os;
			os << "Cannot open file '" << outputFileName << "' " << buff;
			throw ios_base::failure{ os.str() };
		}

		for (auto&& r = m_matrix.begin(); r != m_matrix.end(); ++r)
		{
			for (auto&& c = r->begin(); c != r->end(); ++c)
			{
				out << *c;
				if (next(c) != r->end())
				{
					out << ",";
				}
			}
			out << endl;
		}
	}

	std::string CSV_File::getString(const Cell_Position& pos) const
	{
		return m_matrix[pos.getRow()][pos.getCol()];
	}

	std::chrono::sys_days CSV_File::getDate(const Cell_Position& pos) const
	{
		auto text = m_matrix[pos.getRow()][pos.getCol()];
		return util::to_date(text);
	}

	std::chrono::sys_days CSV_File::getUSDate(const Cell_Position& pos) const
	{
		std::string text = m_matrix[pos.getRow()][pos.getCol()];
		std::string inverted_text;

		//		std::regex reg{ "(\d+)/(\d+)/(\d+)" };

		std::regex reg{ "([[:d:]]{1,2})[-/]([[:d:]]{1,2})[-/]([[:d:]]{2,4})" };


		std::smatch m;
		if (std::regex_search(text, m, reg))
		{
			auto month = m[1].str();
			auto day = m[2].str();
			auto year = m[3].str();
			inverted_text = day + "/" + month + "/" + year;
		}
		else
		{
			throw std::exception{ "Error inviertendo fecha" };
		}

		return util::to_date(inverted_text);
	}

	Money CSV_File::getMoney(const Cell_Position& pos) const
	{
		auto text = m_matrix[pos.getRow()][pos.getCol()];
		return util::to_money(text);		// makes negative values deposits
	}

	bool CSV_File::getBool(const Cell_Position& pos) const
	{
		auto text = m_matrix[pos.getRow()][pos.getCol()];
		int number = stoi(text);
		return number != 0;
	}

	CSV_File::Row CSV_File::getRow(std::size_t rowNumber) const
	{
		return m_matrix[rowNumber];
	}

	CSV_File& CSV_File::operator<<(const std::string& col)
	{
		addCol(col);
		return *this;
	}

	CSV_File& CSV_File::operator<<(std::chrono::sys_days pt)
	{
		addCol(pt);
		return *this;
	}

	CSV_File& CSV_File::operator<<(Money money)
	{
		addCol(money);
		return *this;
	}

	CSV_File& CSV_File::operator<<(Manipulator func)
	{
		Manipulator m = std::endl;
		if (func == m)
		{
			addEndl();
			return *this;
		}
		assert(!"Another manipulator passed");
		return *this;
	}

	void CSV_File::addCol(const std::string& col)
	{
		m_currentRow.push_back(col);
	}

	void CSV_File::addCol(std::chrono::sys_days pt)
	{
#define USING_PLUS_FOR_DATE
#ifdef USING_PLUS_FOR_DATE
		year_month_day ymd{ pt };
		ostringstream os;
		// to enable correct importing into FileMaker Pro
		os << static_cast<int>(ymd.year());
		os << "+";
		os << static_cast<unsigned>(ymd.month());
		os << "+";
		os << static_cast<unsigned>(ymd.day());
		//os << ymd;
		addCol(os.str());
#else	// dd/mm/yyyy
		date::year_month_day ymd{ pt };
		ostringstream os;

		os.fill('0');
		os.width(2);
		os.flags(std::ios::dec | std::ios::right);
		os << ymd.day() << "/";
		os.width(2);
		os << static_cast<unsigned>(ymd.month()) << '/';
		os.width(4);
		os << ymd.year();
		addCol(os.str());
#endif
	}

	void CSV_File::addCol(Money money)
	{
		ostringstream os;
		os << money;
		addCol(os.str());
	}

	void CSV_File::addEndl()
	{
		m_matrix.push_back(m_currentRow);
		m_currentRow.clear();
	}

	std::string CSV_File::getText(int row, int col) const noexcept
	{
		return m_matrix[row][col];
	}

	size_t CSV_File::getRowCount() const noexcept
	{
		return m_matrix.size();
	}
	// assume all rows have same amount of columns!
	size_t CSV_File::getColCount() const noexcept
	{
		return m_matrix[0].size();
	}
}

