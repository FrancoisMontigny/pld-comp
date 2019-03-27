#include "CInstrVariable.h"

CInstrVariable::CInstrVariable(string name) :
        name(name), expr(nullptr) {
}

CInstrVariable::CInstrVariable(string name, CExpression* expr_) :
        name(name) {
    CExpressionVar* exprVar = new CExpressionVar(name);
    expr = new CExpressionComposed(exprVar, "=", expr_);
}

CInstrVariable::~CInstrVariable() {
    if (expr != nullptr) {
        delete expr;
    }
}

string CInstrVariable::to_asm(const CFunction* f) const {
    if (expr == nullptr) {
        return "";
    } else {
        return expr->to_asm(f).first;
    }
}

string CInstrVariable::getType() const{
    return type;
}

string CInstrVariable::getName() const{
    return name;
}

void CInstrVariable::setType(string type) {
    this->type = type;
}

