#ifndef WINDOW_H
#define WINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QTableView>
#include <QHeaderView>
#include <QMenu>
#include <QToolBar>
#include <QColorDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QFontComboBox>
#include <QSpinBox>
#include <QToolButton>
#include <QItemSelectionModel>
#include <algorithm>
#include "tablemodel.h"
#include "MainToolBar/textAlignmentButtons.h"

enum FontStyleOptions
{
    Bold,
    Italic,
    Strikethrough
};

class Window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Window();
    ~Window();
Q_SIGNALS:
    void currentFontChanged(const QFont& font);
    void currentFontSizeChanged(int fontSize);
    void currentFontIsBoldChanged(bool bold);
    void currentFontIsItalicChanged(bool italic);
    void currentFontIsStrikethroughChanged(bool strikethrough);
private Q_SLOTS:
    void verHeaderCustomContextMenu(const QPoint& point);
    void horHeaderCustomContextMenu(const QPoint& point);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void fontChangedFromComboBox(const QFont& font);
    void fontSizeChanged(int fontSize);
    void fontStyleChanged(FontStyleOptions style, bool value);
private:
    void save();
    void saveAs();
    void load();
    void addRows();
    void removeRows();
    void addColumns();
    void removeColumns();
    void chooseColor(const Qt::ItemDataRole& role);
    void chooseHorizontalAlignment(const Qt::Alignment alignment);
    void chooseVerticalAlignment(const Qt::Alignment alignment);
    void changeFontDialog();
    void changeSpinBoxValueWithoutSignaling(QSpinBox* spinBox, int value);
    void createActions();
    void createToolBars();
    void createTableRowsAndColumnsToolButtons(QToolBar* toolBar);
    void createColorToolButtons(QToolBar* toolBar);
    void createTextAlignmentToolButtons(QToolBar* toolBar);
    void createFontToolButtonsAndWidgets(QToolBar* toolBar);
    void about();
    TableModel* tableModel;
    QTableView* tableView;
    QString openedFileName;
};

#endif // WINDOW_H
