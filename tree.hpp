#pragma once
#include "./tokenize.hpp"

enum class NodeType{
  none,
  unknown,
  top,
  contract,
  library,
  interface,
  constructor,
  function,
  modifier,
  globalvariable,
  localvariable,
  loop,
  conditionalstatement,
};

struct SolTree{
  //nodeType type;
  string name;
  NodeType type;
  vector<SolTree> children;
  vector<SolTree> inherited;

  //SolTree( nodeType t=nodeType::none, string _name = string(), TokenType _token = TokenType::none) : type(t), name(_name.c_str()), token(_token){}
  SolTree(string _name = string(), NodeType _type= NodeType::none) : name(_name.c_str()), type(_type){}
};

SolTree SolParser(const string &source);
