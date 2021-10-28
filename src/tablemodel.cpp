#include "tablemodel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent), formulaInterp(this)
{
}

TableModel::TableModel(QVector<QVector<cell> > &tableData, QObject *parent) :
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
            return tableData[index.row()][index.column()].getDisplayText();
        case Qt::EditRole:
            return tableData[index.row()][index.column()].getInnerText();
        case Qt::BackgroundRole:
            return tableData[index.row()][index.column()].getBackgroundColor();
        case Qt::ForegroundRole:
            return tableData[index.row()][index.column()].getTextColor();
        case Qt::TextAlignmentRole:
            return QVariant::fromValue(tableData[index.row()][index.column()].getTextAlignment());
        case Qt::FontRole:
            return tableData[index.row()][index.column()].getFont();
        case Qt::UserRole:
            return tableData[index.row()][index.column()].getIsFormula();
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
            if (tableData[index.row()][index.column()].getIsFormula())
            {
                QString valueToStr = value.toString();
                tableData[index.row()][index.column()].setDisplayText(formulaInterp.interpret(valueToStr));
            }
            else
                tableData[index.row()][index.column()].setDisplayText(value.toString());
            tableData[index.row()][index.column()].setInnerText(value.toString());
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
            return true;
        case Qt::BackgroundRole:
            tableData[index.row()][index.column()].setBackgroundColor(value.value<QColor>());
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::BackgroundRole});
            return true;
        case Qt::ForegroundRole:
            tableData[index.row()][index.column()].setTextColor(value.value<QColor>());
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::ForegroundRole});
            return true;
        case Qt::TextAlignmentRole:
            tableData[index.row()][index.column()].setTextAlignment(value.value<Qt::Alignment>());
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::TextAlignmentRole});
            return true;
        case Qt::FontRole:
            tableData[index.row()][index.column()].setFont(value.value<QFont>());
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::FontRole});
            return true;
        case Qt::UserRole:
            tableData[index.row()][index.column()].setIsFormula(value.value<bool>());
            if (value.value<bool>())
                tableData[index.row()][index.column()].setDisplayText(
                        formulaInterp.interpret(tableData[index.row()][index.column()].getInnerText()));
            else
                tableData[index.row()][index.column()].setDisplayText(tableData[index.row()][index.column()].getInnerText());
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
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
    if (tableSize.height() == 1)
    {
        QMessageBox::critical(nullptr, "Error", "Last row can`t be deleted!");
        return false;
    }
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        tableData.removeAt(position);
    tableSize.rheight() -= rows;

    endRemoveRows();
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
    if (tableSize.width() == 1)
    {
        QMessageBox::critical(nullptr, "Error", "Last column can`t be deleted!");
        return false;
    }
    beginRemoveColumns(QModelIndex(), position, position + columns - 1);

    for (int row = 0; row < tableSize.height(); ++row)
        tableData[row].removeAt(position);
    tableSize.rwidth() -= columns;

    endRemoveColumns();
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



