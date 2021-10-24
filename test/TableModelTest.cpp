#include <gtest/gtest.h>
#include "tablemodel.h"
#include "cell.h"
#include <QFileDialog>
#include <QFile>

using namespace testing;

TEST(TableModelTest, saveAndLoadToFile)
{
    /*cell tableCell("Hello", Qt::AlignCenter, QFont("Arial"), QColor(0, 0, 0), QColor(255, 255, 255));
    QVector<QVector<cell>> tableInitData = {{tableCell, tableCell, {}}, {{}, {}, tableCell}};
    TableModel classUnderTest(tableInitData);
    QString fileName = QFileDialog::getOpenFileName(nullptr,
        "Open MiniExcel Table", "/", "MiniExcel Files (*.mex)");*/
}


TEST(TableModelTest, rowCountHandleNonEmptyTable)
{
    QVector<QVector<cell>> tableInitData = {{{}, {}, {}}, {{}, {}, {}}};
    TableModel classUnderTest(tableInitData);
    size_t rowCount = classUnderTest.rowCount(QModelIndex());
    ASSERT_EQ(rowCount, 2);
}

TEST(TableModelTest, rowCountHandleEmptyTable)
{
    QVector<QVector<cell>> tableInitData;
    TableModel classUnderTest(tableInitData);
    size_t rowCount = classUnderTest.rowCount(QModelIndex());
    ASSERT_EQ(rowCount, 0);
}

TEST(TableModelTest, columnCountHandleNonEmptyTable)
{
    QVector<QVector<cell>> tableInitData = {{{}, {}, {}}, {{}, {}, {}}};
    TableModel classUnderTest(tableInitData);
    size_t columnCount = classUnderTest.columnCount(QModelIndex());
    ASSERT_EQ(columnCount, 3);
}

TEST(TableModelTest, columnCountHandleEmptyTable)
{
    QVector<QVector<cell>> tableInitData;
    TableModel classUnderTest(tableInitData);
    size_t columnCount = classUnderTest.columnCount(QModelIndex());
    ASSERT_EQ(columnCount, 0);
}
