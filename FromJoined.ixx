module;

#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include <optional>
#include <format>


export module FromJoined;
import util;


export class IJoined
{
public:
	virtual void generate() = 0;

	using Matrix = std::vector<std::vector<std::string>>;
	virtual Matrix&& getMatrix() = 0;
};



using std::string;

export
template<typename T, typename ColonesCols, typename DollarsCols>
class FromJoined  : public IJoined
{

	Matrix matrix;

	using Container = std::vector<std::remove_reference_t<T>>;
	using RowType = typename Container::value_type;
	inline static constexpr size_t NumCols = std::tuple_size<RowType>::value;
	Container					lines;
public:
	FromJoined(	Container&& lines_)		: lines{ std::move(lines_) }
	{
		matrix.resize(lines.size());
		for (int row = 0; row < lines.size(); ++row)
		{
			matrix[row].resize(NumCols);
		}

	}

	void generate() override
	{
		for (int i = 0; i < lines.size(); ++i)
		{
			PrintDataInMatrix<0, Container, NumCols>::Apply(i, lines, matrix);
		}
	}

	Matrix&& getMatrix() override
	{
		return std::move(matrix);
	}
private:
	template<int Col, typename FieldType, typename T>
	static string FormatCol(T& value, Matrix& matrix)
	{
		string cs;
		if constexpr ((std::is_integral_v<FieldType> || std::is_floating_point_v<FieldType>) && !std::is_same_v<std::decay<FieldType>::type, bool>)
		{
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
		}
		else
		{
			cs = format(value);
		}
		return cs;
	}
	template<int Col, typename Container, unsigned NumCols>
	struct PrintDataInMatrix
	{
		static void Apply(int row, const Container& z, Matrix& matrix)
		{
			string cs;

			using FieldType = std::remove_reference_t<decltype(std::get<Col>(z[row]))>;
			FieldType* pT = nullptr;
			if constexpr (std::is_same_v<FieldType, const std::unique_ptr<double>>)
			{
				auto&& value = extractValue(std::move(std::get<Col>(z[row])));
				cs = FormatCol<Col, double>(value, matrix);
			}
			else
			{
				auto&& value = std::get<Col>(z[row]);
				// auto val = extractValue(value);	// JDH

				cs = FormatCol<Col, FieldType>(value, matrix);
			}
			matrix[row][Col] = cs;
			PrintDataInMatrix<Col + 1, Container, NumCols>::Apply(row, z, matrix);
		}
	};

	template<typename Container>
	struct PrintDataInMatrix<NumCols, Container, NumCols>
	{
		static void Apply(int row, const Container& z, Matrix& matrix)
		{
		}
	};


	template<typename T>
	static string format(T t)
	{
		static_assert(std::is_same_v<decltype(t), T>);
		//return util::to_string(t);
		return std::to_string(t);
	}
	static string format(const std::string& str)
	{
		return str;
	}
	static string format(util::Colones t)
	{
		//static_assert(std::is_same_v<decltype(t), T>);
		//return util::to_string(t);
		return util::to_string(t);
	}

	static string format(util::Dolares t)
	{
		//static_assert(std::is_same_v<decltype(t), T>);
		//return util::to_string(t);
		return util::to_string(t);
	}



	template<typename T>
	static string format(const std::shared_ptr<T>& t)
	{
		if (t)
		{
			return util::to_string(*t);
		}
		return "";
	}

	template<typename T>
	static string format(const std::unique_ptr<T>& t)
	{
		if (t)
		{
			return util::to_string(*t);
		}
		return "";
	}

	template<typename T>
	static string format(const std::optional<T>& t)
	{
		if (t)
		{
			return std::to_string(*t);
		}
		return "";
	}

};
