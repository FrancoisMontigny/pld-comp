#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;

#include "CInstruction.h"

typedef string CType;

class CFunction {
public:
  CFunction();
  string to_asm() const;
  
  string name;
  
  vector<string> tos;
  map<string, CType> tosType;
  map<string, int> tosAddress;
  int tosOffset;
  
  CInstructions bloc;
  
  int temp_id;
  string tos_add_temp(CType type);
  string tos_addr(string variable) const;
  
  void fill_tos();
private:
  void fill_tos(CInstructions& bloc);
};

