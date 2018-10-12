#include "./tree.hpp"

int S_variable(Tokit begin, SolTree &out){
  
}

int S_ConditionalStatement(Tokit begin, SolTree &out){
  vector<SolTree> tmp;
  int count = 0;
  auto it = begin;

  cout << "S_ConditionalStatement() called!!" << endl;
  cout << "it->str  : " << it->str << endl;

}

int S_while(Tokit begin, SolTree &out){
  vector<SolTree> tmp;
  int count = 0;
  auto it = begin;

  cout << "S_while() called!!" << endl;
  cout << "it->str  : " << it->str << endl;

  assert(it->type == TokenType::lparen && (it+1)->type != TokenType::eof);
  it++; count++;


  
  return count;
}

int S_for(Tokit begin, SolTree &out){

}

int S_do_while(Tokit begin, SolTree &out){
  
}

int S_modifier(Tokit begin, SolTree &out){

}

int S_loop_and_if(Tokit begin, SolTree &out){
}

int S_Function(Tokit begin, SolTree &out){
  vector<SolTree> tmp;
  int count = 0;
  auto it = begin;

  cout << "S_Function() called!!" << endl;
  cout << "it->str : " << it->str << endl;

  if((it+1)->type == TokenType::lparen){
    it++; count++;
    for(; it->type != TokenType::rparen && it->type != TokenType::eof; it++){
      count++;
    }
    for(; it->type != TokenType::lbracket && it->type != TokenType::eof; it++){
      count++;
      if(it->type == TokenType::identifier){
        out.usemodifier.push_back(SolTree(it->str, NodeType::modifier));
      }
    }

    if(it->type == TokenType::lbracket){
      count++;
      it++;
    }

    for(; it->type != TokenType::rbracket && it->type != TokenType::eof; it++){
      int c = 0;
      switch(it->type){
        case TokenType::for_:
        case TokenType::while_:
        case TokenType::do_:
          if((it+1)->type == TokenType::lparen && (it+1)->type != TokenType::eof){
            auto p = SolTree(it->str, NodeType::loop);
            it++; count++;
            if((it-1)->type == TokenType::while_){
              c = S_while(it, p);
            } else if((it-1)->type == TokenType::for_){
              c = S_for(it, p);
            } else {
              c = S_do_while(it, p);
            }
            it += c;
            count += c;
            tmp.push_back(p);
          } else {
            cout << "loop error!! line : " << it->line << endl;
            exit(-1);
          }
          break;
        case TokenType::if_:
        case TokenType::else_:
          if((it+1)->type == TokenType::lparen && (it+1)->type != TokenType::eof){
            auto p = SolTree(it->str, NodeType::conditionalstatement);
            it++; count++;
            c = S_ConditionalStatement(it, p);
            it += c;
            count += c;
          } else {
            cout << "conditional statement error!! line : " << it->line << endl;
            exit(-1);
          }
          break;
      }
    }
  }
  return count;
}

int S_Parent(Tokit begin, SolTree &out){
  vector<SolTree> tmp;
  int count = 0;
  auto it = begin;

  cout << "S_Parent() called!!" << endl;
  cout << " it->str : " << it->str << endl;

  if(it->type == TokenType::lbracket){
    it++;
    count++;
    for(; it->type != TokenType::rbracket && it->type != TokenType::eof; it++){
      int c = 0;
      count++;
      switch(it->type){
        case TokenType::constructor:
        case TokenType::function:
          if((it+1)->type != TokenType::eof){
            auto p = SolTree((it+1)->str, NodeType::function);
            it++;
            count++;
            c = S_Function(it, p);
            count += c;
            tmp.push_back(p);
          } else {
            cout << "function error!! line : " << it->line << endl;
            exit(-1);
          }
          break;
        default:
          tmp.push_back(SolTree(it->str, NodeType::none));
          break;
      }
    }
  } else {
    cout << "error! line : " << it->line;
    exit(-1);
  }

  out.children = tmp;

  return count;
}

int S_Top(Tokit begin, SolTree &out){
  unsigned int ret = 0;

  vector<SolTree> tmp;

  auto it = begin;

  cout << "S_Top() called!!" << endl;

  for(; it->type != TokenType::eof;){
    if(it->type == TokenType::contract || it->type == TokenType::library || it->type == TokenType::interface){
      SolTree p;
      if((it+1)->type != TokenType::eof){
        switch(it->type){
          case TokenType::contract:
            p = SolTree((it+1)->str, NodeType::contract);
            it+=2;
            break;
          case TokenType::library:
            p = SolTree((it+1)->str, NodeType::library);
            it+=2;
            break;
          case TokenType::interface:
            p = SolTree((it+1)->str, NodeType::interface);
            it+=2;
            break;
        }

        if(it->type == TokenType::is){
          it++;
          for(; it->type != TokenType::lbracket && it->type != TokenType::eof; it++)
            p.inherited.push_back(SolTree(it->str, NodeType::none));
          it += S_Parent(it, p);
        } else if(it->type == TokenType::lbracket){
          it += S_Parent(it, p);
        } else {
          cout << "Error!!! line : " << it->line << endl;
          return -1;
        }
        tmp.push_back(p);
        ret++;
      }
    } else {
      it++;
    }
  }

  out.children = tmp;

  return ret;
}

SolTree SolParser(const string &source){

  auto tokens = Tokenize(source);

  //SolTree ret = SolTree(nodeType::top, "TopNode", TokenType::none);
  SolTree ret = SolTree("TopNode", NodeType::top);
  S_Top(tokens.begin(), ret);

  return ret;
}
