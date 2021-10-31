#pragma once
#include <QLineEdit>
#include <QToolBar>

class InputToolBar : public QToolBar {
public:
    InputToolBar();
    QLineEdit *inputLineEdit;
};
