#pragma once

#include <string>
#include <list>
using std::string;
using std::list;

#include "CInstruction.h"
#include "CType.h"
class CExpression;

class CInstrArray : public CInstruction {
public:
    CInstrArray(string name, int size);
    CInstrArray(string name, int size, list<CExpression*> exprs);
    ~CInstrArray();
    void to_IR(CFG* cfg) const;
    string to_asm(const CFunction* f) const;
    
    CType type;
    string name;
    int size;
    list <CExpression*> exprs;
    
private:
    // no copy wanted
    CInstrArray(const CInstrArray&); // no implementation
    CInstrArray& operator=(const CInstrArray&); // no implementation
};

