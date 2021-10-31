#include "MainToolBar/MainToolBar.h"

MainToolBar::MainToolBar() {
    setWindowTitle("Main ToolBar");
    setAllowedAreas(Qt::AllToolBarAreas);
    setMovable(true);

    createTableRowsAndColumnsToolButtons();
    createColorToolButtons();
    createTextAlignmentToolButtons();
    createFontToolButtonsAndWidgets();
}

void MainToolBar::changeSpinBoxValueWithoutSignaling(int value) {
    fontSizeSpinBox->blockSignals(true);
    fontSizeSpinBox->setValue(value);
    fontSizeSpinBox->blockSignals(false);
}

void MainToolBar::createTableRowsAndColumnsToolButtons() {
    addRowsAction = addAction(QIcon(":/icons/tableRowAddition.png"), "Add Rows");
    removeRowsAction = addAction(QIcon(":/icons/tableRowRemoval.png"), "Remove Rows");

    addSeparator();

    addColumnsAction = addAction(QIcon(":/icons/tableColumnAddition.png"), "Add Columns");
    removeColumnsAction = addAction(QIcon(":/icons/tableColumnRemoval.png"), "Remove Columns");

    addSeparator();
}

void MainToolBar::createColorToolButtons() {
    backgroundColorChooseAction = addAction(QIcon(":/icons/ColorChooseIcon.png"), "Background Color");
    textColorChooseAction = addAction(QIcon(":/icons/TextColorChooseIcon.png"), "Text Color");

    addSeparator();
}

void MainToolBar::createTextAlignmentToolButtons() {
    auto *horizontalTextAlignmentButton = new QToolButton;
    horizontalTextAlignmentButton->setIcon(QIcon(":/icons/textAlignmentLeft.png"));
    horizontalTextAlignmentButton->setToolTip("Horizontal Text Alignment");
    addWidget(horizontalTextAlignmentButton);
    horizontalTextAlignmentButton->setPopupMode(QToolButton::InstantPopup);
    auto *horizontalTextAlignMenu = new QMenu(horizontalTextAlignmentButton);
    horizontalTextAlignmentButton->setMenu(horizontalTextAlignMenu);

    alignLeftAction = horizontalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentLeft.png"), "Left");
    alignCenterAction = horizontalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentHCenter.png"), "Center");
    alignRightAction = horizontalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentRight.png"), "Right");

    auto *verticalTextAlignmentButton = new QToolButton;
    verticalTextAlignmentButton->setIcon(QIcon(":/icons/textAlignmentUp.png"));
    verticalTextAlignmentButton->setToolTip("Vertical Text Alignment");
    addWidget(verticalTextAlignmentButton);
    verticalTextAlignmentButton->setPopupMode(QToolButton::InstantPopup);
    auto *verticalTextAlignMenu = new QMenu(verticalTextAlignmentButton);
    verticalTextAlignmentButton->setMenu(verticalTextAlignMenu);

    alignUpAction = verticalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentUp.png"), "Up");
    alignVCenterAction = verticalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentVCenter.png"), "Center");
    alignDownAction = verticalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentDown.png"), "Down");

    addSeparator();
}

void MainToolBar::createFontToolButtonsAndWidgets() {
    fontDialogAction = addAction(QIcon(":/icons/fontDialog.png"), "Font Settings");

    fontComboBox = new QFontComboBox;
    addWidget(fontComboBox);

    fontSizeSpinBox = new QSpinBox;
    constexpr int minimalFontSize = 6;
    constexpr int maximumFontSize = 36;
    fontSizeSpinBox->setMinimum(minimalFontSize);
    fontSizeSpinBox->setMaximum(maximumFontSize);
    addWidget(fontSizeSpinBox);

    addSeparator();

    boldFontAction = addAction(QIcon(":/icons/boldFont.png"), "Bold");
    boldFontAction->setCheckable(true);

    italicsFontAction = addAction(QIcon(":/icons/italicsFont.png"), "Italic");
    italicsFontAction->setCheckable(true);

    strikethroughFontAction = addAction(QIcon(":/icons/strikethroughFont.png"), "Strikethrough");
    strikethroughFontAction->setCheckable(true);

    addSeparator();

    formulaAction = addAction(QIcon(":/icons/formula.png"), "Formula");
    formulaAction->setCheckable(true);
}
