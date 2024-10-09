module;


#include <any>
#include <variant>
#include <optional>
#include <QObject>
#include <QAbstractTableModel>
#include <qsortfilterproxymodel.h>
#include <QDateTime>
#include <QTableView>
#include <tuple>
#include <array>
#include <sqlite_orm/sqlite_orm.h>

export module RowProcessor;

using namespace sqlite_orm;

import idisplayer;
import util;

export
{
	struct Null {};


	using all_variant = std::variant<int, std::optional<int>, double, std::optional<double>, std::string, std::optional<std::string >, std::chrono::sys_days, std::optional<std::chrono::sys_days>,
		std::vector<char>, std::optional<std::vector<char>>, nullptr_t>;

	struct RowProcessor
	{
		template<typename T>
		static QVariant extract(T val)
		{
			return val;
		}
		static QVariant extract(const std::vector<char>& vec)
		{
			QByteArray bytes(vec.data(), vec.size());
			QVariant var{ bytes };
			return var;
		}
		static QVariant extract(const std::string& str)
		{
			QVariant qstr{ str.c_str() };
			return qstr;
		}
		static QVariant extract(const std::chrono::sys_days& val)
		{
			QString str = std::format("{:%F}", val).c_str();
			QVariant dt = QDate::fromString(str, Qt::ISODate);
			auto ss = dt.toDate().toString();
			return dt;
		}
		template<typename T>
		static QVariant extract(const std::unique_ptr<T>& val)
		{
			if (val)
				return extract(*val);
			return QVariant{};
		}
		template<typename T>
		static QVariant extract(const std::shared_ptr<T>& val)
		{
			if (val)
				return extract(*val);
			return QVariant{};
		}

		template<typename T>
		static QVariant extract(const std::optional<T>& val)
		{
			if (val)
				return extract(*val);
			return QVariant{};
		}
		template<typename T>
		struct field_type
		{
			using type = T;
		};

		template<typename T>
		struct field_type<std::optional<T>>
		{
			using type = T;
		};

		template<int Col, typename Container>
		static all_variant RawRead(int row, const Container& z)
		{
			auto&& val = std::get<Col>(z[row]);
			using Type = field_type<typename std::remove_reference<decltype(val)>::type>::type;
			Type* tt = nullptr;
			using RawType = std::decay_t<Type>;
			RawType* rt;

			all_variant all{ val };
			RawType v = std::get<RawType>(all);
			auto pos = all.index();

			if constexpr (std::is_same<Type, double>::value)
			{
				int x = 0;

			}
			return all;
		}

		template<int Col, typename Container>
		static QVariant Read(int row, const Container& z)
		{
			//using FieldType = std::remove_reference_t<decltype(std::get<Col>(z[row]))>;
			//FieldType* p = nullptr;


			auto&& val = std::get<Col>(z[row]);
			using Type = field_type<typename std::remove_reference<decltype(val)>::type>::type;
			Type* tt = nullptr;
			using RawType = std::decay_t<Type>;
			RawType* rt;

			all_variant all{ val };
			RawType v = std::get<RawType>(all);
			auto pos = all.index();

			if constexpr (std::is_same<Type, double>::value)
			{
				int x = 0;

			}
			return extract(val);
		}

		template<int Col, typename Container>
		static void Write(int row, Container& z, const QVariant& value)
		{
			auto&& val = std::get<Col>(z[row]);
			using Type = field_type<std::decay_t<decltype(val)>>::type;
			Type* tt = nullptr;

			if constexpr (std::is_same<Type, double>::value)
			{
				std::get<Col>(z[row]) = value.toDouble();
			}
			if constexpr (std::is_same<Type, std::string>::value)
			{
				QString str = value.toString();
				std::string s = str.toStdString();
				std::get<Col>(z[row]) = s;
			}
			if constexpr (std::is_same<Type, int>::value)
			{
				std::get<Col>(z[row]) = value.toInt();
			}
		}
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

	template<typename Container >
	all_variant raw_read(Container& z, QModelIndex index) {
		auto resultRow = z[index.row()];
		constexpr int TupleSize = std::tuple_size_v<decltype(resultRow)>;

		switch (index.column()) {
		case 0:
			if constexpr (TupleSize > 0)
				return RowProcessor::RawRead<0>(index.row(), z);
			break;
		case 1:
			if constexpr (TupleSize > 1)
				return RowProcessor::RawRead<1>(index.row(), z);

			break;
		case 2:
			if constexpr (TupleSize > 2)
				return RowProcessor::RawRead<2>(index.row(), z);

			break;
		case 3:
			if constexpr (TupleSize > 3)
				return RowProcessor::RawRead<3>(index.row(), z);

			break;
		case 4:
			if constexpr (TupleSize > 4)
				return RowProcessor::RawRead<4>(index.row(), z);
			break;
		case 5:
			if constexpr (TupleSize > 5)
				return RowProcessor::RawRead<5>(index.row(), z);
			break;
		case 6:
			if constexpr (TupleSize > 6)
				return RowProcessor::RawRead<6>(index.row(), z);
			break;
		case 7:
			if constexpr (TupleSize > 7)
				return RowProcessor::RawRead<7>(index.row(), z);
			break;
		case 8:
			if constexpr (TupleSize > 8)
				return RowProcessor::RawRead<8>(index.row(), z);
			break;
		case 9:
			if constexpr (TupleSize > 9)
				return RowProcessor::RawRead<9>(index.row(), z);
			break;
		case 10:
			if constexpr (TupleSize > 10)
				return RowProcessor::RawRead<10>(index.row(), z);
			break;
		case 11:
			if constexpr (TupleSize > 11)
				return RowProcessor::RawRead<11>(index.row(), z);
			break;
		case 12:
			if constexpr (TupleSize > 12)
				return RowProcessor::RawRead<12>(index.row(), z);
			break;
		case 13:
			if constexpr (TupleSize > 13)
				return RowProcessor::RawRead<13>(index.row(), z);
			break;
		case 14:
			if constexpr (TupleSize > 14)
				return RowProcessor::RawRead<14>(index.row(), z);
			break;
		case 15:
			if constexpr (TupleSize > 15)
				return RowProcessor::RawRead<15>(index.row(), z);
			break;
		case 16:
			if constexpr (TupleSize > 16)
				return RowProcessor::RawRead<16>(index.row(), z);
			break;
		default:
			throw std::out_of_range(std::format("{0:} too large!", TupleSize));
		}

		return all_variant{ nullptr };
	}


	template<typename Container >
	QVariant read(Container& z, QModelIndex index) {
		auto resultRow = z[index.row()];
		constexpr int TupleSize = std::tuple_size_v<decltype(resultRow)>;

		switch (index.column()) {
		case 0:
			if constexpr (TupleSize > 0)
				return RowProcessor::Read<0>(index.row(), z);
			break;
		case 1:
			if constexpr (TupleSize > 1)
				return RowProcessor::Read<1>(index.row(), z);

			break;
		case 2:
			if constexpr (TupleSize > 2)
				return RowProcessor::Read<2>(index.row(), z);

			break;
		case 3:
			if constexpr (TupleSize > 3)
				return RowProcessor::Read<3>(index.row(), z);

			break;
		case 4:
			if constexpr (TupleSize > 4)
				return RowProcessor::Read<4>(index.row(), z);
			break;
		case 5:
			if constexpr (TupleSize > 5)
				return RowProcessor::Read<5>(index.row(), z);
			break;
		case 6:
			if constexpr (TupleSize > 6)
				return RowProcessor::Read<6>(index.row(), z);
			break;
		case 7:
			if constexpr (TupleSize > 7)
				return RowProcessor::Read<7>(index.row(), z);
			break;
		case 8:
			if constexpr (TupleSize > 8)
				return RowProcessor::Read<8>(index.row(), z);
			break;
		case 9:
			if constexpr (TupleSize > 9)
				return RowProcessor::Read<9>(index.row(), z);
			break;
		case 10:
			if constexpr (TupleSize > 10)
				return RowProcessor::Read<10>(index.row(), z);
			break;
		case 11:
			if constexpr (TupleSize > 11)
				return RowProcessor::Read<11>(index.row(), z);
			break;
		case 12:
			if constexpr (TupleSize > 12)
				return RowProcessor::Read<12>(index.row(), z);
			break;
		case 13:
			if constexpr (TupleSize > 13)
				return RowProcessor::Read<13>(index.row(), z);
			break;
		case 14:
			if constexpr (TupleSize > 14)
				return RowProcessor::Read<14>(index.row(), z);
			break;
		case 15:
			if constexpr (TupleSize > 15)
				return RowProcessor::Read<15>(index.row(), z);
			break;
		case 16:
			if constexpr (TupleSize > 16)
				return RowProcessor::Read<16>(index.row(), z);
			break;
		default:
			throw std::out_of_range(std::format("{0:} too large!", TupleSize));
		}

		return QVariant{};
		//assert(false);
	}

	template<typename Container >
	void write(Container& z, QModelIndex index, const QVariant& value) {
		auto resultRow = z[index.row()];
		constexpr int TupleSize = std::tuple_size_v<decltype(resultRow)>;

		switch (index.column()) {
		case 0:
			if constexpr (TupleSize > 0)
				RowProcessor::Write<0>(index.row(), z, value);
			break;
		case 1:
			if constexpr (TupleSize > 1)
				RowProcessor::Write<1>(index.row(), z, value);

			break;
		case 2:
			if constexpr (TupleSize > 2)
				RowProcessor::Write<2>(index.row(), z, value);

			break;
		case 3:
			if constexpr (TupleSize > 3)
				RowProcessor::Write<3>(index.row(), z, value);

			break;
		case 4:
			if constexpr (TupleSize > 4)
				RowProcessor::Write<4>(index.row(), z, value);
			break;
		case 5:
			if constexpr (TupleSize > 5)
				RowProcessor::Write<5>(index.row(), z, value);
			break;
		case 6:
			if constexpr (TupleSize > 6)
				RowProcessor::Write<6>(index.row(), z, value);
			break;
		case 7:
			if constexpr (TupleSize > 7)
				RowProcessor::Write<7>(index.row(), z, value);
			break;
		case 8:
			if constexpr (TupleSize > 8)
				RowProcessor::Write<8>(index.row(), z, value);
			break;
		case 9:
			if constexpr (TupleSize > 9)
				RowProcessor::Write<9>(index.row(), z, value);
			break;
		case 10:
			if constexpr (TupleSize > 10)
				RowProcessor::Write<10>(index.row(), z, value);
			break;
		case 11:
			if constexpr (TupleSize > 11)
				RowProcessor::Write<11>(index.row(), z, value);
			break;
		case 12:
			if constexpr (TupleSize > 12)
				RowProcessor::Write<12>(index.row(), z, value);
			break;
		case 13:
			if constexpr (TupleSize > 13)
				RowProcessor::Write<13>(index.row(), z, value);
			break;
		case 14:
			if constexpr (TupleSize > 14)
				RowProcessor::Write<14>(index.row(), z, value);
			break;
		case 15:
			if constexpr (TupleSize > 15)
				RowProcessor::Write<15>(index.row(), z, value);
			break;
		case 16:
			if constexpr (TupleSize > 16)
				RowProcessor::Write<16>(index.row(), z, value);
			break;
		default:
			throw std::out_of_range(std::format("{0:} too large!", TupleSize));
		}

		//assert(false);
	}

}
