module;

#include <QVariant>
#include <QModelIndex>
//#include "Data.h"


export module SubclassModel;

import RowStructure;
import RowProcessor;

export
{
	enum class Status : unsigned char
	{
		Clean,
		Modified,
		Deleted
	};

	template<typename T, typename Row>
	class Collection
	{
	protected:
		T* This()
		{
			return static_cast<T*>(this);
		}
	public:
		Row row_;

		Collection(Row row) : row_(row) {}

		QVariant data(const QModelIndex& index) const
		{
			auto row = row_.rows[index.row()];
			QVariant qvar = read(row_.rows, index);
			return qvar;
		}
		using RowType = Row::RowType;

		template<typename Select>
		std::vector<RowType> refresh(Select select_clause)
		{
			row_.refresh(select_clause);
			return row_.rows;
		}

		std::vector<RowType> get()
		{
			return row_.rows;
		}

		std::vector<RowType> refresh()
		{
			row_.refresh();
			return row_.rows;
		}
		void load()
		{
			(void)refresh();
		}

		Status status(int row) const
		{
			return this->statuses[row];
		}
		void clearStatus(int row)
		{
			this->statuses[row] = Status::Clean;
		}
		all_variant raw_data(const QModelIndex& index) const
		{
			auto row = row_.rows[index.row()];
			auto var = raw_read(row_.rows, index);
			return var;
		}
		bool insertRows(int row, int count)
		{
			auto tmp = this->row_.rows[row];

			this->row_.rows.insert(this->row_.rows.begin() + row, tmp);
			this->statuses.insert(this->statuses.begin() + row, Status::Modified);
			return true;
		}
		bool eraseRows(int row, int count)
		{
			this->row_.rows.erase(this->row_.rows.begin() + row);
			return true;
		}
		bool set_data(const QModelIndex& index, const QVariant& value)
		{
			if (!isReadOnly(index))
			{
				write(row_.rows, index, value);
				statuses[index.row()] = Status::Modified;
				return true;
			}
			return false;
		}

		bool submitAll()
		{
			bool updated = false;
			for (auto index = 0u; index < row_count(); ++index)
			{
				if (statuses[index] == Status::Modified)
				{
					//store.update(rows[index]);
					statuses[index] = Status::Clean;
					updated = true;
				}
			}
			return updated;
		}

		int row_count() const
		{
			return row_.rows.size();
		}

		int coll_count() const
		{
			return (int)std::tuple_size<RowType>::value;
		}
	private:
		bool isReadOnly(const QModelIndex& index) const
		{
			auto ret = std::find(readonlyColumns.begin(), readonlyColumns.end(), index.column());
			return ret != readonlyColumns.end();
		}
		std::vector<int> readonlyColumns;
		std::vector<Status> statuses;

	};

	template<typename RowStruct>
	class ConcreteCollection : public Collection<ConcreteCollection<RowStruct>, RowStruct>
	{

	public:
		ConcreteCollection(RowStruct row_struct) : Collection<ConcreteCollection<RowStruct>, RowStruct>(row_struct) {}
		auto base() { return static_cast<Collection<ConcreteCollection<RowStruct>, RowStruct>*>(this); }
	};


	class AbstractModelImpl
	{
	public:
		void refresh(auto storage_, auto select_);
		virtual void load() = 0;
		virtual int coll_count() = 0;
		virtual Status status(int row) const = 0;
		virtual void clearStatus(int row) = 0;
		virtual bool insertRows(int row, int count) = 0;
		virtual bool eraseRows(int row, int count) = 0;
		virtual int row_count() = 0;
		virtual QVariant data(const QModelIndex& index) = 0;
		virtual all_variant raw_data(const QModelIndex& index) const = 0;
		virtual bool set_data(const QModelIndex& index, const QVariant& value) = 0;
	};


	template<typename RowStructure>
	class SubclassModel : public AbstractModelImpl
	{
	public:
		ConcreteCollection<RowStructure> collection;
		SubclassModel(RowStructure row_struct) : collection{ row_struct } {}
		void load() override
		{
			collection.load();
		}
		int coll_count() override
		{
			return collection.coll_count();
		}
		Status status(int row) const override
		{
			return collection.status(row);
		}
		void clearStatus(int row) override
		{
			collection.clearStatus(row);
		}
		bool insertRows(int row, int count) override
		{
			return collection.insertRows(row, count);
		}
		bool eraseRows(int row, int count) override
		{
			return collection.eraseRows(row, count);
		}
		int row_count() override
		{
			return collection.row_count();
		}
		QVariant data(const QModelIndex& index) override
		{
			return collection.data(index);
		}
		all_variant raw_data(const QModelIndex& index) const override
		{
			return collection.raw_data(index);
		}
		bool set_data(const QModelIndex& index, const QVariant& value) override
		{
			return collection.set_data(index, value);
		}
		void refresh(auto select_)
		{
			collection.refresh(select_);
		}
	};

	void AbstractModelImpl::refresh(auto storage_, auto select_)
	{
		using RowStruct = RowStructure<decltype(storage_), decltype(select_)>;
		// downcast
		auto sub = static_cast<SubclassModel<RowStruct>*>(this);
		sub->refresh(select_);
	}
}
