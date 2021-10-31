#pragma once
#include <QFontComboBox>
#include <QMenu>
#include <QSpinBox>
#include <QToolBar>
#include <QToolButton>

class MainToolBar : public QToolBar {
public:
    MainToolBar();
    void changeSpinBoxValueWithoutSignaling(int value);
private:
    void createTableRowsAndColumnsToolButtons();
    void createColorToolButtons();
    void createTextAlignmentToolButtons();
    void createFontToolButtonsAndWidgets();

public:
    QAction *addRowsAction, *removeRowsAction, *addColumnsAction, *removeColumnsAction,
            *backgroundColorChooseAction, *textColorChooseAction,
            *alignLeftAction, *alignCenterAction, *alignRightAction,
            *alignUpAction, *alignVCenterAction, *alignDownAction,
            *fontDialogAction, *boldFontAction, *italicsFontAction, *strikethroughFontAction,
            *formulaAction;

    QFontComboBox *fontComboBox;
    QSpinBox *fontSizeSpinBox;
};
