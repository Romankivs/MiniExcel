#ifndef CELL_H
#define CELL_H

#include <QString>
#include <QFont>
#include <QColor>

class cell
{
public:
    cell() = default;
    cell(QString textData, Qt::Alignment textAlignment, QFont font, QColor backgroundColor, QColor textColor);
    ~cell() = default;
    cell(const cell& other) = default;
    cell& operator=(const cell& other) = default;
    bool operator==(const cell& other) const
    {
        return (textData == other.textData &&
               textAlignment == other.textAlignment &&
               font == other.font &&
               backgroundColor == other.backgroundColor &&
               textColor == other.textColor);
    }

    const QString &getTextData() const;
    void setTextData(const QString &newTextData);

    const QFont &getFont() const;
    void setFont(const QFont &newFont);

    friend QDataStream& operator<<(QDataStream& stream, const cell&);
    friend QDataStream& operator>>(QDataStream& stream, cell&);

    const QColor &getBackgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    const QColor &getTextColor() const;
    void setTextColor(const QColor &newTextColor);

    const Qt::Alignment &getTextAlignment() const;
    void setTextAlignment(const Qt::Alignment &newTextAlignment);

private:
    QString textData;
    Qt::Alignment textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    QFont font;
    QColor backgroundColor = QColor(255, 255, 255);
    QColor textColor = QColor(0, 0, 0);
};

// Serialization functions
QDataStream& operator<<(QDataStream& stream, const cell& _cell);
QDataStream& operator>>(QDataStream& stream, cell& _cell);

Q_DECLARE_METATYPE(cell);

#endif // CELL_H
