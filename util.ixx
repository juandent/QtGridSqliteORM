module;

#include <string>
#include <debugapi.h>
#include <cassert>
#include <chrono>
#include <array>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <filesystem>
#include <locale>

#include <sstream>
#include <format>
#include <QString>
#include <QVariant>



export module util;

import fixed_point;
import date_as_string;


export namespace util
{
	template<typename T> requires (std::is_arithmetic_v<T>)
	QString to_qstring(T t)
	{
		return std::to_string(t).c_str();
	}

	template<typename T>
	QString to_qstring( const std::optional<T>& opt)
	{
		if(opt)
		{
			return to_qstring(*opt);
		}
		else
		{	// NULL
			return QVariant().toString();
		}
	}

	QString to_qstring(const std::string& str)
	{
		return str.c_str();
	}
	QString to_qstring( std::chrono::sys_days days)
	{
		return format("{:%F}", days).c_str();
	}
}

namespace fs = std::filesystem;



// Sample diagnostics class:
export namespace diag
{
	template<typename T>
	struct TypeName;	// un purpose not defined!

#define OUTPUT_TO_IMMEDIATE_WINDOW
#ifdef OUTPUT_TO_IMMEDIATE_WINDOW
	export template<size_t Size>
		inline void display(const std::array<char, Size>& buffer)
	{
		std::string str;
		for (auto el : buffer)
		{
			if (el == '\0' || el == '\n')
				el = ' ';
			str += el;
		}
		str += "\n\0";
		::OutputDebugStringA(str.c_str());
	}
	export inline void endl()
	{
		::OutputDebugStringA("\n");
	}
	export inline void display(const std::wstring& str)
	{
		::OutputDebugStringW(str.c_str());
		endl();
	}
	export inline void display(const std::string& str)
	{
		::OutputDebugStringA(str.c_str());
		endl();
	}
	export inline void display(std::ifstream& in)
	{
		std::string str = "not_good == ";
		str += (!in.good()) ? "true" : "false";
		display(str);
	}
	export inline void display(const std::vector<char>& buffer)
	{
		std::string str;
		for (auto el : buffer)
		{
			if (el == '\0' || el == '\n')
				el = ' ';
			str += el;
		}
		str += "\0";
		::OutputDebugStringA(str.c_str());
		endl();
	}




	export inline void display(std::chrono::sys_days pt)
	{
		using namespace std::chrono;

		const std::chrono::year_month_day ymd{ pt };
		std::ostringstream os;
		os << ymd;
		display(os.str());
	}

#else
	export template<size_t Size>
		inline void display(const std::array<char, Size>& buffer)
	{}
	export template<typename T>
		export inline void display(const T&) {}
	export inline void display(const std::vector<char>& buffer) {}
	export inline void display(const std::string& str) {}
	export inline void endl() {}
#endif
}

export namespace util
{
	//export using Money = fixed_decimal<2, long long>;

	export class Colones : public Money
	{
		unsigned m_width = 13;
	public:
		using Money::Money;
		inline Colones() : Money(0) {}
		inline Colones(const Money& money)
			: Money(money.rep() / (long double)money.scale_)
		{}
		inline void setWidth(unsigned w) { m_width = w; }
		operator std::string();
	};

	// MFC_UTILITIES_API
	export class Dolares : public Money
	{
		unsigned m_width = 13;
	public:
		using Money::Money;
		inline Dolares() : Money(0) {}
		inline Dolares(const Money& money)
			: Money(money.rep() / (long double)money.scale_)
		{}
		inline void setWidth(unsigned w) { m_width = w; }
		operator std::string();
	};

	// using pointers
	export template <typename T, typename U, typename X = std::remove_pointer<T>::type, typename Y = std::remove_pointer<U>::type,
		typename A = std::enable_if< std::is_base_of<Y, X>::value>::type,
		typename B = std::enable_if<std::is_pointer<T>::value>::type,
		typename C = std::enable_if<std::is_pointer<U>::value>::type
	>
		T  polymorphic_cast(U p)
	{
		assert(dynamic_cast<T>(p));
		return static_cast<T>(p);
	}
	// using references
	export template <typename T, typename U, typename X = std::remove_reference<T>::type, typename Y = std::remove_reference<U>::type,
		typename A = std::enable_if< std::is_base_of<Y, X>::value>::type>
		T& polymorphic_cast(U& p)
	{
		assert(dynamic_cast<T&>(p));
		return static_cast<T&>(p);
	}

	// convert string to uppercase
	export inline std::string touppercase(const std::string& name)
	{
		// pass 'name' to upper case
		std::string upper_name{ name };
		std::transform(name.cbegin(), name.cend(), upper_name.begin(), [](char c) {
			return std::toupper(c, std::locale{});
			});
		return upper_name;
	}

	// convert string to lowercase
	export inline std::string tolowercase(const std::string& name)
	{
		// pass 'name' to upper case
		std::string lower_name{ name };
		std::transform(name.cbegin(), name.cend(), lower_name.begin(), [](char c) {
			return std::tolower(c, std::locale{});
			});
		return lower_name;
	}


	//// convert string to wstring
	//export inline std::wstring to_wstring(const std::string& str, const std::locale& loc = std::locale{})
	//{
	//	std::vector<wchar_t> buf(str.size());
	//	std::use_facet<std::ctype<wchar_t>>(loc).widen(str.data(), str.data() + str.size(), buf.data());

	//	return std::wstring(buf.data(), buf.size());
	//}

	//// convert wstring to string
	//export inline std::string to_string(const std::wstring& str, const std::locale& loc = std::locale{})
	//{
	//	std::vector<char> buf(str.size());
	//	std::use_facet<std::ctype<wchar_t>>(loc).narrow(str.data(), str.data() + str.size(), '?', buf.data());

	//	return std::string(buf.data(), buf.size());
	//}

	//export inline QString to_qstring(std::string msg)
	//{
	//	QString str{ msg.c_str() };
	//	return str;
	//}
#if 0
	export QString to_qstring(double&& d)
	{
		auto d_str = std::to_string(d);
		return to_qstring(d_str);
	}

	export QString to_qstring(long d)
	{
		auto d_str = std::to_string(d);
		return to_qstring(d_str);
	}

	export QString to_qstring(long long d)
	{
		auto d_str = std::to_string(d);
		return to_qstring(d_str);
	}

	export QString to_qstring(std::size_t d)
	{
		auto d_str = std::to_string(d);
		return to_qstring(d_str);
	}

	export QString to_qstring(int d)
	{
		auto d_str = std::to_string(d);
		return to_qstring(d_str);
	}
#endif
#if 1
	export template<typename T> requires std::is_integral<T>::value || std::is_floating_point<T>::value
		QString to_qstring(T && t)
	{
		auto d_str = std::to_string(std::forward<T>(t));
		return to_qstring(d_str);
	}
#endif

	export template<typename...Args>
		QString to_qstring(std::tuple<Args...> tuple)
	{
		auto str = std::get<0>(tuple);
		return to_qstring(str);
	}

	export template<typename T>
		QString to_qstring(std::unique_ptr<T>&& d)
	{
		auto p = d.get();

		if (p)
		{
			auto val = *p;
			return to_qstring(val);
		}
		return "";
	}

	export template<typename T>
		QString unique_ptr_to_qstring(std::unique_ptr<T>&& d)
	{

		auto p = d.get();
		if (p)
		{
			auto val = *p;
			return to_qstring(val);
		}
		return "";
	}


	export inline std::string to_string(std::chrono::sys_days dp)
	{
		//std::chrono::year_month_day ymd{ dp };
		//std::ostringstream os;
		auto str = std::format("{0:%F}", dp);
		//os << ymd;
		//return os.str();
		return str;
	}

	//export inline QString to_qstring(std::chrono::sys_days dp)
	//{
	//	auto str = to_string(dp);
	//	return to_qstring(str);
	//}

	export inline std::string from_QString(const QString& msg)
	{
		return msg.toStdString();
	}
#if 0
	export template<typename T> requires std::is_integral_v<T>
		inline QString to_qstring(T val)
	{
		std::string str = std::to_string(val);
		return to_qstring(str);
	}
#endif
	export inline QString to_qstring(bool val)
	{
		return QString{ val ? "True" : "False" };
	}
#if 0
	export inline QString to_qstring(Colones amount)
	{
		auto tmp = static_cast<std::string>(amount);
		return QString{ tmp.c_str() };
	}

	export inline QString to_qstring(Dolares amount)
	{
		auto tmp = static_cast<std::string>(amount);
		return QString{ tmp.c_str() };
	}
#endif

	export inline std::string as_string(const QString& s)
	{
		std::string ss = from_QString(s);
		if (ss.empty()) ss = "0";
		return ss;
	}

	export inline
		int to_int(const QString& s)
	{
		auto ss = as_string(s);
		return std::stoi(ss);
	}

	export inline
		long to_long(const QString& s)
	{
		auto ss = as_string(s);
		return std::stol(ss);
	}

	export inline
		double to_double(const QString& s)
	{
		auto ss = as_string(s);
		return std::stod(ss);
	}



	export template<typename Element>
		struct get_quote
	{
		static constexpr void const* chars[2] = { "\"", L"\"" };
		static constexpr Element const* quote_char = reinterpret_cast<Element const*>(chars[sizeof(Element) - 1]);
	};


	export template<typename Element >
		inline std::basic_string<Element> quote(const std::basic_string<Element>& str)
	{
		constexpr Element const* quoted_char = get_quote<Element>::quote_char;

		std::basic_string<Element> quoted = quoted_char;
		quoted += str;
		quoted += quoted_char;
		return quoted;
	}

	export struct SingletonsInitializer
	{
		SingletonsInitializer();
		~SingletonsInitializer();
	};

	// jd how does it work? - removed the export
	extern SingletonsInitializer initializer;

	export QString local_to_qstring(Money money, int width = 13);

	export QString dollars_to_qstring(Money money, int width = 13);

	export QString number_to_qstring(Money money, int width = 13);

	export QString to_qstring(Colones colones, int width = 13);

	export QString to_qstring(Dolares dolares, int width = 13);

	export std::string to_string(Colones money, int width = 13);

	export std::string to_string(Dolares money, int width = 13);

	export struct Str
	{
		static Money from_local_QString(const QString& s);

		static Money from_dollars_QString(const QString& s);
	};
#if 0
	export Money from_local_QString(const QString& s);

	export Money from_dollars_QString(const QString& s);
#endif
	export inline Money to_money(const std::string& s)
	{
		auto val = stold(s);
		return Money{ val };
	}

	export inline long double strip_to_long_double(std::string moneyAsString)
	{
		std::string stripped;
		for (auto& c : moneyAsString)
		{
			if (c != '$' && c != '�' && c != ',')
			{
				stripped += c;
			}
		}
		return stold(stripped);
	}

	export inline Money strip_to_money(std::string moneyAsString)
	{
		auto val = strip_to_long_double(moneyAsString);
		return Money{ val };
	}
#if 0
	export inline COleDateTime to_ole_date_time(std::chrono::sys_days fecha)
	{
		using namespace std::chrono;

		year_month_day ymd = fecha;
		auto year_val = static_cast<int>(ymd.year());
		auto month_val = static_cast<unsigned>(ymd.month());
		auto day_val = static_cast<unsigned>(ymd.day());
		COleDateTime rDateTime;
		rDateTime.SetDate(year_val, month_val, day_val);
		return rDateTime;
	}

	export inline std::chrono::sys_days to_sys_days(const COleDateTime& fecha)
	{
		using namespace std::chrono;

		int yearVal = fecha.GetYear();
		unsigned monthVal = fecha.GetMonth();
		unsigned dayVal = fecha.GetDay();

		year_month_day ymd{ year{yearVal}, month{monthVal}, day{dayVal} };
		sys_days date = ymd;
		return date;
	}
#endif

	//export inline std::string convert(const QString& str)
	//{
	//	std::wstring s{ str };
	//	return to_string(s);
	//}

	export inline unsigned lineYear(int lineMonth, unsigned statementMonth, unsigned statementYear)
	{
		assert(statementYear != 0);

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

	export inline std::chrono::sys_days selected_criteria_to_date(const std::string& asText)
	{
		using namespace std;
		using namespace std::chrono;

		smatch m;

		regex regDigitsByDash{ "([[:d:]]{4})[-/]([[:d:]]{2})[-/]([[:d:]]{2})" };

		bool okSecondFormat = regex_search(asText, m, regDigitsByDash);

		if (okSecondFormat)
		{
			auto sDay = m.str(3);
			auto sMonth = m.str(2);
			auto sYear = m.str(1);

			unsigned intDay = stoi(sDay);
			unsigned intMonth = stoi(sMonth);
			unsigned intYear = stoi(sYear);

			year_month_day aDate{ year(intYear), month(intMonth), day(intDay) };
			return aDate;
		}
		return year_month_day{ year{0}, month{0}, day{0} };
	}

	export inline std::chrono::sys_days to_date(const std::string& asText, unsigned statementMonth = 0, int statementYear = 0)
	{
		::date_as_string::StringDateConverter converter{ asText, statementMonth, statementYear };
		auto ret = converter.convert();
		if (!ret.first)
		{
			throw std::logic_error("Cannot convert text to date");
		}
		return ret.second;
	}

	export inline bool is_date(const std::string& asText)
	{
		date_as_string::StringDateConverter converter{ asText, 0, 0 };
		auto ret = converter.convert();
		return ret.first;
	}

	export inline std::string remove_extension(const std::string& fileName)
	{
		//namespace fs = std::filesystem;

		auto x = fs::path(fileName);
		auto y = x.filename();
		auto file = y.string();
		std::string::size_type n = file.find('.');
		// remove extension:
		if (n != std::string::npos)
		{
			file = file.substr(0, n);
		}
		return file;
	}

	export inline QString weekDayQS(const std::chrono::sys_days dp)
	{
		static std::array<QString, 7> days{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		std::chrono::year_month_weekday ymwd{ dp };
		auto wkday = ymwd.weekday();
		int i = (wkday - std::chrono::Sunday).count();
		//auto i = ymwd.weekday_indexed().weekday().operator unsigned int();
		return days[i];
	}

	export inline const std::string& weekDay(const std::chrono::sys_days dp)
	{
		static std::array<std::string, 7> days{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		std::chrono::year_month_weekday ymwd{ dp };
		auto wkday = ymwd.weekday();
		int i = (wkday - std::chrono::Sunday).count();
		//auto i = ymwd.weekday_indexed().weekday().operator unsigned int();
		return days[i];
	}

	export inline LPARAM weekDayAsData(const std::chrono::sys_days dp)
	{
		auto& dayName = weekDay(dp);
		return reinterpret_cast<LPARAM>(dayName.c_str());
	}

	export inline LPARAM stringAsData(const std::string& str)
	{
		return reinterpret_cast<LPARAM>(str.c_str());
	}

	export template<typename T> requires std::is_base_of<std::exception, T>::value
		std::string ProcessCodeException(const T& e)
	{
		using namespace std;

		auto c = e.code();

		ostringstream os;

		os << c.category().name() << " " << c.value() << " " << c.message() << " ";

		return os.str();
	}

	//export struct Comparison
	//{
	//	static std::locale loc;
	//	static std::ostringstream os;

	//	static std::string stripNonDigits(const QString& s);

	//	inline static long double to_number(const std::string& s)
	//	{
	//		return stold(s);
	//	}
	//	inline static long double to_number(const QString& s)
	//	{
	//		auto str = stripNonDigits(s);
	//		return stold(str);
	//	}

	//	static std::pair<CGridCellBase*, CGridCellBase*> translate(LPARAM lpar1, LPARAM lpar2);
	//	static int __stdcall Text(LPARAM lpar1, LPARAM lpar2, LPARAM lpar3);
	//	static int __stdcall Money(LPARAM lpar1, LPARAM lpar2, LPARAM lpar3);
	//};


	// for map or multimap insertion or updating: (Effective STL, pg 110 by Scott Meyers)
	export template< typename MapType,
		typename KeyArgType,
		typename ValueArgType>
		auto efficientAddOrUpdate(MapType& m, const KeyArgType& k, const ValueArgType& v)
	{
		auto lb = m.lower_bound(k);			// find where k is or should be

		if (lb != m.end() && !(m.key_comp()(k, lb->first)))
		{
			lb->second = v;
			return lb;
		}
		else
		{
			using MVT = typename MapType::value_type;
			return m.insert(lb, MVT{ k,v });
		}
	}
	// utility for when pointers, iterators or smart pointers are stored in associative containers (Effective STL, pg 91, Scott Meyers)
	export struct DereferenceLess
	{
		template<typename PtrType>
		bool operator()(PtrType pT1, PtrType pT2) const
		{
			return *pT1 < *pT2;
		}
	};

	export using MoneyPair = std::pair<Money, Money>;
	export inline MoneyPair& operator+=(MoneyPair& lhs, MoneyPair rhs)
	{
		lhs.first += rhs.first;
		lhs.second += rhs.second;
		return lhs;
	}

	export template< typename CharT>
	class Buffer
	{
		CharT* m_pointerToBuffer;
	public:
		Buffer(int num_bytes)
		{
			m_pointerToBuffer = new CharT[num_bytes];
		}
		~Buffer()
		{
			delete[] m_pointerToBuffer;
		}
		operator CharT* () const noexcept
		{
			return m_pointerToBuffer;
		}
	};
}
