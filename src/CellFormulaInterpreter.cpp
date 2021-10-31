#include "CellFormulaInterpreter.h"
#include "tablemodel.h"

CellFormulaInterpreter::CellFormulaInterpreter(TableModel *model) : model(model) {
}

QString CellFormulaInterpreter::interpret(QString string, QModelIndex curIndex) const {
    if (string.isEmpty())
        return QString("0");
    antlr4::ANTLRInputStream input(string.toStdString());
    ExceptionCellExpressionListener errListener;
    antlr4::CellExpressionLexer lexer(&input);
    lexer.removeErrorListeners();
    lexer.addErrorListener(&errListener);
    antlr4::CommonTokenStream tokens(&lexer);
    antlr4::CellExpressionParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&errListener);
    CellFormulaVisitor visitor(model, curIndex);
    try {
        double res = visitor.visit(parser.start());
        return QString::number(res);
    } catch (std::runtime_error &e) {
        model->setData(curIndex, true, AddRoles::ExceptionState);
        return QString(e.what());
    }
}
