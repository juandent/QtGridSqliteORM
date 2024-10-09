#pragma once

#include <QDataWidgetMapper>
#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include "Data.h"
#include "VectorModel.h"

import TableGrid;
import joinedgrid;
import idisplayer;
import FromJoined;

class VectorModel;

using namespace sqlite_orm;

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    void currentRowChanged(const QModelIndex& current, const QModelIndex& previous);
    ~QtWidgetsApplication1();

    void setVariant(const QVariant& val);
private:
    Ui::QtWidgetsApplication1Class ui;
    void onCellChanged(int row, int column);
    void onCellChanged_joined(int row, int column);

    std::unique_ptr<GridDisplayer<Claim>> displayer;

    std::unique_ptr<IDisplayer> m_displayer_passwords;

    using RowType = decltype(storage().select(columns(&Claim::id, &Claim::start_date, &Claim::asprose_claim_number))[0]);

    std::unique_ptr<IJoined> m_joined;
    QDataWidgetMapper* mapper;
    void modelIndexChanged(const QModelIndex& index);
    QItemSelectionModel* selection_model;
    VectorModel* model;

public slots:
    void insertRow();
    void saveModel();
    void removeRow();

};
