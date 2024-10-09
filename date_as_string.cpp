module;
//#include "pch.h"

#include <array>
#include <string>
#include <utility>
#include <locale>
#include <algorithm>
#include <chrono>
#include <map>
#include <regex>



module date_as_string;


using namespace std;
using namespace std::chrono;
using namespace std::literals::string_literals;

namespace date_as_string
{
	// MonthNames:

	array<string, 12> MonthNames::s_monthNames{ "ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DIC" };

	std::pair<bool, unsigned> MonthNames::getMonthNumber(const string& name)
	{
		// pass 'name' to upper case
		string upper_name{ name };
		transform(name.cbegin(), name.cend(), upper_name.begin(), [](char c) {
			return std::toupper(c, std::locale{});
			});

		auto pos = find_if(s_monthNames.cbegin(), s_monthNames.cend(), [upper_name = upper_name](const string& monthName)
			{
				return upper_name == monthName;
			});
		if (pos == s_monthNames.cend())
		{
			return std::make_pair(false, 0);
		}
		auto number = static_cast<unsigned>(distance(s_monthNames.cbegin(), pos) + 1);
		return std::make_pair(true, number);
	}

	// CompleteDateExtractor...
	std::pair<bool, std::chrono::sys_days> CompleteDateExtractor::convert(const std::string& asText) const
	{
		using namespace std::chrono;
		std::smatch m;

		bool ok = regex_search(asText, m, m_regularExpression);

		if (ok)
		{
			auto sDay = m.str(m_dayIndex);
			auto sMonth = m.str(m_monthIndex);
			auto sYear = m.str(m_yearIndex);

			unsigned intDay = stoi(sDay);
			unsigned intMonth = stoi(sMonth);
			unsigned intYear = stoi(sYear);

			year_month_day aDate{ year(intYear), month(intMonth), day(intDay) };
			return std::make_pair(true, aDate);
		}
		return std::make_pair(false, sys_days{});
	}

	inline int yearFromLineMonthAndStatement(unsigned lineMonth, unsigned statementMonth, unsigned statementYear)
	{
		if (lineMonth == statementMonth)
		{
			return statementYear;
		}
		else if (statementMonth == 1)
		{
			return statementYear - 1;
		}
		else
		{
			return statementYear;
		}
	}


	// we try to see if month is alphabetic, and if not then it is converted to numeric
	std::pair<bool, std::chrono::sys_days> IncompleteDateExtractor::convert(const std::string& asText) const
	{
		using namespace std::chrono;
		std::smatch m;

		bool ok = regex_search(asText, m, m_regularExpression);

		if (ok)
		{
			auto sDay = m.str(m_dayIndex);
			auto sMonth = m.str(m_monthIndex);

			unsigned intDay = stoi(sDay);
			unsigned intMonth;
			auto monthPair = MonthNames::getMonthNumber(sMonth);
			if (!monthPair.first)
			{
				intMonth = stoi(sMonth);
			}
			else
			{
				intMonth = monthPair.second;
			}
			auto intYear = yearFromLineMonthAndStatement(intMonth, m_statementMonth, m_statementYear);

			year_month_day aDate{ year(intYear), month(intMonth), day(intDay) };
			return std::make_pair(true, aDate);
		}
		return std::make_pair(false, sys_days{});
	}

	bool isDateComplete(const std::string& asText)
	{
		auto numSeparators = count_if(asText.cbegin(), asText.cend(), [](auto elem) {

			return elem == '/' || elem == '-' || elem == '+';
			});
		return numSeparators >= 2;
	}

	std::map<std::string, std::array<int, 3>>	StringDateConverter::s_expressions_for_complete_dates = {
		{ "([[:d:]]+)[/-]([[:d:]]+)[/-]([[:d:]]{4})"s, {1,2,3} }, 														// dd/mm/yyyy or dd-mm-yyyy
		{ "([[:d:]]{4})[+/-]([[:d:]]{1,2})[+/-]([[:d:]]{1,2})", {3,2,1} }									// yyyy-mm-dd or yyyy/mm/dd or yyyy+mm+dd
	};

	std::map<std::string, std::array<int, 2>>	StringDateConverter::s_expressions_for_incomplete_dates = {
		{ "([[:d:]]+)[/-]([[:alpha:]]+)", {1,2} },																			// dd-MONTH or dd/MONTH
		{ "([[:alpha:]]+)[/-]([[:d:]]+)", {2,1} }																			// MONTH-dd or MONTH/dd
	};

	std::pair<bool, std::chrono::sys_days>  StringDateConverter::convert() const
	{
		if (isDateComplete(m_asText))
		{
			for (auto& exp : s_expressions_for_complete_dates)
			{
				CompleteDateExtractor extractor{ exp.first, exp.second[0], exp.second[1], exp.second[2] };
				auto ret = extractor.convert(m_asText);
				if (ret.first)
				{
					return ret;
				}
			}
		}
		else
		{
			for (auto& exp : s_expressions_for_incomplete_dates)
			{
				IncompleteDateExtractor extractor{ exp.first, exp.second[0], exp.second[1], m_statementMonth, m_statementYear };
				auto ret = extractor.convert(m_asText);
				if (ret.first)
				{
					return ret;
				}
			}
		}
		return make_pair(false, std::chrono::sys_days{});
		//throw logic_error("Cannot convert text to date");
	}

}
