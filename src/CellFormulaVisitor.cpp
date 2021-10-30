#include <cmath>
#include <limits>
#include "CellFormulaVisitor.h"
#include "tablemodel.h"

CellFormulaVisitor::CellFormulaVisitor(TableModel *table, QModelIndex curIndex) :
    table(table), interpretedCellIndex(curIndex)
{
}

antlrcpp::Any CellFormulaVisitor::visitStart(CellExpressionParser::StartContext *ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any CellFormulaVisitor::visitAddOrSub(CellExpressionParser::AddOrSubContext *ctx) {
    double left = visit(ctx->left);
    double right = visit(ctx->right);
    if (ctx->op->getType() == antlr4::CellExpressionParser::ADD)
        return left + right;
    else
        return left - right;
}

antlrcpp::Any CellFormulaVisitor::visitNumber(CellExpressionParser::NumberContext *ctx) {
    return atof(ctx->getText().c_str());
}

antlrcpp::Any CellFormulaVisitor::visitParanthesis(CellExpressionParser::ParanthesisContext *ctx) {
    return visit(ctx->inner);
}

antlrcpp::Any CellFormulaVisitor::visitUnaryMinOrPlus(CellExpressionParser::UnaryMinOrPlusContext *ctx) {
    double res = visit(ctx->expr());
    if (ctx->op->getType() == CellExpressionParser::ADD)
        return res;
    else
        return res * -1;
}

antlrcpp::Any CellFormulaVisitor::visitMulOrDiv(CellExpressionParser::MulOrDivContext *ctx) {
    double left = visit(ctx->left);
    double right = visit(ctx->right);
    if (ctx->op->getType() == CellExpressionParser::MUL)
        return left * right;
    else
    {
        if (fabs(right - 0) <= EPSILON)
            throw std::runtime_error("Division by 0");
        return left / right;
    }
}

antlrcpp::Any CellFormulaVisitor::visitPow(CellExpressionParser::PowContext *ctx) {
    double left = visit(ctx->left);
    double right = visit(ctx->right);
    return pow(left, right);
}

antlrcpp::Any CellFormulaVisitor::visitCellName(CellExpressionParser::CellNameContext *ctx) {
    std::string cellName = ctx->CELL_NAME()->getText().c_str();
    size_t columnLast = cellName.find_first_of("0123456789");
    QString columnName = QString::fromStdString(cellName.substr(0, columnLast));
    QString rowName = QString::fromStdString(cellName.substr(columnLast));
    int columnIndex = table->columnNameToNumber(columnName);
    int rowIndex = rowName.toInt();

    QVector<QPersistentModelIndex> dependentCells = table->data(table->index(rowIndex, columnIndex),
                                                    AddRoles::Dependent).value<QVector<QPersistentModelIndex>>();
    if (!dependentCells.contains(interpretedCellIndex))
        dependentCells.push_back(interpretedCellIndex);
    table->setData(table->index(rowIndex, columnIndex), QVariant::fromValue(dependentCells), AddRoles::Dependent);

    QVector<QModelIndex> dependsOnCells = table->data(interpretedCellIndex, AddRoles::DependsOn).value<QVector<QModelIndex>>();
    if (!dependsOnCells.contains(table->index(rowIndex, columnIndex)))
        dependsOnCells.push_back(table->index(rowIndex, columnIndex));
    table->setData(interpretedCellIndex, QVariant::fromValue(dependsOnCells), AddRoles::DependsOn);

    if (table->data(table->index(rowIndex, columnIndex), AddRoles::ExceptionState).value<bool>())
    {
        throw std::runtime_error(table->data(table->index(rowIndex, columnIndex), Qt::DisplayRole).value<QString>().toStdString());
    }

    double value = 0;
    QVariant cellVal = table->data(table->index(rowIndex, columnIndex), Qt::DisplayRole);
    if (cellVal.isValid() && !cellVal.toString().isEmpty()) {
        bool ok;
        value = cellVal.toDouble(&ok);
        if (!ok) {
            throw std::runtime_error("Wrong value type");
        }
    }
    return value;
}

antlrcpp::Any CellFormulaVisitor::visitMmaxOrMmin(CellExpressionParser::MmaxOrMminContext *ctx) {
    auto expressions = ctx->expr();
    if (ctx->op->getType() == CellExpressionParser::MMAX)
    {
        double max = std::numeric_limits<double>::min();
        for (auto& expr : expressions)
        {
            double num = visit(expr);
            if (num >= max)
                max = num;
        }
        return max;
    }
    else
    {
        double min = std::numeric_limits<double>::max();
        for (auto& expr : expressions)
        {
            double num = visit(expr);
            if (num <= min)
                min = num;
        }
        return min;
    }
}

