#include "InputToolBar/InputToolBar.h"

InputToolBar::InputToolBar() {
    setWindowTitle("Input ToolBar");
    setAllowedAreas(Qt::AllToolBarAreas);
    setMovable(true);

    inputLineEdit = new QLineEdit;
    addWidget(inputLineEdit);
}
