#include <QApplication>
#include <gtest/gtest.h>
#include <qtimer.h>
#include "Window.h"

TEST(MainTests, saveAndLoadTest) {
    TableModel table;
    table.insertRows(0, 5);
    table.insertColumns(0, 5);
    QModelIndex index = table.index(1, 1);
    table.setData(index, QString("Hello"), Qt::EditRole);
    table.setData(index, QFont("Hack"), Qt::FontRole);
    table.setData(index, QColor(0, 0, 0), Qt::BackgroundRole);
    table.setData(index, QColor(255, 255, 255), Qt::ForegroundRole);
    QModelIndex indexA0 = table.index(0, 0);
    table.setData(indexA0, QString("10"), Qt::EditRole);

    QString fileName = "testing.mex";
    QVector<QVector<Cell>> oldTableData = table.getTableData();
    table.saveToFile(fileName);
    table.loadFromFile(fileName);
    QVector<QVector<Cell>> newTableData = table.getTableData();
    ASSERT_EQ(oldTableData, newTableData);
}

TEST(MainTests, interpreterTest) {
    TableModel table;
    table.insertRows(0, 1);
    table.insertColumns(0, 2);
    table.setData(table.index(0, 0), "B0 + 1", Qt::EditRole);
    table.setData(table.index(0, 0), true, AddRoles::Formula);
    table.setData(table.index(0, 1), "33", Qt::EditRole);
    CellFormulaInterpreter interpreter(&table);

    QString unaryOperations = "--1 + ---1 + ++5";
    QString result = interpreter.interpret(unaryOperations, QModelIndex());
    EXPECT_EQ(result, QString("5"));

    QString binaryOperations = "100 * 100 - 100 / (1/100)";
    result = interpreter.interpret(binaryOperations, QModelIndex());
    EXPECT_EQ(result, QString("0"));

    QString mminAndMmaxOperations = "mmin(10-10, 1*-1, 0) + mmax(3, mmin(0, 1, -2)) / mmin(1,1)";
    result = interpreter.interpret(mminAndMmaxOperations, QModelIndex());
    EXPECT_EQ(result, QString("2"));

    QString cellNames = "mmax(B0, A0) - mmin(A0, B0)";
    result = interpreter.interpret(cellNames, QModelIndex());
    EXPECT_EQ(result, QString("1"));
}

TEST(MainTests, interpreterTestSpecial) {
    TableModel table;
    CellFormulaInterpreter interpreter(&table);

    QString empty = "";
    QString result = interpreter.interpret(empty, QModelIndex());
    EXPECT_EQ(result, QString("0"));

    QString divByZero = "1/0";
    result = interpreter.interpret(divByZero , QModelIndex());
    EXPECT_EQ(result, QString("Division by 0"));

    QString tokenFailure = "o";
    result = interpreter.interpret(tokenFailure, QModelIndex());
    EXPECT_EQ(result, QString("token recognition error at: 'o'"));

    QString cellNameOutOfBounds = "A1000";
    result = interpreter.interpret(cellNameOutOfBounds, QModelIndex());
    EXPECT_EQ(result, QString("0"));
}


int main(int argc, char *argv[]) {
    QApplication app{argc, argv};

    QTimer::singleShot(0, [&]() {
        ::testing::InitGoogleTest(&argc, argv);
        auto testResult = RUN_ALL_TESTS();
        app.exit(testResult);
    });

    return app.exec();
}
