#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "Cell.h"
#include "CellFormulaInterpreter.h"
#include <QAbstractTableModel>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSize>
#include <QVariant>
#include <optional>

enum AddRoles {
    Formula = Qt::UserRole,
    Dependent,
    DependsOn,
    ExceptionState
};

class TableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    TableModel(const QVector<QVector<Cell>> &tableData = {}, QObject *parent = nullptr);
    bool saveToFile(QString fileName);
    bool loadFromFile(QString fileName);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool insertColumns(int position, int columns, const QModelIndex &index = QModelIndex()) override;
    bool removeColumns(int position, int columns, const QModelIndex &index = QModelIndex()) override;
    void recomputeCell(const QModelIndex &index, QVector<QModelIndex> alreadyVisited = {});
    void clearDependenciesFromCellsItDependsFrom(const QModelIndex &index);
    const QSize &getTableSize() const;
    void setTableSize(const QSize &newTableSize);
    QVector<QVector<Cell>> &getTableData();
    void setTableData(const QVector<QVector<Cell>> &newTableData);
    QString numberToColumnName(int num) const;
    int columnNameToNumber(QString columnName);

private:
    QSize tableSize;
    QVector<QVector<Cell>> tableData;
    CellFormulaInterpreter formulaInterp;
};

#endif// TABLEMODEL_H
