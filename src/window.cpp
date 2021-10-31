#include "window.h"
#include "CellFormulaVisitor.h"
#include "ExceptionCellExpressionListener.h"

Window::Window() {
    tableModel = new TableModel();
    tableModel->insertRows(0, 50);
    tableModel->insertColumns(0, 50);
    tableView = new QTableView(this);
    tableView->setModel(tableModel);
    setCentralWidget(tableView);
    connect(tableView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &Window::selectionChanged);

    QHeaderView *verHeader = tableView->verticalHeader();
    verHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(verHeader, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(verHeaderCustomContextMenu(QPoint)));
    QHeaderView *horHeader = tableView->horizontalHeader();
    horHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(horHeader, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(horHeaderCustomContextMenu(QPoint)));

    createActions();
    createToolBars();
}

Window::~Window() {
    delete tableModel;
}

void Window::verHeaderCustomContextMenu(const QPoint &point) {
    auto *contextMenu = new QMenu("Rows", this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose);
    QAction *remRowAction = contextMenu->addAction(QIcon(":/icons/tableRowRemoval.png"), "Remove row");
    connect(remRowAction, &QAction::triggered, this, [point, this]() { tableModel->removeRows(tableView->verticalHeader()->logicalIndexAt(point), 1); });
    QAction *insRowAction = contextMenu->addAction(QIcon(":/icons/tableRowAddition.png"), "Insert row");
    connect(insRowAction, &QAction::triggered, this, [point, this]() { tableModel->insertRows(tableView->verticalHeader()->logicalIndexAt(point), 1); });
    contextMenu->popup(mapToGlobal(point));
}

void Window::horHeaderCustomContextMenu(const QPoint &point) {
    auto *contextMenu = new QMenu("Columns", this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose);
    QAction *remColAction = contextMenu->addAction(QIcon(":/icons/tableColumnRemoval.png"), "Remove column");
    connect(remColAction, &QAction::triggered, this, [point, this]() { tableModel->removeColumns(tableView->horizontalHeader()->logicalIndexAt(point), 1); });
    QAction *insColAction = contextMenu->addAction(QIcon(":/icons/tableColumnAddition.png"), "Insert column");
    connect(insColAction, &QAction::triggered, this, [point, this]() { tableModel->insertColumns(tableView->horizontalHeader()->logicalIndexAt(point), 1); });
    contextMenu->popup(mapToGlobal(point));
}

void Window::selectionChanged(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous);
    auto changedFont = tableModel->data(current, Qt::FontRole).value<QFont>();
    Q_EMIT currentFontChanged(changedFont);
    Q_EMIT currentFontSizeChanged(changedFont.pointSize());
    Q_EMIT currentFontIsBoldChanged(changedFont.bold());
    Q_EMIT currentFontIsItalicChanged(changedFont.italic());
    Q_EMIT currentFontIsStrikethroughChanged(changedFont.strikeOut());
    bool changedIsFormula = tableModel->data(current, AddRoles::Formula).value<bool>();
    Q_EMIT currentIsFormulaChanged(changedIsFormula);
    auto changedInnerText = tableModel->data(current, Qt::EditRole).value<QString>();
    Q_EMIT currentInnerTextChanged(changedInnerText);
}

void Window::fontChangedFromComboBox(const QFont &font) {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            tableModel->setData(index, font, Qt::FontRole);
        }
    }
}

void Window::fontSizeChanged(int fontSize) {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            auto currentCellFont = tableModel->data(index, Qt::FontRole).value<QFont>();
            currentCellFont.setPointSize(fontSize);
            tableModel->setData(index, currentCellFont, Qt::FontRole);
        }
    }
}

void Window::fontStyleChanged(FontStyleOptions style, bool value) {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            auto currentCellFont = tableModel->data(index, Qt::FontRole).value<QFont>();
            switch (style) {
                case FontStyleOptions::Bold:
                    currentCellFont.setBold(value);
                    break;
                case FontStyleOptions::Italic:
                    currentCellFont.setItalic(value);
                    break;
                case FontStyleOptions::Strikethrough:
                    currentCellFont.setStrikeOut(value);
                    break;
                default:
                    break;
            }
            tableModel->setData(index, currentCellFont, Qt::FontRole);
        }
    }
}

void Window::isFormulaChanged(bool value) {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            tableModel->setData(index, value, AddRoles::Formula);
        }
    }
}

void Window::innerTextChanged(const QString& text) {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            tableModel->setData(index, text, Qt::EditRole);
        }
    }
}


void Window::save() {
    if (!openedFileName.isEmpty()) {
        if (!tableModel->saveToFile(openedFileName)) {
            QMessageBox::critical(this, "Save error", "Such file cannot be created");
        }
    } else {
        QString fileNameToSave = QInputDialog::getText(this, "Save", "Enter new file name");
        if (fileNameToSave.isEmpty())
            return;
        if (!tableModel->saveToFile(fileNameToSave)) {
            QMessageBox::critical(this, "Save error", "Such file cannot be created");
        } else {
            openedFileName = fileNameToSave;
        }
    }
}

void Window::saveAs() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open MiniExcel Table"), "/", "MiniExcel Files (*.mex)");
    if (fileName.isEmpty())
        return;
    if (!tableModel->saveToFile(fileName)) {
        QMessageBox::critical(this, "Save As error", "Error saving to such file");
    } else {
        openedFileName = fileName;
    }
}

void Window::load() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open MiniExcel Table"), "/", "MiniExcel Files (*.mex)");
    if (fileName.isEmpty())
        return;
    if (!tableModel->loadFromFile(fileName)) {
        QMessageBox::critical(this, "Load failure", "Cannot load such file");
    } else {
        openedFileName = fileName;
    }
}

void Window::addRows() {
    int rowsCount = QInputDialog::getInt(this, "Add Rows", "Enter number of rows to add.", 1, 1);
    if (!tableView->selectionModel()->hasSelection()) {
        tableModel->insertRows(0, rowsCount);
    } else {
        QModelIndexList selectedRows = tableView->selectionModel()->selectedIndexes();
        std::sort(selectedRows.begin(), selectedRows.end(), [](const QModelIndex &a, const QModelIndex &b) { return a.row() < b.row(); });
        tableModel->insertRows(selectedRows.constFirst().row(), rowsCount);
    }
}

void Window::removeRows() {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No rows were selected!");
    } else {
        QModelIndexList selectedRows = tableView->selectionModel()->selectedIndexes();
        QModelIndexList::iterator last = std::unique(selectedRows.begin(), selectedRows.end(),
                                                     [](const QModelIndex &a, const QModelIndex &b) { return a.row() == b.row(); });
        selectedRows.erase(last, selectedRows.end());
        std::sort(selectedRows.begin(), selectedRows.end(), [](const QModelIndex &a, const QModelIndex &b) { return a.row() < b.row(); });
        tableModel->removeRows(selectedRows.constFirst().row(), selectedRows.size());
    }
}

void Window::addColumns() {
    int columnsCount = QInputDialog::getInt(this, "Add Columns", "Enter number of columns to add.", 1, 1);
    if (!tableView->selectionModel()->hasSelection()) {
        tableModel->insertColumns(0, columnsCount);
    } else {
        QModelIndexList selectedColumns = tableView->selectionModel()->selectedIndexes();
        std::sort(selectedColumns.begin(), selectedColumns.end(), [](const QModelIndex &a, const QModelIndex &b) { return a.column() < b.column(); });
        tableModel->insertColumns(selectedColumns.constFirst().column(), columnsCount);
    }
}

void Window::removeColumns() {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No columns were selected!");
    } else {
        QModelIndexList selectedColumns = tableView->selectionModel()->selectedIndexes();
        QModelIndexList::iterator last = std::unique(selectedColumns.begin(), selectedColumns.end(),
                                                     [](const QModelIndex &a, const QModelIndex &b) { return a.column() == b.column(); });
        selectedColumns.erase(last, selectedColumns.end());
        std::sort(selectedColumns.begin(), selectedColumns.end(), [](const QModelIndex &a, const QModelIndex &b) { return a.column() < b.column(); });
        tableModel->removeColumns(selectedColumns.constFirst().column(), selectedColumns.size());
    }
}

void Window::chooseColor(const Qt::ItemDataRole &role) {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        QColor colorChosen = QColorDialog::getColor();
        if (!colorChosen.isValid())
            return;
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            tableModel->setData(index, colorChosen, role);
        }
    }
}

void Window::chooseHorizontalAlignment(const Qt::Alignment alignment) {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            Qt::Alignment verticalAlignment = tableModel->data(index, Qt::TextAlignmentRole).value<Qt::Alignment>() & Qt::AlignVertical_Mask;
            Qt::Alignment resultingAlignment = (alignment & Qt::AlignHorizontal_Mask) | verticalAlignment;
            tableModel->setData(index, QVariant::fromValue(resultingAlignment), Qt::TextAlignmentRole);
        }
    }
}

void Window::chooseVerticalAlignment(const Qt::Alignment alignment) {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            Qt::Alignment horizontalAlignment = tableModel->data(index, Qt::TextAlignmentRole).value<Qt::Alignment>() & Qt::AlignHorizontal_Mask;
            Qt::Alignment resultingAlignment = (alignment & Qt::AlignVertical_Mask) | horizontalAlignment;
            tableModel->setData(index, QVariant::fromValue(resultingAlignment), Qt::TextAlignmentRole);
        }
    }
}

void Window::changeFontDialog() {
    if (!tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    } else {
        bool ok;
        QFont fontChosen = QFontDialog::getFont(&ok);
        if (!ok)
            return;
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto &index : selectedTableIndexes) {
            tableModel->setData(index, fontChosen, Qt::FontRole);
        }
        Q_EMIT currentFontChanged(fontChosen);
        Q_EMIT currentFontSizeChanged(fontChosen.pointSize());
        Q_EMIT currentFontIsBoldChanged(fontChosen.bold());
        Q_EMIT currentFontIsItalicChanged(fontChosen.italic());
        Q_EMIT currentFontIsStrikethroughChanged(fontChosen.strikeOut());
    }
}

void Window::createActions() {
    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    connect(saveAction, &QAction::triggered, this, [this]() { save(); });
    QAction *saveAsAction = fileMenu->addAction("Save As");
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAction, &QAction::triggered, this, [this]() { saveAs(); });
    QAction *loadAction = fileMenu->addAction("Load");
    loadAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    connect(loadAction, &QAction::triggered, this, [this]() { load(); });

    QMenu *helpMenu = menuBar()->addMenu("Help");
    QAction *aboutAction = helpMenu->addAction("About");
    aboutAction->setShortcut(QKeySequence(Qt::Key_F1));
    connect(aboutAction, &QAction::triggered, this, [this]() { about(); });
}

void Window::createToolBars() {
    auto *mainToolBar = new MainToolBar;
    addToolBar(mainToolBar);

    connect(mainToolBar->addColumnsAction, &QAction::triggered, this, &Window::addColumns);
    connect(mainToolBar->addRowsAction, &QAction::triggered, this, &Window::addRows);
    connect(mainToolBar->removeColumnsAction, &QAction::triggered, this, &Window::removeColumns);
    connect(mainToolBar->removeRowsAction, &QAction::triggered, this, &Window::removeRows);

    connect(mainToolBar->backgroundColorChooseAction, &QAction::triggered, this, [this]() { chooseColor(Qt::BackgroundRole); });
    connect(mainToolBar->textColorChooseAction, &QAction::triggered, this, [this]() { chooseColor(Qt::ForegroundRole); });

    connect(mainToolBar->alignLeftAction, &QAction::triggered, this, [this]() { chooseHorizontalAlignment(Qt::AlignLeft); });
    connect(mainToolBar->alignCenterAction, &QAction::triggered, this, [this]() { chooseHorizontalAlignment(Qt::AlignHCenter); });
    connect(mainToolBar->alignRightAction, &QAction::triggered, this, [this]() { chooseHorizontalAlignment(Qt::AlignRight); });

    connect(mainToolBar->alignUpAction, &QAction::triggered, this, [this]() { chooseVerticalAlignment(Qt::AlignTop); });
    connect(mainToolBar->alignVCenterAction, &QAction::triggered, this, [this]() { chooseVerticalAlignment(Qt::AlignVCenter); });
    connect(mainToolBar->alignDownAction, &QAction::triggered, this, [this]() { chooseVerticalAlignment(Qt::AlignBottom); });

    connect(mainToolBar->fontDialogAction, &QAction::triggered, this, &Window::changeFontDialog);
    connect(this, &Window::currentFontChanged, mainToolBar->fontComboBox, &QFontComboBox::setCurrentFont);
    connect(mainToolBar->fontComboBox, &QComboBox::activated, this,
            [this, mainToolBar](int index) { fontChangedFromComboBox(QFont(mainToolBar->fontComboBox->itemText(index))); });

    connect(this, &Window::currentFontSizeChanged, mainToolBar, &MainToolBar::changeSpinBoxValueWithoutSignaling);
    connect(mainToolBar->fontSizeSpinBox, &QSpinBox::valueChanged, this, &Window::fontSizeChanged);

    connect(this, &Window::currentFontIsBoldChanged, mainToolBar->boldFontAction, &QAction::setChecked);
    connect(mainToolBar->boldFontAction, &QAction::triggered, this,
            [this](bool value) { fontStyleChanged(FontStyleOptions::Bold, value); });
    connect(this, &Window::currentFontIsItalicChanged, mainToolBar->italicsFontAction, &QAction::setChecked);
    connect(mainToolBar->italicsFontAction, &QAction::triggered, this,
            [this](bool value) { fontStyleChanged(FontStyleOptions::Italic, value); });
    connect(this, &Window::currentFontIsStrikethroughChanged, mainToolBar->strikethroughFontAction, &QAction::setChecked);
    connect(mainToolBar->strikethroughFontAction, &QAction::triggered, this,
            [this](bool value) { fontStyleChanged(FontStyleOptions::Strikethrough, value); });

    connect(this, &Window::currentIsFormulaChanged, mainToolBar->formulaAction, &QAction::setChecked);
    connect(mainToolBar->formulaAction, &QAction::triggered, this, [this](bool value) { isFormulaChanged(value); });

    auto *inputToolBar = new InputToolBar;
    addToolBar(inputToolBar);

    connect(this, &Window::currentInnerTextChanged, inputToolBar->inputLineEdit, &QLineEdit::setText);
    connect(inputToolBar->inputLineEdit, &QLineEdit::textEdited, this, &Window::innerTextChanged);
    connect(tableModel, &QAbstractTableModel::dataChanged, inputToolBar->inputLineEdit, [this, inputToolBar](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles) {
        if (roles.contains(Qt::EditRole)) {
            auto newText = tableModel->data(topLeft, Qt::EditRole).value<QString>();
            inputToolBar->inputLineEdit->setText(newText);
        }
    });
}

void Window::about() {
    QMessageBox::about(this, "About Application",
                       "MiniExcel is a very simple table viewer and editor\n"
                       "Available operations:\n"
                       "+x : unary plus\n"
                       "-x : unary minus\n"
                       "x+y : addition\n"
                       "x-y : subtraction\n"
                       "x*y : multiplication\n"
                       "x/y : division\n"
                       "mmin(x1,x2,...,xn) : minimum\n"
                       "mmax(x1,x2,...,xn) : maximum ");
}
