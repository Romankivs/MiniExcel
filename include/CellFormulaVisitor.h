#pragma once
#include "CellExpressionBaseVisitor.h"
#include <QModelIndex>

using namespace antlr4;

constexpr double EPSILON = 1e-6;

class TableModel;

class CellFormulaVisitor : public CellExpressionBaseVisitor
{
public:
    CellFormulaVisitor(TableModel* table, QModelIndex curIndex);
private:
    antlrcpp::Any visitStart(CellExpressionParser::StartContext *ctx) override;
    antlrcpp::Any visitAddOrSub(CellExpressionParser::AddOrSubContext *ctx) override;
    antlrcpp::Any visitNumber(CellExpressionParser::NumberContext *ctx) override;
    antlrcpp::Any visitParanthesis(CellExpressionParser::ParanthesisContext *ctx) override;
    antlrcpp::Any visitUnaryMinOrPlus(CellExpressionParser::UnaryMinOrPlusContext *ctx) override;
    antlrcpp::Any visitMulOrDiv(CellExpressionParser::MulOrDivContext *ctx) override;
    antlrcpp::Any visitPow(CellExpressionParser::PowContext *ctx) override;
    antlrcpp::Any visitCellName(CellExpressionParser::CellNameContext *ctx);
    antlrcpp::Any visitMmaxOrMmin(CellExpressionParser::MmaxOrMminContext *ctx) override;

    TableModel* table;
    QModelIndex interpretedCellIndex;
};