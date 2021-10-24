#include "textAlignmentButtons.h"

HorizontalTextAlignmentButton::HorizontalTextAlignmentButton()
{
    setIcon(QIcon(":/icons/textAlignmentLeft.png"));
    setToolTip("Horizontal Text Alignment");
    setPopupMode(QToolButton::InstantPopup);
    QMenu* horizontalTextAlignMenu = new QMenu(this);
    setMenu(horizontalTextAlignMenu);
}
