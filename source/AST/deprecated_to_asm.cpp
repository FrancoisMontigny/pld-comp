#include "CProg.h"
#include "CFunction.h"
#include "CFunctionHeader.h"

#include <string>
using std::to_string;
#include <iostream>
using std::cerr;
using std::endl;


string CProg::to_asm() const {
    string code;
    code += ".text\n";
    code += ".global main\n";
    try {
        for (const CFunction& f : functions) {
            code += f.to_asm();
        }
    } catch(...) {
        cerr << "ERROR: couldn't generate assembly code" << endl;
    }
    return code;
}

string CParameter::to_asm(const CFunction* f, int index) const {
    static const string registerName[] = { "rdi", "rsi", "rdx", "rcx", "r8d", "r9d" };
    
    string variable = f->tos_addr(name);
    string code = "  movl %" + registerName[index] + ", " + variable + " # Move the parameter to one of the function registers\n";
    return code;
}

string CFunction::to_asm() const {
    string code;
    code += name;
    
    /*
        string params = "";
        params += "(";
        for(auto it = parameters.begin(); it != parameters.end(); ++it) {
            if (!params.empty()) params += ", ";
            params += it->type;
        }
        params += ")";
        code += params;
    } */
    
    code += ":\n";

    code += "  ## prologue\n";
    code += "  pushq %rbp # save %rbp on the stack\n";
    code += "  movq %rsp, %rbp # define %rbp for the current function\n";

    int index = 0;
    for (auto it = parameters.begin() ; it!= parameters.end() ; ++it) {
        code += it->to_asm(this, index);
        index++;
    }

    code += "  ## contenu\n";

    for (const CInstruction* i : block.instructions) {
        code += i->to_asm(this);
    }

    code += "  ## epilogue\n";
    code += "  popq %rbp # restore %rbp from the stack\n";
    code += "  ret\n";

    return code;
}

