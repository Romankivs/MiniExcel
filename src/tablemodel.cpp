#include "tablemodel.h"

TableModel::TableModel(const QVector<QVector<cell> > &tableData, QObject *parent) :
    QAbstractTableModel(parent), tableData(tableData), formulaInterp(this)
{
    int width;
    int height = tableData.size();
    if (height == 0)
    {
        width = 0;
    }
    else
    {
        width = tableData.front().size();
    }
    tableSize = QSize(width, height);
}

bool TableModel::saveToFile(QString fileName)
{
    QFile file(fileName);
    if (fileName.isEmpty() || !file.open(QIODevice::WriteOnly))
        return false;
    QDataStream out(&file);
    out << tableSize << tableData;
    return true;
}

bool TableModel::loadFromFile(QString fileName)
{
    QFile file(fileName);
    if (fileName.isEmpty() || !file.open(QIODevice::ReadOnly))
        return false;
    QDataStream in(&file);
    beginResetModel();
    in >> tableSize >> tableData;
    endResetModel();
    for (size_t r = 0; r < rowCount(QModelIndex()); ++r)
    {
        for (size_t c = 0; c < columnCount(QModelIndex()); ++c)
        {
            recomputeCell(this->index(r, c));
        }
    }
    return true;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : tableSize.height();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : tableSize.width();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= tableSize.height() || index.row() < 0)
        return QVariant();
    if (index.column() >= tableSize.width() || index.column() < 0)
        return QVariant();
    switch(role)
    {
        case Qt::DisplayRole:
            return tableData[index.row()][index.column()].displayText;
        case Qt::EditRole:
            return tableData[index.row()][index.column()].innerText;
        case Qt::BackgroundRole:
            return tableData[index.row()][index.column()].backgroundColor;
        case Qt::ForegroundRole:
            return tableData[index.row()][index.column()].textColor;
        case Qt::TextAlignmentRole:
            return QVariant::fromValue(tableData[index.row()][index.column()].textAlignment);
        case Qt::FontRole:
            return tableData[index.row()][index.column()].font;
        case AddRoles::Formula:
            return tableData[index.row()][index.column()].isFormula;
        case AddRoles::Dependent:
            return QVariant::fromValue(tableData[index.row()][index.column()].dependentCells);
        case AddRoles::DependsOn:
            return QVariant::fromValue(tableData[index.row()][index.column()].cellsThatThisDependsOn);
        case AddRoles::ExceptionState:
            return tableData[index.row()][index.column()].inExceptionState;
        default:
            return QVariant();
    }
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        return numberToColumnName(section);
    }
    else if (orientation == Qt::Vertical)
    {
        return section;
    }
    return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    switch(role)
    {
        case Qt::EditRole:
            tableData[index.row()][index.column()].innerText = value.toString();
            clearDependenciesFromCellsItDependsFrom(index);
            Q_EMIT dataChanged(index, index, {Qt::EditRole});
            recomputeCell(index);
            return true;
        case Qt::BackgroundRole:
            tableData[index.row()][index.column()].backgroundColor = value.value<QColor>();
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::BackgroundRole});
            return true;
        case Qt::ForegroundRole:
            tableData[index.row()][index.column()].textColor = value.value<QColor>();
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::ForegroundRole});
            return true;
        case Qt::TextAlignmentRole:
            tableData[index.row()][index.column()].textAlignment = value.value<Qt::Alignment>();
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::TextAlignmentRole});
            return true;
        case Qt::FontRole:
            tableData[index.row()][index.column()].font = value.value<QFont>();
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::FontRole});
            return true;
        case AddRoles::Formula:
            tableData[index.row()][index.column()].isFormula = value.value<bool>();
            recomputeCell(index);
            return true;
        case AddRoles::Dependent:
            tableData[index.row()][index.column()].dependentCells = value.value<QVector<QPersistentModelIndex>>();
            return true;
        case AddRoles::DependsOn:
            tableData[index.row()][index.column()].cellsThatThisDependsOn = value.value<QVector<QModelIndex>>();
            return true;
        case AddRoles::ExceptionState:
            tableData[index.row()][index.column()].inExceptionState = value.value<bool>();
            return true;
        default:
            return false;
    }

}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
        tableData.insert(position, QVector<cell>(tableSize.width()));
    }
    tableSize.rheight() += rows;

    endInsertRows();
    return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    if (tableSize.height() - rows <= 0)
    {
        QMessageBox::critical(nullptr, "Error", "Last row can`t be deleted!");
        return false;
    }
    QVector<QPersistentModelIndex> dependentCellsToUpdate;
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        const QVector<cell>& curRow = tableData[position];
        for (size_t column = 0; column < curRow.size(); ++column)
        {
            for(auto c : curRow[column].dependentCells)
            {
                clearDependenciesFromCellsItDependsFrom(c);
                if (!dependentCellsToUpdate.contains(c))
                    dependentCellsToUpdate.push_back(c);
            }
        }
        tableData.removeAt(position);
    }
    tableSize.rheight() -= rows;

    endRemoveRows();

    for (auto c : dependentCellsToUpdate)
    {
        if (c.isValid()) // avoid recomputing already deleted
            recomputeCell(c);
    }

    return true;
}

bool TableModel::insertColumns(int position, int columns, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertColumns(QModelIndex(), position, position + columns - 1);

    for (int row = 0; row < tableSize.height(); ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            tableData[row].insert(position, {});
        }
    }
    tableSize.rwidth() += columns;

    endInsertColumns();
    return true;
}

bool TableModel::removeColumns(int position, int columns, const QModelIndex &index)
{
    Q_UNUSED(index);
    if (tableSize.width() - columns <= 0)
    {
        QMessageBox::critical(nullptr, "Error", "Last column can`t be deleted!");
        return false;
    }

    QVector<QPersistentModelIndex> dependentCellsToUpdate;
    beginRemoveColumns(QModelIndex(), position, position + columns - 1);

    for (int row = 0; row < tableSize.height(); ++row)
    {
        for (auto c : tableData[row][position].dependentCells)
        {
            clearDependenciesFromCellsItDependsFrom(c);
            if (!dependentCellsToUpdate.contains(c))
                dependentCellsToUpdate.push_back(c);
        }
        tableData[row].removeAt(position);
    }
    tableSize.rwidth() -= columns;

    endRemoveColumns();

    for (const auto& c : dependentCellsToUpdate)
    {
        if (c.isValid()) // avoid recomputing already deleted
            recomputeCell(c);
    }

    return true;
}

QVector<QVector<cell>>& TableModel::getTableData()
{
    return tableData;
}

QString TableModel::numberToColumnName(int num) const
{
    if (num <= 0)
        return "A";
    constexpr int numberOfLettersInAlphabet = 26;
    QString result;
    while (num > 0)
    {
        result += (QChar)('A' + (num % numberOfLettersInAlphabet));
        num /= numberOfLettersInAlphabet;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int TableModel::columnNameToNumber(QString columnName) {
    std::string str = columnName.toStdString();
    int result = 0;
    std::reverse(str.begin(), str.end());
    for (int i = 0; i< str.size(); ++i)
    {
        result += pow(26,i) * (char)(str[i] - 'A');
    }
    return result;
}

void TableModel::recomputeCell(const QModelIndex &index, QVector<QModelIndex> alreadyVisited)
{
    alreadyVisited.push_back(index);
    QString innerValue = tableData[index.row()][index.column()].innerText;
    if (tableData[index.row()][index.column()].isFormula)
    {
        QString recomputedDisplayValue = formulaInterp.interpret(innerValue, index);
        tableData[index.row()][index.column()].displayText = recomputedDisplayValue;
    }
    else
        tableData[index.row()][index.column()].displayText = innerValue;
    Q_EMIT dataChanged(index, index, {Qt::DisplayRole});
    QVector<QPersistentModelIndex> dependentCells = tableData[index.row()][index.column()].dependentCells;
    for (auto& c : dependentCells)
    {
        if (alreadyVisited.contains(c))
        {
            tableData[index.row()][index.column()].displayText = QString("Circular dependency");
            tableData[index.row()][index.column()].inExceptionState = true;
            if (alreadyVisited.count(c) == 1) // inform others about circular dependency
                recomputeCell(c, alreadyVisited);
        }
        else
        {
            recomputeCell(c, alreadyVisited);
        }
    }
}

void TableModel::clearDependenciesFromCellsItDependsFrom(const QModelIndex& index)
{
    for (auto& c : tableData[index.row()][index.column()].cellsThatThisDependsOn)
    {
        if (data(c, AddRoles::Dependent).isValid()) {
            QVector<QPersistentModelIndex> dependentCells = tableData[c.row()][c.column()].dependentCells;
            dependentCells.removeAll(index);
            tableData[c.row()][c.column()].dependentCells = dependentCells;
        }
    }
    tableData[index.row()][index.column()].cellsThatThisDependsOn = {};
}


void TableModel::setTableData(const QVector<QVector<cell> > &newTableData)
{
    tableData = newTableData;
}

const QSize &TableModel::getTableSize() const
{
    return tableSize;
}

void TableModel::setTableSize(const QSize &newTableSize)
{
    tableSize = newTableSize;
}




