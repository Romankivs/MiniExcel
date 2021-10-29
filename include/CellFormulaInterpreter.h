#pragma once
#include <QString>
#include <antlr4-runtime.h>
#include <CellExpressionLexer.h>
#include <CellExpressionParser.h>
#include "CellFormulaVisitor.h"
#include "ExceptionCellExpressionListener.h"

class CellFormulaInterpreter {
public:
    explicit CellFormulaInterpreter(TableModel* model);
    QString interpret(QString string, QModelIndex curIndex) const;
private:
    TableModel* model;
};
