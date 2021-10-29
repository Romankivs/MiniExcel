#include "cell.h"

cell::cell(QString innerText, QString displayText, Qt::Alignment textAlignment,
           QFont font, QColor backgroundColor, QColor textColor, bool isFormula,
           QVector<QModelIndex> dependentCells, QVector<QModelIndex> cellsThatThisDependsOn) :
    innerText(innerText), displayText(displayText), textAlignment(textAlignment),
    font(font), backgroundColor(backgroundColor), textColor(textColor),
    isFormula(isFormula), dependentCells(dependentCells), cellsThatThisDependsOn(cellsThatThisDependsOn)
{

}

QDataStream& operator<<(QDataStream& stream, const cell& _cell)
{
    QVector<QSize> convertedDependentCells;
    for (size_t i = 0; i < _cell.dependentCells.size(); ++i)
    {
        convertedDependentCells.push_back(QSize(_cell.dependentCells[i].column(), _cell.dependentCells[i].row()));
    }
    QVector<QSize> convertedCellsThatThisDependsOn;
    for (size_t i = 0; i < _cell.cellsThatThisDependsOn.size(); ++i)
    {
        convertedCellsThatThisDependsOn.push_back(QSize(_cell.cellsThatThisDependsOn[i].column(),
                                                        _cell.cellsThatThisDependsOn[i].row()));
    }
    stream << _cell.innerText << _cell.displayText << _cell.textAlignment <<_cell.font
        << _cell.backgroundColor << _cell.textColor
        << _cell.isFormula << convertedDependentCells
        << convertedCellsThatThisDependsOn;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, cell& _cell)
{
    QString innerText, displayText;
    Qt::Alignment textAlignment;
    QFont font;
    QColor backgroundColor, textColor;
    bool isFormula;
    QVector<QSize> convertedDependentCells;
    QVector<QSize> convertedCellsThatThisDependsOn;
    stream >> innerText >> displayText >> textAlignment >> font
        >> backgroundColor >> textColor >> isFormula >> convertedDependentCells >> convertedCellsThatThisDependsOn;
    QVector<QModelIndex> dependentCells;
    for (size_t i = 0; i < convertedDependentCells.size(); ++i)
    {
        dependentCells.push_back(QSize(_cell.dependentCells[i].column(), _cell.dependentCells[i].row()));
    }
    _cell = cell(innerText, displayText, textAlignment, font, backgroundColor, textColor, isFormula, dependentCells, cellsThatThisDependsOn);
    return stream;
}



