#include "IR.h"

#include <iostream>
using std::ostream;
using std::cerr;
using std::endl;
using std::to_string;

#include "../AST/CExpression.h"
#include "../AST/CInstruction.h"
#include "../AST/CInstrExpression.h"
#include "../AST/CInstrVariable.h"
#include "../AST/CInstrReturn.h"
#include "../AST/CInstrArray.h"
#include "../AST/CInstrIf.h"

void CInstructions::to_IR(CFG* cfg) const {
    for (auto it = instructions.begin(); it != instructions.end(); ++it) {
        (*it)->to_IR(cfg);
    }
}

void CInstrExpression::to_IR(CFG* cfg) const {
    expr->to_IR(cfg);
}

void CInstrArray::to_IR(CFG* cfg) const {
    if(exprs.size() <= size){
        BasicBlock* bb = cfg->current_bb;

        int index;

        cfg->tos_add_array(name,type,size);
        int address = cfg->tos_get_index(name);
        
        if(exprs.size() != size && exprs.size()!=0){
            for (index = 0; index < size; index++){
                bb->add_IRInstr(op_ldconst_mem, type, {to_string(address - 4*index), "0"});
            }
        }

        index = 0;
        for (auto expr : exprs){
            if(dynamic_cast<CExpressionInt*>(expr) != NULL){
                bb->add_IRInstr(op_ldconst_mem, type, {to_string(address - 4*index), to_string(dynamic_cast<CExpressionInt*>(expr)->value)});
            }
            else{
                string temp = expr->to_IR(cfg);
                bb->add_IRInstr(op_copy_mem, type, {to_string(address - 4*index), temp});
            }
            index++;
        }
    }
    else{
        cerr << "ERROR: too many initializers for '" + type + "[" + to_string(size) + "]'" << endl;
        throw;
    }
}

void CInstrVariable::to_IR(CFG* cfg) const {
    if (expr != nullptr)
        expr->to_IR(cfg);
}

void CInstrReturn::to_IR(CFG* cfg) const {
    if (expr != nullptr)
        expr->to_IR(cfg);

    // to do
}

void CInstrIf::to_IR(CFG* cfg) const {
    //TODO : make two basic blocks : one true one false. Add those to cfg's vector of basic block
    // if there is no else statement, the basicblock exit_false will be nullptr (very important to check it later)
    // run to_ir on condition
    // run to_ir on the two blocks (check if the second is nullptr here)
}

