#ifndef WINDOW_H
#define WINDOW_H

#include "InputToolBar/InputToolBar.h"
#include "MainToolBar/MainToolBar.h"
#include "tablemodel.h"
#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontComboBox>
#include <QFontDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QSpinBox>
#include <QTableView>
#include <QToolBar>
#include <QToolButton>
#include <algorithm>

enum FontStyleOptions {
    Bold,
    Italic,
    Strikethrough
};

class Window : public QMainWindow {
    Q_OBJECT
public:
    explicit Window();
    ~Window();
Q_SIGNALS:
    void currentFontChanged(const QFont &font);
    void currentFontSizeChanged(int fontSize);
    void currentFontIsBoldChanged(bool bold);
    void currentFontIsItalicChanged(bool italic);
    void currentFontIsStrikethroughChanged(bool strikethrough);
    void currentIsFormulaChanged(bool formula);
    void currentInnerTextChanged(QString text);
private Q_SLOTS:
    void verHeaderCustomContextMenu(const QPoint &point);
    void horHeaderCustomContextMenu(const QPoint &point);
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void fontChangedFromComboBox(const QFont &font);
    void fontSizeChanged(int fontSize);
    void fontStyleChanged(FontStyleOptions style, bool value);
    void isFormulaChanged(bool value);
    void innerTextChanged(const QString& text);

private:
    void save();
    void saveAs();
    void load();
    void addRows();
    void removeRows();
    void addColumns();
    void removeColumns();
    void chooseColor(const Qt::ItemDataRole &role);
    void chooseHorizontalAlignment(const Qt::Alignment alignment);
    void chooseVerticalAlignment(const Qt::Alignment alignment);
    void changeFontDialog();
    void createActions();
    void createToolBars();
    void about();
    TableModel *tableModel;
    QTableView *tableView;
    QString openedFileName;
};

#endif// WINDOW_H
