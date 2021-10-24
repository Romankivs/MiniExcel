#include "cell.h"

cell::cell(QString textData, Qt::Alignment textAlignment, QFont font, QColor backgroundColor, QColor textColor) :
    textData(textData), textAlignment(textAlignment), font(font), backgroundColor(backgroundColor), textColor(textColor)
{

}

const QString &cell::getTextData() const
{
    return textData;
}

void cell::setTextData(const QString &newTextData)
{
    textData = newTextData;
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

QDataStream& operator<<(QDataStream& stream, const cell& _cell)
{
    stream << _cell.getTextData() << _cell.getTextAlignment() <<_cell.getFont() << _cell.getBackgroundColor() << _cell.getTextColor();
    return stream;
}
QDataStream& operator>>(QDataStream& stream, cell& _cell)
{
    QString textData;
    Qt::Alignment textAlignment;
    QFont font;
    QColor backgroundColor, textColor;
    stream >> textData >> textAlignment >> font >> backgroundColor >> textColor;
    _cell = cell(textData, textAlignment, font, backgroundColor, textColor);
    return stream;
}

