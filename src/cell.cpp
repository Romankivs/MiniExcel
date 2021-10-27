#include "cell.h"

cell::cell(QString innerText, QString displayText, Qt::Alignment textAlignment, QFont font, QColor backgroundColor, QColor textColor, bool isFormula) :
    innerText(innerText), displayText(displayText), textAlignment(textAlignment), font(font), backgroundColor(backgroundColor), textColor(textColor),
    isFormula(isFormula)
{

}

const QString &cell::getInnerText() const {
    return innerText;
}

void cell::setInnerText(const QString &newText) {
    innerText = newText;
}

const QString &cell::getDisplayText() const {
    return displayText;
}

void cell::setDisplayText(const QString &newText) {
    displayText = newText;
}

const QFont &cell::getFont() const
{
    return font;
}

void cell::setFont(const QFont &newFont)
{
    font = newFont;
}

const QColor &cell::getBackgroundColor() const
{
    return backgroundColor;
}

void cell::setBackgroundColor(const QColor &newBackgroundColor)
{
    backgroundColor = newBackgroundColor;
}

const QColor &cell::getTextColor() const
{
    return textColor;
}

void cell::setTextColor(const QColor &newTextColor)
{
    textColor = newTextColor;
}

const Qt::Alignment &cell::getTextAlignment() const
{
    return textAlignment;
}

void cell::setTextAlignment(const Qt::Alignment &newTextAlignment)
{
    textAlignment = newTextAlignment;
}

bool cell::getIsFormula() const {
    return isFormula;
}

void cell::setIsFormula(bool value) {
    isFormula = value;
}

QDataStream& operator<<(QDataStream& stream, const cell& _cell)
{
    stream << _cell.getInnerText() << _cell.getDisplayText() << _cell.getTextAlignment() <<_cell.getFont()
        << _cell.getBackgroundColor() << _cell.getTextColor() << _cell.getIsFormula();
    return stream;
}

QDataStream& operator>>(QDataStream& stream, cell& _cell)
{
    QString innerText, displayText;
    Qt::Alignment textAlignment;
    QFont font;
    QColor backgroundColor, textColor;
    bool isFormula;
    stream >> innerText >> displayText >> textAlignment >> font >> backgroundColor >> textColor >> isFormula;
    _cell = cell(innerText, displayText, textAlignment, font, backgroundColor, textColor, isFormula);
    return stream;
}



