#pragma once

#include <map>
using std::map;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::ostream;
#include <initializer_list>

#include "../CType.h"
#include "../TOS.h"
class CFunction;

class IRInstr;
class BasicBlock;
class CFG;

/* the instructions themselves */
typedef enum {
    op_ldconst,
    op_ldconst_to_array_index,
    op_ldconst_array,
    op_index,
    op_index_ldconst,
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_mod,
    op_copy,
    op_copy_to_array_index,
    op_copy_array,
    op_copy_from_array,
    op_rmem,
    op_wmem,
    op_call, 
    op_cmp_eq,
    op_cmp_ne,
    op_cmp_lt,
    op_cmp_le,
    op_cmp_gt,
    op_cmp_ge,
    op_not,
    op_binary_and,
    op_binary_or,
    op_binary_xor,
    op_return
} Operation;

// class for one 3-address instruction
class IRInstr {
public:
    /* constructor */
    IRInstr(BasicBlock* bb, Operation op, CType type, vector<string> params);
    ~IRInstr() = default;
    
    /* code generation */
    void gen_asm_x86(ostream &o) const; /* x86 assembly code generation for this IR instruction */
    void gen_asm_z80(ostream &o) const; /* z80 assembly code generation for this IR instruction */
    
private:
    BasicBlock* bb; /* the BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
    Operation op;
    CType type;
    
    vector<string> params;
    /*
        for 3-op instrs: d, x, y;
        for ldconst: d, c;
        for call: label, d, params;
        for wmem and rmem: variable, address;
    */
};

/*  Important comments:
    IRInstr has no jump instructions:
    assembly jumps are generated as follows in BasicBlock::gen_asm():
    1/  a cmp_* comparison instructions, if it is the last instruction of its block, 
        generates an actual assembly comparison followed by a conditional jump to the exit_false branch
        If it is not the last instruction of its block, it behaves as an arithmetic two-operand instruction (add or mult)
    2/  BasicBlock::gen_asm() first calls IRInstr::gen_asm() on all its instructions, and then 
        if  exit_true  is a  nullptr, it generates the epilogue
        if  exit_false is not a nullptr, and the last instruction is not a cmp, it generates two conditional branches based on the value of the last variable assigned
        otherwise it generates an unconditional jmp to the exit_true branch 
*/

// class for a basic block
class BasicBlock {
public:
    BasicBlock(CFG* cfg, string entry_label);
    ~BasicBlock();
    void gen_asm_x86(ostream &o) const; /* x86 assembly code generation for this basic block */
    void gen_asm_z80(ostream &o) const; /* z80 assembly code generation for this basic block */
    
    void add_IRInstr(Operation op, const CType& type, vector<string> params);
    
    BasicBlock* exit_true;  /* pointer to the next basic block, true branch. If nullptr, return from procedure */ 
    BasicBlock* exit_false; /* pointer to the next basic block, false branch. If null_ptr, the basic block ends with an unconditional jump */
    string label; /* label of the BB, also will be the label in the generated code */
    CFG* cfg; /* the CFG where this block belongs */
    vector<IRInstr*> instrs; /* the instructions themselves */
protected:
};

/*  A few important comments:
    - The entry block is the one with the same label as the AST function name.
      (it could be the first of bbs, or it could be defined by an attribute value)
    - The exit block is the one with both exit pointers equal to nullptr.
      (again it could be identified in a more explicit way)
*/

// class for the control flow graph, also includes the symbol table
class CFG {
public:
    CFG(const CFunction* ast, string name);
    ~CFG();
    const CFunction* ast; /* The AST this CFG comes from */
    
    void optimize();
    
    // x86 code generation
    void gen_asm_x86(ostream& o) const;
    void gen_asm_x86_prologue(ostream& o) const;
    void gen_asm_x86_epilogue(ostream& o) const;
    // z80 code generation
    void gen_asm_z80(ostream& o) const;
    void gen_asm_z80_prologue(ostream& o) const;
    void gen_asm_z80_epilogue(ostream& o) const;
    
    // basic block management
    void add_bb(BasicBlock* bb); 
    string new_BB_name(const string& prefix = "");
    BasicBlock* current_bb;
    
    
public:
    string name;
    
    TOS tos;
    
    vector<BasicBlock*> bbs; /* all the basic blocks of this CFG */
    int bbNumberLast; /* just for naming */
};

