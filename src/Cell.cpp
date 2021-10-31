#include "Cell.h"

Cell::Cell(QString innerText, QString displayText, Qt::Alignment textAlignment,
           QFont font, QColor backgroundColor, QColor textColor, bool isFormula,
           QVector<QPersistentModelIndex> dependentCells, QVector<QModelIndex> cellsThatThisDependsOn, bool inExceptionState) : innerText(innerText), displayText(displayText), textAlignment(textAlignment),
                                                                                                                                font(font), backgroundColor(backgroundColor), textColor(textColor),
                                                                                                                                isFormula(isFormula), dependentCells(dependentCells), cellsThatThisDependsOn(cellsThatThisDependsOn),
                                                                                                                                inExceptionState(inExceptionState) {
}

QDataStream &operator<<(QDataStream &stream, const Cell &_cell) {
    stream << _cell.innerText << _cell.displayText << _cell.textAlignment << _cell.font
           << _cell.backgroundColor << _cell.textColor << _cell.isFormula;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Cell &_cell) {
    stream >> _cell.innerText >> _cell.displayText >> _cell.textAlignment >> _cell.font >> _cell.backgroundColor >> _cell.textColor >> _cell.isFormula;
    return stream;
}