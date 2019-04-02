#pragma once

#include <string>
using std::string;

#include <utility>
using std::pair;

class CFunction;
class CFG;

class CExpression {
public:
    virtual ~CExpression();
    virtual string to_IR(CFG* cfg) const = 0;
    virtual pair<string, string> to_asm(CFunction* f) const = 0;
    pair<string, string> to_asm(const CFunction* f) const;
};

class CExpressionInt: public CExpression {
public:
    CExpressionInt(int value);
    ~CExpressionInt();
    string to_IR(CFG* cfg) const;
    pair<string, string> to_asm(CFunction* f) const;
    
    int value;
};

class CExpressionVar: public CExpression {
public:
    CExpressionVar(string variable);
    ~CExpressionVar();
    string to_IR(CFG* cfg) const;
    pair<string, string> to_asm(CFunction* f) const;

    string variable;
};

class CExpressionComposed: public CExpression {
public:
    CExpressionComposed(CExpression* lhs, string op, CExpression* rhs);
    ~CExpressionComposed();
    string to_IR(CFG* cfg) const;
    pair<string, string> to_asm(CFunction* f) const;

    CExpression* lhs;
    string op;
    CExpression* rhs;

private:
    // no copy wanted
    CExpressionComposed(const CExpressionComposed&); // no implementation
    CExpressionComposed& operator=(const CExpressionComposed&); // no implementation
};

