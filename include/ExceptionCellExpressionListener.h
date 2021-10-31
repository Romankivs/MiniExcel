#pragma once
#include <BaseErrorListener.h>

using namespace antlr4;

class ExceptionCellExpressionListener : public BaseErrorListener {
    void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line, size_t charPositionInLine,
                     const std::string &msg, std::exception_ptr e) override;
};
