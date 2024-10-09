module;
#pragma once
//#include "pch.h"
#include <QString>
#include <optional>


export module idisplayer;


export
{

	// template<typename ... DataTypes>
	struct IDisplayer
	{
		virtual void display() = 0;
		virtual ~IDisplayer() = default;
	};

	namespace util
	{
		inline QString to_qstring(const std::optional<int>& o)
		{
			if (o)
			{
				return to_qstring(*o);
			}
			else
			{
				return QString{ "" };
			}
		}
	}



	template<int N>
	struct Integral
	{
		static constexpr int Int = N;
	};


	template<int Value, typename List, int Index>
	struct Find
	{
		constexpr static bool same = std::get<Index>(List::Values).Int == Value;
		constexpr static bool Check()
		{
			// if same, found it - end recursion
			return same || Find<Value, List, Index - 1>::Check();	// not found at Index, try at Index - 1
		}
	};

	template<int Value, typename List>
	struct Find<Value, List, 0>
	{
		constexpr static bool same = std::get<0>(List::Values).Int == Value;
		constexpr static bool Check()
		{
			return same;
		}
	};


	/// <summary>
	/// is Value in values list?
	/// </summary>
	template<bool isColones, int...N>
	struct IntegerList
	{
		constexpr inline static std::tuple<Integral<N>...> Values;
		constexpr static unsigned Count = std::tuple_size_v<decltype(Values)>;

		template<int Value>
		constexpr static bool found()
		{
			using lista = IntegerList;

			bool found = Find<Value, lista, lista::Count - 1>::Check();
			return found;
		}
	};

	template<bool isColones>
	struct IntegerList<isColones>
	{
		template<int Value>
		constexpr static bool found()
		{
			return false;
		}
	};


	template<int...N>
	using ColonesCols = IntegerList<true, N...>;

	template<int...N>
	using DollarsCols = IntegerList<false, N...>;

}

