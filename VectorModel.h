#pragma once


#include <QObject>
#include <QAbstractTableModel>


import SubclassModel;
import RowProcessor;
//import 

class VectorModel : public QAbstractTableModel
{

	Q_OBJECT

	std::unique_ptr<AbstractModelImpl> p_impl_;

	std::vector<QVariant> headers;
public:
	VectorModel(std::unique_ptr<AbstractModelImpl> impl, QObject* parent = nullptr)
		: p_impl_{ std::move(impl) }, QAbstractTableModel(parent)
	{
		p_impl_->load();
		headers.resize(p_impl_->coll_count());
	}

	~VectorModel() = default;

	void refresh(auto storage_, auto select_)
	{
		p_impl_->refresh(storage_, select_);
	}

	void refresh()
	{
		p_impl_->load();
	}
	Status status(int row) const
	{
		return this->p_impl_->status(row);
	}
	void clearStatus(int row)
	{
		this->p_impl_->clearStatus(row);
	}
	bool insertRows(int row, int count, const QModelIndex& parent) override
	{
		beginInsertRows(parent, row, row + count - 1);
		auto pre_count = this->rowCount(QModelIndex{});
		bool ok = p_impl_->insertRows(row, count);
		auto pos_count = this->rowCount(QModelIndex{});
		endInsertRows();
		return ok;
	}
	bool removeRows(int row, int count, const QModelIndex& parent) override
	{
		beginRemoveRows(parent, row, row + count - 1);
		bool ok = p_impl_->eraseRows(row, count);
		endRemoveRows();
		return ok;
	}
	int rowCount(const QModelIndex& parent) const override
	{
		return p_impl_->row_count();
	}

	int columnCount(const QModelIndex& parent) const override
	{
		return p_impl_->coll_count();
	}

	QVariant data(const QModelIndex& index, int role) const override
	{
		using std::get;

		if (role != Qt::DisplayRole && role != Qt::EditRole)
		{
			return QVariant();
		}

		return p_impl_->data(index);
	}
	all_variant raw_data(const QModelIndex& index) const
	{
		return p_impl_->raw_data(index);
	}
	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role) override
	{
		if (orientation == Qt::Horizontal && role == Qt::EditRole)
		{
			headers[section] = value;
			headerDataChanged(orientation, section, section);
			return true;
		}
		return false;
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
	{
		if (orientation == Qt::Horizontal)
		{
			if (role == Qt::DisplayRole)
				return headers[section];
			return QVariant{};
		}
		return QVariant{};
	}
	bool setData(const QModelIndex& index, const QVariant& value, int role) override
	{
		if (role == Qt::EditRole)
		{
			auto count = this->p_impl_->row_count();
			bool ok = p_impl_->set_data(index, value);
			auto post_count = this->p_impl_->row_count();
			QModelIndex br = index.siblingAtRow(this->p_impl_->row_count() - 1);
			br = br.siblingAtColumn(this->columnCount(QModelIndex{}) - 1);
			dataChanged(index, br);
			return ok;
		}
		return false;
	}
	Qt::ItemFlags flags(const QModelIndex& index) const override
	{
		return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
	}

signals:
	void editCompleted(const QVariant& val);

};

