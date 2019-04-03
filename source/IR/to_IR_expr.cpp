#include "IR.h"
#include "../AST/CExpression.h"
#include "../AST/CFunctionCall.h"

#include <iostream>
using std::ostream;
using std::cerr;
using std::endl;
#include <string>
using std::string;
using std::to_string;

string CExpressionInt::to_IR(CFG* cfg) const {
    BasicBlock* bb = cfg->current_bb;
    
    string variable = cfg->tos_add_temp("int");
    
    bb->add_IRInstr(op_ldconst, "int", {variable, to_string(value)});
    
    return variable;
}

string CExpressionVar::to_IR(CFG* cfg) const {
    return variable;
}

string CExpressionComposed::to_IR(CFG* cfg) const {
    BasicBlock* bb = cfg->current_bb;
    
    string lhsvar = lhs->to_IR(cfg);
    string rhsvar = rhs->to_IR(cfg);
    
    string variable;
    
    if (op == "=") {
        variable = lhsvar;
        /*
        code += "  movl  " + rhsvar + ", %eax\n";
        code += "  movl  %eax, " + lhsvar + "\n";
        */
    } else {
        variable = cfg->tos_add_temp("int");
        /*
        code += "  movl  " + lhsvar + ", %eax\n";
        if (op == "*") {
            code += "  imull " + rhsvar + ", %eax\n";
        }
        if (op == "/" || op == "%") {
            code += "  cltd\n"; // convert values to long double
            code += "  idivl " + rhsvar + "\n"; // do the division
        }
        if (op == "+") {
            code += "  addl " + rhsvar + ", %eax \n";
        }
        if (op == "-") {
            code += "  subl  " + rhsvar + ", %eax\n";
        }
        if (op == "<" || op == "<=" || op == ">" || op == ">=" || op == "==" || op == "!=") {
            code += "  cmpl  " + rhsvar + ", %eax\n";
            if (op == "<") {
                code += "  setl";
            }
            if (op == "<=") {
                code += "  setle";
            }
            if (op == ">") {
                code += "  setg";
            }
            if (op == ">=") {
                code += "  setge";
            }
            if (op == "==") {
                code += "  sete";
            }
            if (op == "!=") {
                code += "  setne";
            }
            code += "  %al\n";
            code += "  movzbl  %al, %eax\n";
        }
        if (op == "&") {
            code += "  andl  " + rhsvar + ", %eax\n";
        }
        if (op == "|") {
            code += "  orl   " + rhsvar + ", %eax\n";
        }
        if (op == "^") {
            code += "  xorl  " + rhsvar + ", %eax\n";
        }

        if (op == "%") {
            code += "  movl  %edx, ";
        } else {
            code += "  movl  %eax, ";
        }
        code += variable + "\n";
        */
    }
    // to do
    
    return variable;
}

string CFunctionCall::to_IR(CFG* cfg) const{
    BasicBlock* bb = cfg->current_bb;
    
    string variable = cfg->tos_add_temp("int");
    
    vector<string> results;
    results.push_back(functionName);
    results.push_back(variable);
    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        results.push_back((*it)->to_IR(cfg));
    }
    
    bb->add_IRInstr(op_call, "int", results);
    
    return variable;
}

