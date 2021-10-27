#include "ExceptionCellExpressionListener.h"

void ExceptionCellExpressionListener::syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
                                                  size_t charPositionInLine, const std::string &msg,
                                                  std::exception_ptr e) {
    throw std::runtime_error(msg);
}
