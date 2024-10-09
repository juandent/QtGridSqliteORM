#include "QtWidgetsApplication1.h"
#include "QtWidgetsApplication1.h"

#include <qsortfilterproxymodel.h>
#include <QDataWidgetMapper>
#include <qitemselectionmodel.h>

#include "Data.h"
#include "TableModel.h"
//#include "VectorModel.h"
#include "VectorModel.h"
#include <vector>


//using namespace std;

using namespace sqlite_orm;

import TableGrid;
import RowStructure;
import SubclassModel;
//import VectorModel;


QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    RowStructure row_struct(storage(), select(columns(&Claim::id, &Claim::amount, &Claim::start_date, &Claim::asprose_case_number, &Claim::photo),
        where(c(&Claim::id) > 400), order_by(&Claim::id)));

    row_struct.refresh();

    //auto v = storage().select(columns(&Claim::id, &Claim::amount, &Claim::start_date, &Claim::asprose_case_number, &Claim::photo));

    //auto select_ = select(columns(&Claim::id, &Claim::amount, &Claim::start_date, &Claim::asprose_case_number, &Claim::photo));
    //auto st = storage().prepare(select_);

    //auto stmt = storage().prepare(select(columns(&Claim::id, &Claim::amount, &Claim::start_date, &Claim::asprose_case_number, &Claim::photo)));


    ConcreteCollection collect{ row_struct };
    collect.refresh(select(columns(&Claim::id, &Claim::amount, &Claim::start_date, &Claim::asprose_case_number, &Claim::photo)));
    collect.refresh();
    auto vec = collect.get();

    //std::vector<Claim> vec = storage().get_all<Claim>();


    //Claim claim{ -1,-1,-1, -1,  tp, sd, "ASPROSE565788", "CASENo", 2000.00, "Mil", 0, "comment" };



    //std::vector<string> headers = { "id","fkey1", "fkey2", "fkey3", "amount", "start date", "submision date", "claim number", "case number", "amount presented", "ins claim", "type", "comment" };

    //displayer.reset(new GridDisplayer<Claim>
    //    { *ui.tableWidget, std::move(vec), std::move(headers) });
    //displayer->display(&Claim::id, &Claim::fkey_doctor, &Claim::fkey_medication, &Claim::fkey_patient, &Claim::amount, &Claim::start_date, &Claim::submission_date, &Claim::asprose_claim_number, &Claim::asprose_case_number, &Claim::asprose_amount_presented, &Claim::ins_claim_number, &Claim::asprose_acknowledgement_type, &Claim::comment);

    //ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //connect(ui.tableWidget, &QTableWidget::cellChanged, this, &QtWidgetsApplication1::onCellChanged);



 //   auto otherlines = storage().select(columns(&Claim::id, &Claim::start_date, &Claim::asprose_claim_number));

 //   std::vector<std::string> other_headers{ "ID", "START DATE", "ASPROSE CLAIM #" };

 //   //m_displayer_passwords.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<>, IntegerList<>>(*ui.tableWidget_joined, std::move(otherlines), std::move(other_headers)));
 ////   m_displayer_passwords->display();

 //   connect(ui.tableWidget_joined, &QTableWidget::cellChanged, this, &QtWidgetsApplication1::onCellChanged_joined);

    //ui.tableView->setModel(new VectorModel{});


    //auto rows = storage().select(columns(&Claim::id, &Claim::asprose_claim_number, &Claim::amount)); // , & Claim::start_date, & Claim::asprose_claim_number));

    //auto vect = rows;
    ////m_joined.reset(new FromJoined<decltype(rows[0]), IntegerList<>, IntegerList<>>{ rows });
    ////m_joined->generate();
    ////auto m = m_joined->getMatrix();

    //auto joined = new FromJoined<decltype(rows[0]), ColonesCols<>, DollarsCols <>>{ std::move(rows) };
    //joined->generate();
    //auto mm = joined->getMatrix();

    //constexpr size_t ss = std::tuple_size_v<std::decay_t<decltype(rows[0])>>;

    // pass ownership to model
    model = new VectorModel{ std::unique_ptr<AbstractModelImpl>(new SubclassModel{row_struct}), this};


    model->refresh(storage(), select(columns(&Claim::id, &Claim::amount, &Claim::start_date, &Claim::asprose_case_number, &Claim::photo) ));

    QSortFilterProxyModel* proxy = new QSortFilterProxyModel{ this };
    proxy->setSourceModel(model);
    //ui.tableView->setHorizontalHeader(new QHeaderView{ Qt::Horizontal });
    ui.tableView->setSortingEnabled(true);
    ui.tableView->setModel(proxy);
    bool ok = proxy->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    ok = proxy->setHeaderData(1, Qt::Horizontal, QObject::tr("Amount"));
    ok = proxy->setHeaderData(2, Qt::Horizontal, QObject::tr("Start Date"));
    ok = proxy->setHeaderData(3, Qt::Horizontal, QObject::tr("Case Number"));
    ok = proxy->setHeaderData(4, Qt::Horizontal, QObject::tr("Photo"));
    //ui.tableView->setHorizontalHeader(new QHeaderView{ Qt::Horizontal });
    //ui.tableView->setSortingEnabled(true);
    ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    //ui.tableView->verticalHeader()->hide();
    //ui.tableView->horizontalHeader()->setSortIndicator(2, Qt::DescendingOrder);


    mapper = new QDataWidgetMapper{};
    mapper->setModel(proxy);
    mapper->addMapping(ui.lineEdit, 0);
    mapper->addMapping(ui.lineEdit_2, 1);
    mapper->toFirst();


    selection_model = ui.tableView->selectionModel();


    //model->insertRows(1, 1, QModelIndex{});

    connect(selection_model, &QItemSelectionModel::currentRowChanged, mapper, &QDataWidgetMapper::setCurrentModelIndex);
    connect(selection_model, &QItemSelectionModel::currentRowChanged, this, &QtWidgetsApplication1::modelIndexChanged);
    //connect(model, &VectorModel::editCompleted,
    //    this, &QtWidgetsApplication1::setVariant);
//    useTuple();
}
void QtWidgetsApplication1::modelIndexChanged(const QModelIndex& index)
{
    auto ix = ui.tableView->currentIndex();
}
void QtWidgetsApplication1::insertRow()
{
    auto ix = ui.tableView->currentIndex();
    if (!ix.isValid()) return;
    //auto count = model->rowCount(QModelIndex{});
    model->insertRows(ix.row(), 1, QModelIndex{});
    //auto post_count = model->rowCount(QModelIndex{});
    QModelIndex insertIndex = ix.siblingAtColumn(0);

    model->setData(insertIndex, QVariant(-1), Qt::EditRole);
    //count = model->rowCount(QModelIndex{});
    //ui.tableView->reset();
}

void QtWidgetsApplication1::removeRow()
{
    auto ix = ui.tableView->currentIndex();
    if (!ix.isValid()) return;
    model->removeRows(ix.row(), 1, QModelIndex{});

}

void QtWidgetsApplication1::saveModel()
{
    auto ix = ui.tableView->currentIndex();
    if (!ix.isValid()) return;
    for (int row = 0; row < model->rowCount(QModelIndex{}); ++row)
    {
        Status stat = model->status(row);
        if (stat == Status::Clean) continue;

        ix = ix.siblingAtRow(row);
        Claim claim;
        for (int col = 0; col < model->columnCount(QModelIndex{}); ++col)
        {
            ix = ix.siblingAtColumn(col);
            if (!ix.isValid()) continue;
            auto var = model->raw_data(ix);
            auto type_index = var.index();
            switch (col)
            {
            case 0:
            {
                using type = decltype(claim.id);
                type val = get<type>(var);
                claim.id = val;
                if(claim.id != -1)
					claim = storage().get<Claim>(claim.id);
                break;
            }
            case 1:
            {
                using type = decltype(claim.amount);
                type val = get<type>(var);
                claim.amount = val;
                break;
            }
            case 2:
            {
                using type = decltype(claim.start_date);
                type val = get<type>(var);
                claim.start_date = val;
                break;
            }
            case 3:
            {
                using type = decltype(claim.asprose_case_number);
                type val = get<type>(var);
                claim.asprose_case_number = val;
                break;
            }
            case 4:
	            {
                using type = decltype(claim.photo);
                type val = get<type>(var);
                claim.photo = val;
                break;
	            }
            }
        }
        if (claim.id == -1)
            storage().insert(claim);
        else
            storage().replace(claim);
        model->clearStatus(row);
        ix = ix.siblingAtColumn(0);
    }
}

void QtWidgetsApplication1::setVariant(const QVariant& val)
{
    if( val.typeId() == QMetaType::Double )
    {
        double d = val.toDouble();

    }
    auto v = val.metaType();
    //if( v.underlyingType() == QMetaType::Double)
}

void QtWidgetsApplication1::currentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
    int i = current.row();
    if(current.row() > previous.row())
    {
        //mapper->toNext();
    }
    mapper->setCurrentModelIndex(current);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

void QtWidgetsApplication1::onCellChanged(int row, int column)
{
    std::vector vec = storage().get_all<Claim>();

    int i = 0;
    QVariant var = ui.tableWidget->item(row, column)->data(Qt::DisplayRole);
	switch(column)
	{
	case 0:
        vec[row].id = var.toInt();
        break;
	}
    storage().replace(vec[row]);
}

void QtWidgetsApplication1::onCellChanged_joined(int row, int column)
{
 
}
