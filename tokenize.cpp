#include "tokenize.hpp"

bool ishex(char c)
{
	if('0' <= c && c <= '9') return true;
	if('A' <= c && c <= 'F') return true;
	if('a' <= c && c <= 'f') return true;
	return false;
}

int hexToInt(char c)
{
	if('0' <= c && c <= '9') return c - '0';
	if('A' <= c && c <= 'F') return c - 'A' + 10;
	if('a' <= c && c <= 'f') return c - 'a' + 10;
	return -1;
}

char DecodeEscape(string::const_iterator& it, string::const_iterator end, bool& error)
{
	if(it == end || *it != '\\')
	{
		error = true;
		return 0;
	}
	++it;
	error = false;
	switch(*it)
	{
	case '\\':
		++it;
		return '\\';
	case 't':
		++it;
		return '\t';
	case 'n':
		++it;
		return '\n';
	case 'r':
		++it;
		return '\r';
	case '\'':
		++it;
		return '\'';
	case '"':
		++it;
		return '\"';
	case '0':
		++it;
		return '\0';
	case 'x':
		{
			++it;
			int h = 0;
			for(int i = 0; i < 2; ++i)
			{
				if(!ishex(*it))
				{
					error = true;
					return 0;
				}
				h = h * 16 +  hexToInt(*it);
				++it;
			}
			return h;
		}
	}
	error = true;
	return '\\';
}

bool IsCutCharacter(wchar_t c)
{
	switch(c)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
	case '<':
	case '>':
	case '[':
	case ']':
	case '=':
	case '!':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '^':
	case '?':
	case '.':
	case ':':
	case ';':
	case ',':
	case '{':
	case '}':
	case '(':
	case ')':
	case '|':
		return true;
	}
	return false;
}



vector<Token> Tokenize(const string &str){

  vector<Token> ret;
  
  auto it = str.begin();
  auto marker = str.begin();

  TokenType stage = TokenType::none;

  int line = 1;
  string s;

  char tmp = 0;

  for(;it != str.end(); it++){
    switch(stage){
      case TokenType::none:
        switch(*it){
          case '\n':
            line++;
            break;
          case ' ':
          case '\t':
          case '\r':
            break;
          case '\"':
          case '\'':
            stage = TokenType::string;
            marker = it;
            tmp = *it;
            break;
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            stage = TokenType::integer;
            marker = it;
            break;
          case '<':
            if(it+1 != str.end() && it[1] == '='){
              ret.push_back(Token("<=", TokenType::lessequal, line));
              it++;
            } else {
              ret.push_back(Token("<", TokenType::less, line));
            }
            break;
          case '>':
            if(it+1 != str.end() && it[1] == '='){
              ret.push_back(Token(">=", TokenType::greaterequal, line));
              it++;
            } else {
              ret.push_back(Token(">", TokenType::greater, line));
            }
            break;
          case '[':
            ret.push_back(Token("[", TokenType::lsbracket, line));
            break;
          case ']':
            ret.push_back(Token("]", TokenType::rsbracket, line));
            break;
          case '(':
            ret.push_back(Token("(", TokenType::lparen, line));
            break;
          case ')':
            ret.push_back(Token(")", TokenType::rparen, line));
            break;
          case '{':
            ret.push_back(Token("{", TokenType::lbracket, line));
            break;
          case '}':
            ret.push_back(Token("}", TokenType::rbracket, line));
            break;
          case '=':
            if( it+1 != str.end() && it[1] == '='){
              ret.push_back(Token("==", TokenType::equal, line));
              it++;
            } else {
              ret.push_back(Token("=", TokenType::assign, line));
            }
            break;
          case '!':
            if( it+1 != str.end() && it[1] == '='){
              ret.push_back(Token("!=", TokenType::notequal, line));
              it++;
            } else {
              ret.push_back(Token("!", TokenType::not_, line));
            }
            break;
          case '+':
            ret.push_back(Token("+", TokenType::plus, line));
            break;
          case '-':
            ret.push_back(Token("-", TokenType::minus, line));
            break;
          case '*':
            ret.push_back(Token("*", TokenType::asterisk, line));
            break;
          case '/':
            if( it+1 != str.end() && it[1] == '/'){
              stage = TokenType::comment1;
              it++;
            } else if( it+1 != str.end() && it[1] == '*'){
              stage = TokenType::comment2;
              it++;
            } else {
              ret.push_back(Token("/", TokenType::divide, line));
            } 
            break;
          case '%':
            ret.push_back(Token("%", TokenType::percent, line));
            break;
          case '^':
            ret.push_back(Token("^", TokenType::hat, line));
            break;
          case '?':
            ret.push_back(Token("?", TokenType::question, line));
            break;
          case '.':
            ret.push_back(Token(".", TokenType::dot, line));
            break;
          case ':':
            ret.push_back(Token(":", TokenType::colon, line));
            break;
          case ';':
            ret.push_back(Token(";", TokenType::semicolon, line));
            break;
          case ',':
            ret.push_back(Token(",", TokenType::comma, line));
            break;
          case '|':
            if( it+1 != str.end() && it[1] == '|'){
              ret.push_back(Token("||", TokenType::doubleor_, line));
              it++;
            } else {
              ret.push_back(Token("|", TokenType::or_, line));
            }
            break;
          case '&':
            if( it+1 != str.end() && it[1] == '&'){
              ret.push_back(Token("&&", TokenType::doubleand_, line));
              it++;
            } else {
              ret.push_back(Token("&", TokenType::and_, line));
            }
            break;
          default:
            stage = TokenType::identifier;
            marker = it;
            break;
        }
        break;
      case TokenType::string:
        for(;*it != tmp; it++){
          if(it == str.end() || *it == '\n' || *it == '\r'){
            stage = TokenType::none;
            ret.push_back(Token(s, TokenType::error, line));
            break;
          } else if(*it == '\\'){
            bool err;
            s.push_back(DecodeEscape(it, str.end(), err));
            if(err){
              stage = TokenType::none;
              ret.push_back(Token(s, TokenType::error, line));

              break;
            }
            it--;
          } else {
            s.push_back(*it);
          }
        }

        stage = TokenType::none;
        ret.push_back(Token(s, TokenType::string, line));
        break;
      case TokenType::integer:
        if(*it == '.'){
          stage = TokenType::real;
        } else if(IsCutCharacter(*it)){
          stage = TokenType::none;
          ret.push_back(Token(string(marker, it--), TokenType::integer, line));
        } else if(!isdigit(*it)){
          stage = TokenType::none;
          ret.push_back(Token(string(marker, it--), TokenType::error, line));
        }
        break;
      case TokenType::real:
        if(IsCutCharacter(*it)){
          stage = TokenType::none;
          ret.push_back(Token(string(marker, it--), TokenType::real, line));
        } else if(!isdigit(*it)){
          stage = TokenType::none;
          ret.push_back(Token(string(marker, it--), TokenType::real, line));
        }
        break;
      case TokenType::identifier:
        if(IsCutCharacter(*it)){
          stage = TokenType::none;
          string s(marker, it--);
          if(s == "true"){
            ret.push_back(Token(s, TokenType::true_, line));
          } else if(s == "false"){
            ret.push_back(Token(s, TokenType::false_, line));
          } else if(s == "this"){
            ret.push_back(Token(s, TokenType::this_, line));
          } else if(s == "contract"){
            ret.push_back(Token(s, TokenType::contract, line));
          } else if(s == "library"){
            ret.push_back(Token(s, TokenType::library, line));
          } else if(s == "interface"){
            ret.push_back(Token(s, TokenType::interface, line));
          } else if(s == "constructor"){
            ret.push_back(Token(s, TokenType::constructor, line));
          } else if(s == "if"){
            ret.push_back(Token(s, TokenType::if_, line));
          } else if(s == "else"){
            ret.push_back(Token(s, TokenType::else_, line));
          } else if(s == "for"){
            ret.push_back(Token(s, TokenType::for_, line));
          } else if(s == "while"){
            ret.push_back(Token(s, TokenType::while_, line));
          } else if(s == "do"){
            ret.push_back(Token(s, TokenType::do_, line));
          } else if(s == "return"){
            ret.push_back(Token(s, TokenType::return_, line));
          } else if(s == "break"){
            ret.push_back(Token(s, TokenType::break_, line));
          } else if(s == "continue"){
            ret.push_back(Token(s, TokenType::continue_, line));
          } else if(s == "throw"){
            ret.push_back(Token(s, TokenType::throw_, line));
          } else if(s == "public"){
            ret.push_back(Token(s, TokenType::public_, line));
          } else if(s == "external"){
            ret.push_back(Token(s, TokenType::external_, line));
          } else if(s == "private"){
            ret.push_back(Token(s, TokenType::private_, line));
          } else if(s == "internal"){
            ret.push_back(Token(s, TokenType::internal_, line));
          } else if(s == "is"){
            ret.push_back(Token(s, TokenType::is, line));
          } else {
            ret.push_back(Token(s, TokenType::identifier, line));
          }
        }
        break;
      case TokenType::comment1:
        if(*it == '\n'){
          line++;
          stage = TokenType::none;
        }
        break;
      case TokenType::comment2:
        switch(*it){
          case '\n':
            line++;
            break;
          case '*':
            if( it+1 != str.end() && it[1] == '/'){
              it++;
              stage = TokenType::none;
            }
            break;
        }
        break;
    }
  }
  ret.push_back(Token("", TokenType::eof, line));

  return ret;

}
