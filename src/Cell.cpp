#include "Cell.h"
#include <iostream>


Cell::Cell(QString innerText, QString displayText, Qt::Alignment textAlignment,
           QFont font, QColor backgroundColor, QColor textColor, bool isFormula,
           QVector<QPersistentModelIndex> dependentCells, QVector<QModelIndex> cellsThatThisDependsOn, bool inExceptionState) : innerText(innerText), displayText(displayText), textAlignment(textAlignment),
                                                                                                                                font(font), backgroundColor(backgroundColor), textColor(textColor),
                                                                                                                                isFormula(isFormula), dependentCells(dependentCells), cellsThatThisDependsOn(cellsThatThisDependsOn),
                                                                                                                                inExceptionState(inExceptionState) {
}

bool Cell::operator==(const Cell &other) const {
    if (!(innerText == other.innerText &&
                displayText == other.displayText &&
                textAlignment == other.textAlignment &&
                font == other.font &&
                backgroundColor == other.backgroundColor &&
                textColor == other.textColor &&
                isFormula == other.isFormula &&
                inExceptionState == other.inExceptionState
                )) {
        return false;
    };
    if (cellsThatThisDependsOn.size() != other.cellsThatThisDependsOn.size())
        return false;
    else {
        for (size_t i = 0; i < cellsThatThisDependsOn.size(); ++i)
        {
            if (cellsThatThisDependsOn[i].row() != other.cellsThatThisDependsOn[i].row() ||
                cellsThatThisDependsOn[i].column() != other.cellsThatThisDependsOn[i].column()) {
                return false;
            }
        }
    }
    return true;
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