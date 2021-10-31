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

class cell {
public:
    cell() = default;
    cell(QString innerText, QString displayText, Qt::Alignment textAlignment, QFont font,
         QColor backgroundColor, QColor textColor, bool isFormula, QVector<QPersistentModelIndex> dependentCells,
         QVector<QModelIndex> cellsThatThisDependsOn, bool inExceptionState);
    ~cell() = default;
    cell(const cell &other) = default;
    cell &operator=(const cell &other) = default;
    bool operator==(const cell &other) const = default;

    friend QDataStream &operator<<(QDataStream &stream, const cell &);
    friend QDataStream &operator>>(QDataStream &stream, cell &);

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
QDataStream &operator<<(QDataStream &stream, const cell &_cell);
QDataStream &operator>>(QDataStream &stream, cell &_cell);

Q_DECLARE_METATYPE(cell);

#endif// CELL_H
