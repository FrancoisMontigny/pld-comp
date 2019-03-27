#pragma once

#include <string>
using std::string;

#include "CInstruction.h"
#include "CExpression.h"
typedef string CType;

class CInstrVariable: public CInstruction {
public:
    CInstrVariable(string name);
    CInstrVariable(string name, CExpression * expr);
    ~CInstrVariable();
    string to_asm(const CFunction * f) const;
    string getType() const;
    string getName() const;
    void setType(string type);
    
    CType type;
    string name;
    CExpression* expr;

private:
    // no copy wanted
    CInstrVariable(const CInstrVariable&); // no implementation
    CInstrVariable& operator=(const CInstrVariable&); // no implementation
};

