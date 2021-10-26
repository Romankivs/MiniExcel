#pragma once
#include "CellExpressionBaseVisitor.h"

using namespace antlr4;

class CellFormulaVisitor : public CellExpressionBaseVisitor
{
    antlrcpp::Any visitStart(CellExpressionParser::StartContext *ctx) override;
    antlrcpp::Any visitAddOrSub(CellExpressionParser::AddOrSubContext *ctx) override;
    antlrcpp::Any visitNumber(CellExpressionParser::NumberContext *ctx) override;
    antlrcpp::Any visitParanthesis(CellExpressionParser::ParanthesisContext *ctx) override;
    antlrcpp::Any visitUnaryMinOrPlus(CellExpressionParser::UnaryMinOrPlusContext *ctx) override;
    antlrcpp::Any visitMulOrDiv(CellExpressionParser::MulOrDivContext *ctx) override;
    antlrcpp::Any visitPow(CellExpressionParser::PowContext *ctx) override;
    antlrcpp::Any visitMmaxOrMmin(CellExpressionParser::MmaxOrMminContext *ctx) override;
};