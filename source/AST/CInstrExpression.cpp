#include "CInstrExpression.h"

#include "CExpression.h"

CInstrExpression::CInstrExpression(CExpression* expr)
: expr(expr)
{
}

CInstrExpression::~CInstrExpression() {
    if (expr != nullptr) {
        delete expr;
    }
}

void CInstrExpression::optimize()
{
    if (expr != nullptr) {
        CExpression* opti = expr->optimize();
        if (opti != nullptr) {
            delete expr;
            expr = opti;
        }
    }
}

