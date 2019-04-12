#pragma once

#include <iostream>
using std::ostream;
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "../TOS.h"
class CFunction;

class IProg;

class CProg {
public:
	void optimize();
	IProg* to_IR() const;
	void gen_asm_z80(ostream& o) const;

	TOS tos;
	vector<CFunction> functions;
};

