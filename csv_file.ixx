module;


//#include "pch.h"
#include <vector>
#include <string>
#include <cassert>
#include <chrono>


export module csv_file;

import fixed_point;


namespace csv
{

	export struct  Cell_Position
	{
	private:
		std::size_t m_row;
		std::size_t m_col;
	public:
		Cell_Position(const std::initializer_list<size_t>& coordinates)
			: m_row{ *coordinates.begin() },
			m_col{ *std::next(coordinates.begin()) }
		{
			assert(coordinates.size() == 2);
		}
		std::size_t getRow() const noexcept { return m_row; }
		std::size_t getCol() const noexcept { return m_col; }
	};


	// associated to one file but may be saved to different file names
	export class   CSV_File
	{
	public:
		using Row = std::vector<std::string>;
		using StringMatrix = std::vector<Row>;
		using Iterator = StringMatrix::const_iterator;
		using ReverseIterator = StringMatrix::reverse_iterator;
		using ConstantIterator = StringMatrix::const_iterator;
		using ConstantReverseIterator = StringMatrix::const_reverse_iterator;
	private:
		StringMatrix		m_matrix;
		// assumptions: all rows have the same number of columns
		Row m_currentRow;

		void addCol(const std::string& col);
		void addCol(std::chrono::sys_days pt);
		void addCol(Money money);
		void addEndl();

	public:
		CSV_File();

		CSV_File(const CSV_File& other) = delete;
		~CSV_File() = default;

		// closes file after reading it!
		void load(const std::string& fileName);

		void saveAs(std::string outputFileName);

		// access functions (instead of using m_matrix directly):
		//std::string& operator()( const Cell_Position& pos );
		ConstantReverseIterator crbegin() const { return m_matrix.crbegin(); }
		ConstantReverseIterator crend() const { return m_matrix.crend(); }

		ConstantIterator cbegin() const { return m_matrix.cbegin(); }
		ConstantIterator cend() const { return m_matrix.cend(); }

		std::string getString(const Cell_Position& pos) const;
		std::chrono::sys_days getDate(const Cell_Position& pos) const;
		std::chrono::sys_days getUSDate(const Cell_Position& pos) const;
		Money getMoney(const Cell_Position& pos) const;
		bool getBool(const Cell_Position& pas) const;

		Row getRow(std::size_t rowNumber) const;

		// DONE: moved it to private!
		// StringMatrix		m_matrix;

		CSV_File& operator<<(const std::string& col);
		CSV_File& operator<<(std::chrono::sys_days pt);
		CSV_File& operator<<(Money money);



		//	typedef std::basic_ostream<char>&  (__cdecl *Manip)(std::basic_ostream<char>&);
		using Manipulator = std::basic_ostream<char>& (__cdecl*)(std::basic_ostream<char>&);

		CSV_File& operator<<(Manipulator func);


		// accessors
		std::string getText(int row, int col) const noexcept;
		size_t getRowCount() const noexcept;
		size_t getColCount() const noexcept;
	};
}
