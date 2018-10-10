#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

enum class TokenType{
  none,
  eof,
  error,
  string,
  integer,
  real,
  identifier,
  contract,
  library,
  interface,
  constructor,
  function,
  assign, // =
  equal, // ==
  question, // ?
  plus, // +
  minus, // -
  asterisk, // *
  divide, // /
  percent, // %
  hat, // ^
  dot, // .
  colon, // :
  semicolon, // ;
  comma, // ,
  lbracket, // {
  rbracket, // }
  lsbracket, // [
  rsbracket, // ]
  lparen, // (
  rparen, // )
  lessequal, // <=
  less, // <
  greaterequal, // >=
  greater, // >
  notequal, // !=
  or_, // |
  doubleor_, // ||
  and_, // &
  doubleand_, // &&
  not_, // !
  true_, // true
  false_, // false
  this_, // this
  if_, // i)
  else_, // else
  for_, // for
  while_, // while
  do_, // do
  return_, // return
  break_, // break
  continue_, // continue
  throw_, // throw
  public_, // public
  external_, // external
  private_, // private
  internal_, // internal
  is, // is
  comment1, // //
  comment2, // /*
};

struct Token{
  string str;
  TokenType type;
  int line;
  Token(string s = string(), TokenType t = TokenType::none, int _line = 0) : str(s.c_str()), type(t), line(_line) {}
};

vector<Token> Tokenize(const string &str);
typedef vector<Token>::const_iterator Tokit;
