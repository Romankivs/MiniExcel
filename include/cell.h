#ifndef CELL_H
#define CELL_H

#include <QString>
#include <QFont>
#include <QColor>
#include <QVector>
#include <QSize>
#include <QModelIndex>

struct CellIndex
{
    CellIndex() = default;
    CellIndex(int row, int column) : row(row), column(column) {}
    bool operator==(const CellIndex& other) const = default;
    int row;
    int column;
};
QDataStream& operator<<(QDataStream& stream, const CellIndex& idx);
QDataStream& operator>>(QDataStream& stream, CellIndex& idx);

class cell
{
public:
    cell() = default;
    cell(QString innerText, QString displayText, Qt::Alignment textAlignment, QFont font,
         QColor backgroundColor, QColor textColor, bool isFormula, QVector<CellIndex> dependentCells,
         QVector<CellIndex> cellsThatThisDependsOn);
    ~cell() = default;
    cell(const cell& other) = default;
    cell& operator=(const cell& other) = default;
    bool operator==(const cell& other) const = default;

    friend QDataStream& operator<<(QDataStream& stream, const cell&);
    friend QDataStream& operator>>(QDataStream& stream, cell&);

    QString innerText;
    QString displayText;
    Qt::Alignment textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    QFont font;
    QColor backgroundColor = QColor(255, 255, 255);
    QColor textColor = QColor(0, 0, 0);
    bool isFormula = false;
    QVector<CellIndex> dependentCells;
    QVector<CellIndex> cellsThatThisDependsOn;
};

// Serialization functions
QDataStream& operator<<(QDataStream& stream, const cell& _cell);
QDataStream& operator>>(QDataStream& stream, cell& _cell);

Q_DECLARE_METATYPE(cell);

#endif // CELL_H
