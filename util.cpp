//#include "pch.h"
module;
//
//#include <afx.h>
#include <QString>
#include <locale>
#include <iomanip>
#include <sstream>

//#include <afxwin.h>
//#include "GridCellBase.h"


module util;


using namespace std;


namespace util
{
	// to deal with unneeded positive sign!!
	class costaRicaNumPunct : public moneypunct_byname<char>
	{
	public:
		explicit costaRicaNumPunct(const std::string& name)
			: std::moneypunct_byname<char>("en-US")      ///   "sp-CR")
		{}
	protected:
		virtual std::string do_positive_sign() const override {
			return "";
		}
		virtual std::string do_curr_symbol() const override {
			return "�";
		}
		virtual int do_frac_digits() const override
		{
			return 2;
		}
		virtual char_type do_thousands_sep() const override {
			return ',';
		}
		virtual char_type do_decimal_point() const override {
			return '.';
		}
	};


	//string Comparison::stripNonDigits(const QString& s)
	//{
	//	os.str("");
	//	int count = 0;
	//	for (int i = 0; i < s.GetLength(); ++i)
	//	{
	//		auto c = s.GetAt(i);
	//		if (c == '-' || c == '.' || std::isdigit(c, loc))
	//		{
	//			os << static_cast<char>(c);
	//			count++;
	//		}
	//	}
	//	if (count == 0)
	//	{
	//		return "0"s;
	//	}
	//	return os.str();
	//}

	//int __stdcall Comparison::Text(LPARAM lpar1, LPARAM lpar2, LPARAM lpar3)
	//{
	//	auto cells = translate(lpar1, lpar2);
	//	if (cells.first == nullptr)  return 0;
	//	return _tcscmp(cells.first->GetText(), cells.second->GetText());
	//}

	//int __stdcall Comparison::Money(LPARAM lpar1, LPARAM lpar2, LPARAM lpar3)
	//{
	//	auto cells = translate(lpar1, lpar2);
	//	if (cells.first == nullptr)  return 0;

	//	auto nValue1 = to_number(cells.first->GetText());
	//	auto nValue2 = to_number(cells.second->GetText());

	//	if (nValue1 < nValue2)
	//		return -1;
	//	else if (nValue1 == nValue2)
	//		return 0;
	//	else
	//		return 1;
	//}

	//pair<CGridCellBase*, CGridCellBase*> Comparison::translate(LPARAM lpar1, LPARAM lpar2)
	//{
	//	CGridCellBase* pCell1 = reinterpret_cast<CGridCellBase*>(lpar1);
	//	CGridCellBase* pCell2 = reinterpret_cast<CGridCellBase*>(lpar2);
	//	if (!pCell1 || !pCell2) return make_pair(nullptr, nullptr);
	//	else return make_pair(pCell1, pCell2);
	//}


	//locale Comparison::loc;
	//ostringstream Comparison::os;

#define PLAYING_WITH_LOCALES

	struct MoneytaryHelper
	{
	private:
		costaRicaNumPunct* m_posSignEliminator;
		locale m_usLoc;
		locale m_crLoc;
		ostringstream m_usOs;
		ostringstream m_crOs;
		istringstream m_usIs;
		istringstream m_crIs;

		const money_put<char>& m_usMoney_put;
		const money_put<char>& m_crMoney_put;

		const num_put<char>& m_usNumber_put;
		const num_put<char>& m_crNumber_put;
	public:
		MoneytaryHelper()
			: m_posSignEliminator{ new costaRicaNumPunct{ "" } },
#ifndef PLAYING_WITH_LOCALES
			m_usLoc{ "en-US" }, m_crLoc{ locale{"sp-ES"}.combine<numpunct<char>>(locale{"sp-CR"}), //, m_posSignEliminator
			},
#else
			m_usLoc{ "en-US" }, m_crLoc{ m_usLoc, m_posSignEliminator },
#endif
			m_usMoney_put{ use_facet<money_put<char>>(m_usLoc) }, m_crMoney_put{ use_facet<money_put<char>>(m_crLoc) },
			m_usNumber_put{ use_facet<num_put<char>>(m_usLoc) }, m_crNumber_put{ use_facet<num_put<char>>(m_crLoc) }
		{
			m_usOs << showbase;
			m_usOs.imbue(m_usLoc);

			m_crOs << showbase;
			m_crOs.imbue(m_crLoc);

			m_usIs.imbue(m_usLoc);
			m_crIs.imbue(m_crLoc);


#ifdef PLAYING_WITH_LOCALES
			//m_crLoc = m_crLoc.combine<numpunct<char>>(locale{"sp-ES"});
			//m_crOs.imbue(m_crLoc);
			//m_crIs.imbue(m_crLoc);
#endif
		}
		string putNumberAsCR(Money money, int width)
		{
			m_crOs.str("");
			m_crOs << right << setw(width);
			auto value = money.rep() / (long double)money.scale_;
			m_crNumber_put.put(m_crOs, m_crIs, ' ', value);
			auto temp = m_crOs.str();
			return temp;
		}
		string putColones(Money money, int width)
		{
			m_crOs.str("");
			m_crOs << right << setw(width);
#if 0	// plain
			auto temp = std::to_string(money.getAsLongDouble());
#else
			m_crMoney_put.put(m_crOs, false, m_crOs, ' ', 100 * money.getAsLongDouble()); //   money.rep()
			auto temp = m_crOs.str();
#endif
			return temp;
		}
		string putDollars(Money money, int width)
		{
			m_usOs.str("");
			m_usOs << right << setw(width);
#if 0 // plain
			auto temp = std::to_string(money.getAsLongDouble());
#else
			m_usMoney_put.put(m_usOs, false, m_usOs, ' ', 100 * money.getAsLongDouble());	// money.rep());
			auto temp = m_usOs.str();
#endif
			return temp;
		}
		Money getColones(const std::string& asColones)
		{
			m_crIs.str(asColones);
			auto readVal = 0.0L;
			m_crIs >> get_money(readVal);
			return Money(readVal);
		}
		Money getDollars(const std::string& asDollars)
		{
			m_crIs.str(asDollars);
			auto readVal = 0.0L;
			m_crIs >> get_money(readVal);
			return Money(readVal);
		}
	};

	namespace
	{
		int s_NiftyCounterForSingletons = 0;
		MoneytaryHelper* moneyHelper = nullptr;
		SingletonsInitializer initializer{};
	}


	SingletonsInitializer::SingletonsInitializer()
	{
		if (s_NiftyCounterForSingletons++ == 0)
		{
			moneyHelper = new MoneytaryHelper{};
		}
	}

	SingletonsInitializer::~SingletonsInitializer()
	{
		if (--s_NiftyCounterForSingletons == 0)
		{
			delete moneyHelper;
		}
	}

	QString number_to_qstring(Money money, int width)
	{
		auto temp = moneyHelper->putNumberAsCR(money, width);
		return to_qstring(temp);
	}

	QString local_to_qstring(Money money, int width)
	{
		auto temp = moneyHelper->putColones(money, width);
		return to_qstring(temp);
	}

	QString dollars_to_qstring(Money money, int width)
	{
		auto temp = moneyHelper->putDollars(money, width);
		return to_qstring(temp);
	}

	QString to_qstring(Colones colones, int width)
	{
		return local_to_qstring(colones, width);
	}
	QString to_qstring(Dolares dolares, int width)
	{
		return dollars_to_qstring(dolares, width);
	}
	std::string to_string(Colones money, int width)
	{
		return moneyHelper->putColones(money, width);
	}

	std::string to_string(Dolares money, int width)
	{
		return moneyHelper->putDollars(money, width);
	}


	///

	//std::string to_string(Dolares money, int width)
	//{
	//	auto temp = moneyHelper->putDollars(money, width);
	//	return temp;
	//}

	//std::wstring to_wstring(Dolares money, int width)
	//{
	//	auto temp = to_string(money, width);
	//	return to_wstring(temp);
	//}

	//std::string to_string(Colones money, int width)
	//{
	//	auto temp = moneyHelper->putColones(money, width);
	//	return temp;
	//}

	//std::wstring to_wstring(Colones money, int width)
	//{
	//	auto temp = to_string(money, width);
	//	return to_wstring(temp);
	//}

	// std::string to_string(Colones money, int width)
	// {
	// 	auto temp = moneyHelper->putColones(money, width);
	// 	return temp;
	// }
	//
	// std::wstring to_wstring(Colones money, int width)
	// {
	// 	// auto temp = to_string(money, width);
	// 	return util::to_wstring(money, width);
	// 	// return to_wstring(temp);
	// }


	Colones::operator std::string()
	{
		return moneyHelper->putColones(*this, m_width);
		//return to_string(*this, m_width);
	}

	//Colones::operator std::wstring()
	//{
	//	return to_wstring(*this, m_width);
	//}
	Dolares::operator std::string()
	{
		return moneyHelper->putDollars(*this, m_width);
		//return to_string(*this, m_width);
	}

	//Dolares::operator std::wstring()
	//{
	//	return to_wstring(*this, m_width);
	//}

	Money Str::from_local_QString(const QString& s)
	{
		auto str =   from_QString(s);
		auto temp = moneyHelper->getColones(str);
		return temp;
	}

	Money Str::from_dollars_QString(const QString& s)
	{
		auto str = from_QString(s);
		auto temp = moneyHelper->getDollars(str);
		return temp;
	}

};
