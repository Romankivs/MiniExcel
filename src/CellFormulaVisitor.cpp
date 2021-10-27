#include <cmath>
#include <limits>
#include "CellFormulaVisitor.h"
#include "tablemodel.h"

CellFormulaVisitor::CellFormulaVisitor(TableModel *table) : table(table)
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
    if (ctx->op->getType() == CellExpressionParser::ADD)
        return visit(ctx->expr());
    else
    {
        double res = visit(ctx->expr());
        return res * -1;
    }
}

antlrcpp::Any CellFormulaVisitor::visitMulOrDiv(CellExpressionParser::MulOrDivContext *ctx) {
    double left = visit(ctx->left);
    double right = visit(ctx->right);
    if (ctx->op->getType() == CellExpressionParser::MUL)
        return left * right;
    else
        return left / right;
}

antlrcpp::Any CellFormulaVisitor::visitPow(CellExpressionParser::PowContext *ctx) {
    double left = visit(ctx->left);
    double right = visit(ctx->right);
    return pow(left, right);
}

antlrcpp::Any CellFormulaVisitor::visitCellName(CellExpressionParser::CellNameContext *ctx) {
    double value;
    table->data(QModelIndex(), Qt::UserRole);
    value = 1;
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

