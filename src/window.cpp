#include "window.h"
#include <antlr4-runtime.h>
#include <CellExpressionLexer.h>
#include <CellExpressionParser.h>
#include "CellFormulaVisitor.h"

Window::Window()
{
    QVector<QVector<cell>> tableInitData = {{{}, {}, {}}, {{}, {}, {}}};
    tableModel = new TableModel(tableInitData);
    tableView = new QTableView(this);
    tableView->setModel(tableModel);
    setCentralWidget(tableView);
    connect(tableView->selectionModel(),  SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectionChanged(QItemSelection,QItemSelection)));

    QHeaderView* verHeader = tableView->verticalHeader();
    verHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(verHeader, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(verHeaderCustomContextMenu(QPoint)));
    QHeaderView* horHeader = tableView->horizontalHeader();
    horHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(horHeader, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(horHeaderCustomContextMenu(QPoint)));

    createActions();
    createToolBars();

    antlr4::ANTLRInputStream input(std::string("mmax(3,5,(10-44)) + 3^+4"));
    antlr4::CellExpressionLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    antlr4::CellExpressionParser parser(&tokens);
    CellFormulaVisitor visitor;
    double result = visitor.visit(parser.start());
    QMessageBox::warning(this, "Warning", QString::number(result));
}

Window::~Window()
{
    delete tableModel;
}

void Window::verHeaderCustomContextMenu(const QPoint& point)
{
    QMenu* contextMenu = new QMenu("Rows", this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* remRowAction = contextMenu->addAction(QIcon(":/icons/tableRowRemoval.png"), "Remove row");
    connect(remRowAction, &QAction::triggered, this, [point, this](){tableModel->removeRows(tableView->verticalHeader()->logicalIndexAt(point), 1);});
    QAction* insRowAction = contextMenu->addAction(QIcon(":/icons/tableRowAddition.png"), "Insert row");
    connect(insRowAction, &QAction::triggered, this, [point, this](){tableModel->insertRows(tableView->verticalHeader()->logicalIndexAt(point), 1);});
    contextMenu->popup(mapToGlobal(point));
}

void Window::horHeaderCustomContextMenu(const QPoint& point)
{
    QMenu* contextMenu = new QMenu("Columns", this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* remColAction = contextMenu->addAction(QIcon(":/icons/tableColumnRemoval.png"), "Remove column");
    connect(remColAction, &QAction::triggered, this, [point, this](){tableModel->removeColumns(tableView->horizontalHeader()->logicalIndexAt(point), 1);});
    QAction* insColAction = contextMenu->addAction(QIcon(":/icons/tableColumnAddition.png"), "Insert column");
    connect(insColAction, &QAction::triggered, this, [point, this](){tableModel->insertColumns(tableView->horizontalHeader()->logicalIndexAt(point), 1);});
    contextMenu->popup(mapToGlobal(point));
}

void Window::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    const QModelIndexList selectedIndexes = selected.indexes();
    if (selectedIndexes.isEmpty())
        return;
    QModelIndex firstSelected = selectedIndexes.front();
    QFont changedFont = tableModel->data(firstSelected, Qt::FontRole).value<QFont>();
    Q_EMIT currentFontChanged(changedFont);
    Q_EMIT currentFontSizeChanged(changedFont.pointSize());
    Q_EMIT currentFontIsBoldChanged(changedFont.bold());
    Q_EMIT currentFontIsItalicChanged(changedFont.italic());
    Q_EMIT currentFontIsStrikethroughChanged(changedFont.strikeOut());
}

void Window::fontChangedFromComboBox(const QFont &font)
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    }
    else
    {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto& index : selectedTableIndexes)
        {
            tableModel->setData(index, font, Qt::FontRole);
        }
    }
}

void Window::fontSizeChanged(int fontSize)
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    }
    else
    {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto& index : selectedTableIndexes)
        {
            QFont currentCellFont = tableModel->data(index, Qt::FontRole).value<QFont>();
            currentCellFont.setPointSize(fontSize);
            tableModel->setData(index, currentCellFont, Qt::FontRole);
        }
    }
}

void Window::fontStyleChanged(FontStyleOptions style, bool value)
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    }
    else
    {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto& index : selectedTableIndexes)
        {
            QFont currentCellFont = tableModel->data(index, Qt::FontRole).value<QFont>();
            switch(style)
            {
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

void Window::save()
{
    if (!openedFileName.isEmpty())
    {
        if (!tableModel->saveToFile(openedFileName))
        {
            QMessageBox::critical(this, "Save error", "Such file cannot be created");
        }
    }
    else
    {
        QString fileNameToSave = QInputDialog::getText(this, "Save", "Enter new file name");
        if (fileNameToSave.isEmpty())
            return;
        if (!tableModel->saveToFile(fileNameToSave))
        {
            QMessageBox::critical(this, "Save error", "Such file cannot be created");
        }
        else
        {
            openedFileName = fileNameToSave;
        }
    }
}

void Window::saveAs()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open MiniExcel Table"), "/", "MiniExcel Files (*.mex)");
    if (fileName.isEmpty())
        return;
    if (!tableModel->saveToFile(fileName))
    {
        QMessageBox::critical(this, "Save As error", "Error saving to such file");
    }
    else
    {
        openedFileName = fileName;
    }
}

void Window::load()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open MiniExcel Table"), "/", "MiniExcel Files (*.mex)");
    if (fileName.isEmpty())
        return;
    if (!tableModel->loadFromFile(fileName))
    {
        QMessageBox::critical(this, "Load failure", "Cannot load such file");
    }
    else
    {
        openedFileName = fileName;
    }
}

void Window::addRows()
{
    int rowsCount = QInputDialog::getInt(this, "Add Rows", "Enter number of rows to add.", 1, 1);
    if (!tableView->selectionModel()->hasSelection())
    {
        tableModel->insertRows(0, rowsCount);
    }
    else
    {
        QModelIndexList selectedRows = tableView->selectionModel()->selectedIndexes();
        std::sort(selectedRows.begin(), selectedRows.end(), [](const QModelIndex& a, const QModelIndex& b) { return a.row() < b.row();});
        tableModel->insertRows(selectedRows.constFirst().row(), rowsCount);
    }
}

void Window::removeRows()
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No rows were selected!");
    }
    else
    {
        QModelIndexList selectedRows = tableView->selectionModel()->selectedIndexes();
        QModelIndexList::iterator last = std::unique(selectedRows.begin(), selectedRows.end(),
                               [](const QModelIndex& a, const QModelIndex& b){ return a.row() == b.row();});
        selectedRows.erase(last, selectedRows.end());
        std::sort(selectedRows.begin(), selectedRows.end(), [](const QModelIndex& a, const QModelIndex& b) { return a.row() < b.row();});
        tableModel->removeRows(selectedRows.constFirst().row(), selectedRows.size());
    }
}

void Window::addColumns()
{
    int columnsCount = QInputDialog::getInt(this, "Add Columns", "Enter number of columns to add.", 1, 1);
    if (!tableView->selectionModel()->hasSelection())
    {
        tableModel->insertColumns(0, columnsCount);
    }
    else
    {
        QModelIndexList selectedColumns = tableView->selectionModel()->selectedIndexes();
        std::sort(selectedColumns.begin(), selectedColumns.end(), [](const QModelIndex& a, const QModelIndex& b) { return a.column() < b.column();});
        tableModel->insertColumns(selectedColumns.constFirst().column(), columnsCount);
    }
}

void Window::removeColumns()
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No columns were selected!");
    }
    else
    {
        QModelIndexList selectedColumns = tableView->selectionModel()->selectedIndexes();
        QModelIndexList::iterator last = std::unique(selectedColumns.begin(), selectedColumns.end(),
                               [](const QModelIndex& a, const QModelIndex& b){ return a.column() == b.column();});
        selectedColumns.erase(last, selectedColumns.end());
        std::sort(selectedColumns.begin(), selectedColumns.end(), [](const QModelIndex& a, const QModelIndex& b) { return a.column() < b.column();});
        tableModel->removeColumns(selectedColumns.constFirst().column(), selectedColumns.size());
    }
}

void Window::chooseColor(const Qt::ItemDataRole &role)
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    }
    else
    {
        QColor colorChoosen = QColorDialog::getColor();
        if (!colorChoosen.isValid())
            return;
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto& index : selectedTableIndexes)
        {
            tableModel->setData(index, colorChoosen, role);
        }
    }
}

void Window::chooseHorizontalAlignment(const Qt::Alignment alignment)
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    }
    else
    {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto& index : selectedTableIndexes)
        {
            Qt::Alignment verticalAlignment = tableModel->data(index, Qt::TextAlignmentRole).value<Qt::Alignment>() & Qt::AlignVertical_Mask;
            Qt::Alignment resultingAlignment = (alignment & Qt::AlignHorizontal_Mask) | verticalAlignment;
            tableModel->setData(index, QVariant::fromValue(resultingAlignment), Qt::TextAlignmentRole);
        }
    }
}

void Window::chooseVerticalAlignment(const Qt::Alignment alignment)
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    }
    else
    {
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto& index : selectedTableIndexes)
        {
            Qt::Alignment horizontalAlignment = tableModel->data(index, Qt::TextAlignmentRole).value<Qt::Alignment>() & Qt::AlignHorizontal_Mask;
            Qt::Alignment resultingAlignment = (alignment & Qt::AlignVertical_Mask) | horizontalAlignment;
            tableModel->setData(index, QVariant::fromValue(resultingAlignment), Qt::TextAlignmentRole);
        }
    }
}

void Window::changeFontDialog()
{
    if (!tableView->selectionModel()->hasSelection())
    {
        QMessageBox::warning(this, "Warning", "No indexes were selected!");
    }
    else
    {
        bool ok;
        QFont fontChoosen = QFontDialog::getFont(&ok);
        if (!ok)
            return;
        QModelIndexList selectedTableIndexes = tableView->selectionModel()->selectedIndexes();
        for (const auto& index : selectedTableIndexes)
        {
            tableModel->setData(index, fontChoosen, Qt::FontRole);
        }
        Q_EMIT currentFontChanged(fontChoosen);
        Q_EMIT currentFontSizeChanged(fontChoosen.pointSize());
        Q_EMIT currentFontIsBoldChanged(fontChoosen.bold());
        Q_EMIT currentFontIsItalicChanged(fontChoosen.italic());
        Q_EMIT currentFontIsStrikethroughChanged(fontChoosen.strikeOut());
    }
}

void Window::changeSpinBoxValueWithoutSignaling(QSpinBox *spinBox, int value)
{
    spinBox->blockSignals(true);
    spinBox->setValue(value);
    spinBox->blockSignals(false);
}

void Window::createActions()
{
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    connect(saveAction, &QAction::triggered, this, [this](){save();});
    QAction* saveAsAction = fileMenu->addAction("Save As");
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAction, &QAction::triggered, this, [this](){saveAs();});
    QAction* loadAction = fileMenu->addAction("Load");
    loadAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    connect(loadAction, &QAction::triggered, this, [this](){load();});

    QMenu* helpMenu = menuBar()->addMenu("Help");
    QAction* aboutAction = helpMenu->addAction("About");
    aboutAction->setShortcut(QKeySequence(Qt::Key_F1));
    connect(aboutAction, &QAction::triggered, this, [this](){about();});
}

void Window::createToolBars()
{
    QToolBar* mainToolBar = addToolBar("Main ToolBar");
    mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    mainToolBar->setMovable(true);

    createTableRowsAndColumnsToolButtons(mainToolBar);
    createColorToolButtons(mainToolBar);
    createTextAlignmentToolButtons(mainToolBar);
    createFontToolButtonsAndWidgets(mainToolBar);
}

void Window::createTableRowsAndColumnsToolButtons(QToolBar* toolBar)
{
    QAction* addRowsAction = toolBar->addAction(QIcon(":/icons/tableRowAddition.png"), "Add Rows");
    connect(addRowsAction, &QAction::triggered, this, [this](){addRows();});
    QAction* removeRowsAction = toolBar->addAction(QIcon(":/icons/tableRowRemoval.png"), "Remove Rows");
    connect(removeRowsAction, &QAction::triggered, this, [this](){removeRows();});

    QAction* addColumnsAction = toolBar->addAction(QIcon(":/icons/tableColumnAddition.png"), "Add Columns");
    connect(addColumnsAction, &QAction::triggered, this, [this](){addColumns();});
    QAction* removeColumnsAction = toolBar->addAction(QIcon(":/icons/tableColumnRemoval.png"), "Remove Columns");
    connect(removeColumnsAction, &QAction::triggered, this, [this](){removeColumns();});

    toolBar->addSeparator();
}

void Window::createColorToolButtons(QToolBar *toolBar)
{
    QAction* backgroundColorChooseAction = toolBar->addAction(QIcon(":/icons/ColorChooseIcon.png"), "Background Color");
    connect(backgroundColorChooseAction, &QAction::triggered, this, [this](){chooseColor(Qt::BackgroundRole);});
    QAction* textColorChooseAction = toolBar->addAction(QIcon(":/icons/TextColorChooseIcon.png"), "Text Color");
    connect(textColorChooseAction, &QAction::triggered, this, [this](){chooseColor(Qt::ForegroundRole);});

    toolBar->addSeparator();
}

void Window::createTextAlignmentToolButtons(QToolBar *toolBar)
{
    QToolButton* horizontalTextAlignmentButton = new QToolButton;
    horizontalTextAlignmentButton->setIcon(QIcon(":/icons/textAlignmentLeft.png"));
    horizontalTextAlignmentButton->setToolTip("Horizontal Text Alignment");
    toolBar->addWidget(horizontalTextAlignmentButton);
    horizontalTextAlignmentButton->setPopupMode(QToolButton::InstantPopup);
    QMenu* horizontalTextAlignMenu = new QMenu(horizontalTextAlignmentButton);
    horizontalTextAlignmentButton->setMenu(horizontalTextAlignMenu);

    QAction* alignLeft = horizontalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentLeft.png"), "Left");
    connect(alignLeft, &QAction::triggered, this, [this](){chooseHorizontalAlignment(Qt::AlignLeft);});
    QAction* alignCenter = horizontalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentHCenter.png"), "Center");
    connect(alignCenter, &QAction::triggered, this, [this](){chooseHorizontalAlignment(Qt::AlignHCenter);});
    QAction* alignRight = horizontalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentRight.png"), "Right");
    connect(alignRight, &QAction::triggered, this, [this](){chooseHorizontalAlignment(Qt::AlignRight);});

    QToolButton* verticalTextAlignmentButton = new QToolButton;
    verticalTextAlignmentButton->setIcon(QIcon(":/icons/textAlignmentUp.png"));
    verticalTextAlignmentButton->setToolTip("Vertical Text Alignment");
    toolBar->addWidget(verticalTextAlignmentButton);
    verticalTextAlignmentButton->setPopupMode(QToolButton::InstantPopup);
    QMenu* verticalTextAlignMenu = new QMenu(verticalTextAlignmentButton);
    verticalTextAlignmentButton->setMenu(verticalTextAlignMenu);

    QAction* alignUp = verticalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentUp.png"), "Up");
    connect(alignUp, &QAction::triggered, this, [this](){chooseVerticalAlignment(Qt::AlignTop);});
    QAction* alignVCenter =  verticalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentVCenter.png"), "Center");
    connect(alignVCenter, &QAction::triggered, this, [this](){chooseVerticalAlignment(Qt::AlignVCenter);});
    QAction* alignDown =  verticalTextAlignMenu->addAction(QIcon(":/icons/textAlignmentDown.png"), "Down");
    connect(alignDown, &QAction::triggered, this, [this](){chooseVerticalAlignment(Qt::AlignBottom);});

    toolBar->addSeparator();
}

void Window::createFontToolButtonsAndWidgets(QToolBar *toolBar)
{
    QAction* fontDialog = toolBar->addAction(QIcon(":/icons/fontDialog.png"), "Font Settings");
    connect(fontDialog, &QAction::triggered, this, [this](){changeFontDialog();});

    QFontComboBox* fontBox = new QFontComboBox;
    connect(this, SIGNAL(currentFontChanged(QFont)), fontBox, SLOT(setCurrentFont(QFont)));
    connect(fontBox, &QComboBox::activated, this,
            [this, fontBox](int index){fontChangedFromComboBox(QFont(fontBox->itemText(index)));});
    toolBar->addWidget(fontBox);

    QSpinBox* fontSizeSpinBox= new QSpinBox;
    constexpr int minimalFontSize = 6;
    constexpr int maximumFontSize = 36;
    fontSizeSpinBox->setMinimum(minimalFontSize);
    fontSizeSpinBox->setMaximum(maximumFontSize);
    connect(this, &Window::currentFontSizeChanged, fontBox,
            [this, fontSizeSpinBox](int value){changeSpinBoxValueWithoutSignaling(fontSizeSpinBox, value);});
    connect(fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(fontSizeChanged(int)));
    toolBar->addWidget(fontSizeSpinBox);

    toolBar->addSeparator();

    QAction* boldFont = toolBar->addAction(QIcon(":/icons/boldFont.png"), "Bold");
    boldFont->setCheckable(true);
    connect(this, SIGNAL(currentFontIsBoldChanged(bool)), boldFont, SLOT(setChecked(bool)));
    connect(boldFont, &QAction::triggered, this, [this](bool value){fontStyleChanged(FontStyleOptions::Bold, value);});

    QAction* italicsFont = toolBar->addAction(QIcon(":/icons/italicsFont.png"), "Italic");
    italicsFont->setCheckable(true);
    connect(this, SIGNAL(currentFontIsItalicChanged(bool)), italicsFont, SLOT(setChecked(bool)));
    connect(italicsFont, &QAction::triggered, this, [this](bool value){fontStyleChanged(FontStyleOptions::Italic, value);});

    QAction* strikethroughFont = toolBar->addAction(QIcon(":/icons/strikethroughFont.png"), "Strikethrough");
    connect(this, SIGNAL(currentFontIsStrikethroughChanged(bool)), strikethroughFont, SLOT(setChecked(bool)));
    connect(strikethroughFont, &QAction::triggered, this, [this](bool value){fontStyleChanged(FontStyleOptions::Strikethrough, value);});
    strikethroughFont->setCheckable(true);

    toolBar->addSeparator();
}

void Window::about()
{
    QMessageBox::about(this, "About Application",
             "MiniExcel is a very simple table viewer and editor ");
}
