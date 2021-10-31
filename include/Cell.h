#ifndef CELL_H
#define CELL_H

#include <QColor>
#include <QFont>
#include <QModelIndex>
#include <QPersistentModelIndex>
#include <QSize>
#include <QString>
#include <QVector>
#include <optional>

class Cell {
public:
    Cell() = default;
    Cell(QString innerText, QString displayText, Qt::Alignment textAlignment, QFont font,
         QColor backgroundColor, QColor textColor, bool isFormula, QVector<QPersistentModelIndex> dependentCells,
         QVector<QModelIndex> cellsThatThisDependsOn, bool inExceptionState);
    ~Cell() = default;
    Cell(const Cell &other) = default;
    Cell &operator=(const Cell &other) = default;
    bool operator==(const Cell &other) const = default;

    friend QDataStream &operator<<(QDataStream &stream, const Cell &);
    friend QDataStream &operator>>(QDataStream &stream, Cell &);

    QString innerText;
    QString displayText;
    Qt::Alignment textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    QFont font;
    QColor backgroundColor = QColor(255, 255, 255);
    QColor textColor = QColor(0, 0, 0);
    bool isFormula = false;
    QVector<QPersistentModelIndex> dependentCells;
    QVector<QModelIndex> cellsThatThisDependsOn;
    bool inExceptionState = false;
};

// Serialization functions
QDataStream &operator<<(QDataStream &stream, const Cell &_cell);
QDataStream &operator>>(QDataStream &stream, Cell &_cell);

Q_DECLARE_METATYPE(Cell);

#endif// CELL_H
