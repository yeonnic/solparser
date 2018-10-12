#include "./tree.hpp"

int S_ConditionalStatement(Tokit &begin, SolTree &out){

}

int S_Loop(Tokit &begin, SolTree &out){
}

int S_Function(Tokit &begin, SolTree &out){
  
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
      SolTree p(it->str, NodeType::unknown);

      if(it->type == TokenType::lbracket){
        int c = S_Parent(it, p);
        count += c;
        it += c;
      }
      count++;
      tmp.push_back(p);
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
            p.inherited.push_back(SolTree(it->str, NodeType::unknown));
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
