#include "cell.h"

QDataStream& operator<<(QDataStream& stream, const CellIndex& idx)
{
    stream << idx.row << idx.column;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, CellIndex& idx)
{
    stream >> idx.row >> idx.column;
    return stream;
}

cell::cell(QString innerText, QString displayText, Qt::Alignment textAlignment,
           QFont font, QColor backgroundColor, QColor textColor, bool isFormula,
           QVector<CellIndex> dependentCells, QVector<CellIndex> cellsThatThisDependsOn) :
        innerText(innerText), displayText(displayText), textAlignment(textAlignment),
        font(font), backgroundColor(backgroundColor), textColor(textColor),
        isFormula(isFormula), dependentCells(dependentCells), cellsThatThisDependsOn(cellsThatThisDependsOn)
{

}

QDataStream& operator<<(QDataStream& stream, const cell& _cell)
{
    stream << _cell.innerText << _cell.displayText << _cell.textAlignment <<_cell.font
           << _cell.backgroundColor << _cell.textColor
           << _cell.isFormula << _cell.dependentCells
           << _cell.cellsThatThisDependsOn;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, cell& _cell)
{
    stream >> _cell.innerText >> _cell.displayText >> _cell.textAlignment >>_cell.font
           >> _cell.backgroundColor >> _cell.textColor
           >> _cell.isFormula >> _cell.dependentCells
           >> _cell.cellsThatThisDependsOn;
    return stream;
}