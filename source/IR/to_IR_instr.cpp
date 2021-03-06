#include "IR.h"

#include <iostream>
using std::ostream;
using std::cerr;
using std::endl;
using std::to_string;

#include "../AST/CExpression.h"
#include "../AST/CFunction.h"
#include "../AST/CInstruction.h"
#include "../AST/CInstrExpression.h"
#include "../AST/CInstrVariable.h"
#include "../AST/CInstrReturn.h"
#include "../AST/CInstrArray.h"
#include "../AST/CInstrIf.h"
#include "../AST/CInstrWhile.h"
#include "../AST/CInstrDoWhile.h"
#include "../AST/CInstrFor.h"

void CInstructions::to_IR(CFG* cfg) const {
    for (auto it = instructions.begin(); it != instructions.end(); ++it) {
        (*it)->to_IR(cfg);
    }
}

void CInstrExpression::to_IR(CFG* cfg) const {
    expr->to_IR_full(cfg);
}

void CInstrArray::to_IR(CFG* cfg) const {
    BasicBlock* bb = cfg->current_bb;
    // cfg->tos.add(name, type, size);
    
    int index = 0;
    for (CExpressionPart* expr : exprs) {
        CExpressionInt* exprint = dynamic_cast<CExpressionInt*>(expr); 
        if (exprint != nullptr) {
            bb->add_IRInstr(op_ldconst_to_array_index, type,
                            { name, to_string(index), '$'+to_string(exprint->value) });
        } else {
            string result = expr->to_IR(cfg);
            bb->add_IRInstr(op_copy_to_array_index, type,
                            { name, to_string(index), result });
            cfg->tos.free_temp(result);
        }
        index++;
    }
    for (; index < count; index++) {
        bb->add_IRInstr(op_ldconst_to_array_index, type,
                        { name, to_string(index), "$0" });
    }
}

void CInstrVariable::to_IR(CFG* cfg) const {
    // cfg->tos_add(name, type);
    
    if (expr != nullptr) {
        expr->to_IR_full(cfg);
    }
}

void CInstrReturn::to_IR(CFG* cfg) const {
    BasicBlock* bb = cfg->current_bb;
    
    CType type = cfg->ast->type;
    
    if (expr == nullptr) {
        if (type != "void") {
            cerr << "ERROR: return without value in non-void function" << endl;
            throw;
        }
    } else {
        if (type == "void") {
            cerr << "ERROR: return with value in void function" << endl;
            throw;
        }
        
        string result = expr->to_IR(cfg);
        bb->add_IRInstr(op_return, type, {result});
        cfg->tos.free_temp(result);
    }
    
    bb->exit_true = nullptr;
    bb->exit_false = nullptr;
    cfg->add_bb(new BasicBlock(cfg, cfg->new_BB_name()));
}

void CInstrIf::to_IR(CFG* cfg) const {
    BasicBlock* bb = cfg->current_bb;

    // Add condition to the current block
    condition->to_IR_bool(cfg);

    bool hasTrue = true; // !blockTrue.instructions.empty();
    bool hasFalse = !blockFalse.instructions.empty();
    if ((!hasTrue) && (!hasFalse))
        return;

    // Create new blocks for if statement
    string prefix = cfg->new_BB_name("if");
    BasicBlock* bbTrue =
			hasTrue ? new BasicBlock(cfg, prefix + "true") : nullptr;
    BasicBlock* bbFalse =
            hasFalse ? new BasicBlock(cfg, prefix + "false") : nullptr;
    BasicBlock* bbNext = new BasicBlock(cfg, cfg->new_BB_name());

    // Link current block to the content of the if
    bb->exit_true = hasTrue ? bbTrue : bbNext;
    bb->exit_false = hasFalse ? bbFalse : bbNext;

    // Prepare the exit_true and link it to the next block
    if (hasTrue) {
        cfg->add_bb(bbTrue);
        bbTrue->exit_true = bbNext;
        blockTrue.to_IR(cfg);
    }

    // If there is an exit_false, prepare it as well
    if (hasFalse) {
        cfg->add_bb(bbFalse);
        bbFalse->exit_true = bbNext;
        blockFalse.to_IR(cfg);
    }

    // Add next block to CFG
    cfg->add_bb(bbNext);
}

void CInstrWhile::to_IR(CFG* cfg) const {
    BasicBlock* bb = cfg->current_bb;

    // Create new blocks for while statement
    string prefix = cfg->new_BB_name("while");
    BasicBlock* bbCondition = new BasicBlock(cfg, prefix + "condition");
    BasicBlock* bbContent = new BasicBlock(cfg, prefix);

    BasicBlock* bbNext = new BasicBlock(cfg, cfg->new_BB_name());

    // Link current block to the contents of the while
    bb->exit_true = bbCondition;

    bbCondition->exit_true = bbContent;
    bbCondition->exit_false = bbNext;
    cfg->add_bb(bbCondition);
    // Add condition to the current block
    condition->to_IR_bool(cfg);

    // Prepare the exit_true and exit_false and link them to the next block
    cfg->add_bb(bbContent);
    bbContent->exit_true = bbCondition;
    blockContent.to_IR(cfg);

    // Add next block to CFG
    cfg->add_bb(bbNext);
}

void CInstrDoWhile::to_IR(CFG* cfg) const {
    BasicBlock* bb = cfg->current_bb;

    // Create new blocks for while statement
    string prefix = cfg->new_BB_name("while");
    BasicBlock* bbCondition = new BasicBlock(cfg, prefix + "condition");
    BasicBlock* bbContent = new BasicBlock(cfg, prefix);

    BasicBlock* bbNext = new BasicBlock(cfg, cfg->new_BB_name());

    // Link current block to the contents of the while
    bb->exit_true = bbContent;

    // Prepare the exit_true and exit_false and link them to the next block
    cfg->add_bb(bbContent);
    bbContent->exit_true = bbCondition;
    blockContent.to_IR(cfg);

    cfg->add_bb(bbCondition);
    bbCondition->exit_true = bbContent;
    bbCondition->exit_false = bbNext;
    // Add condition to the current block
    condition->to_IR_bool(cfg);

    // Add next block to CFG
    cfg->add_bb(bbNext);
}

void CInstrFor::to_IR(CFG* cfg) const {
    BasicBlock* bb = cfg->current_bb;

    // Create new blocks for while statement
    string prefix = cfg->new_BB_name("for");
    BasicBlock* bbStopCondition = 
        (stopCondition == nullptr ?
            nullptr :
            new BasicBlock(cfg, prefix + "condition")
        );
    BasicBlock* bbContent = new BasicBlock(cfg, prefix);
    BasicBlock* bbNext = new BasicBlock(cfg, cfg->new_BB_name());

    bb->exit_true = bbStopCondition != nullptr ? bbStopCondition : bbContent;
    if (start != nullptr) start->to_IR_full(cfg);

    if (stopCondition != nullptr) {
        // Link current block to the contents of the while
        cfg->add_bb(bbStopCondition);
        bbStopCondition->exit_true = bbContent;
        bbStopCondition->exit_false = bbNext;
        // Add condition to the current block
        stopCondition->to_IR_bool(cfg);
    }

    // Prepare the exit_true and exit_false and link them to the next block
    cfg->add_bb(bbContent);
    bbContent->exit_true = bbStopCondition != nullptr ? bbStopCondition : bbContent;
    blockContent.to_IR(cfg);
    if (evolution != nullptr) evolution->to_IR_full(cfg);

    // Add next block to CFG
    cfg->add_bb(bbNext);
}

