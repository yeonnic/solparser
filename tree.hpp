#pragma once
#include "./tokenize.hpp"

enum class NodeType{
  none,
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
  vector<SolTree> children; // children node tree
  vector<SolTree> inherited; // contract parents
  vector<SolTree> usemodifier; // modifiers used in function
  vector<SolTree> argument; // function and modifier
  vector<SolTree> conditional; // Conditional Statement content

  //SolTree( nodeType t=nodeType::none, string _name = string(), TokenType _token = TokenType::none) : type(t), name(_name.c_str()), token(_token){}
  SolTree(string _name = string(), NodeType _type= NodeType::none) : name(_name.c_str()), type(_type){}
};

SolTree SolParser(const string &source);
