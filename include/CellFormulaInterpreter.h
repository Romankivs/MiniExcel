#pragma once
#include "CellFormulaVisitor.h"
#include "ExceptionCellExpressionListener.h"
#include <CellExpressionLexer.h>
#include <CellExpressionParser.h>
#include <QString>
#include <antlr4-runtime.h>

class CellFormulaInterpreter {
public:
    explicit CellFormulaInterpreter(TableModel *model);
    QString interpret(QString string, QModelIndex curIndex) const;

private:
    TableModel *model;
};
