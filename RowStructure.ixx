module;

#include <type_traits>
#include <vector>

#include <sqlite_orm/sqlite_orm.h>

export module RowStructure;

import RowProcessor;

using namespace sqlite_orm;

export
{
	template<typename Storage_t, typename Select_t>
	class RowStructure
	{
	protected:
	public:
		using Storage = Storage_t;
		using Select = Select_t;
		using RowType = std::remove_reference_t<decltype(*std::declval<Storage_t>().iterate(std::declval<Select_t>()).begin())>;
		std::vector<RowType> rows;
		RowStructure(Storage_t& store, Select_t select) : store_(store), select_(std::move(select)) {}
		void refresh()
		{
			rows.clear();
			auto stmt = store_.prepare(select_);
			rows = store_.execute(stmt);
		}
		template<typename NewSelect>
		void refresh(NewSelect nselect)
		{
			rows.clear();
			auto stmt = store_.prepare(nselect);
			rows = store_.execute(stmt);
		}

	protected:
		Storage_t& store_;
		Select_t select_;
	};

}
