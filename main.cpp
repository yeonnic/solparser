#include "./tree.hpp"
#include "./argx.hpp"

string ReadFile(const string &fileName){
  ifstream f(fileName);
  string s;

  if(f.is_open()){
    f.seekg(0, ios::end);

    size_t size = f.tellg();

    s.resize(size);

    f.seekg(0, ios::beg);

    f.read(&s[0], size);
  } else {
    cout << "file open error!" << endl;
    exit(-1);
  }

  return s;
}

void printTab(int count){
  for(int i=0; i<count; i++)
    cout << "\t";
}

void TokensView(const vector<Token> &T){
  cout << "[ ";
  for ( auto i: T){
    cout << "'\x1b[91m" << i.str << "\x1b[0m', ";
  }
  cout << "]" << endl;
}

void SoltreeView(const SolTree &tree, int count = 0){
  printTab(count);
  cout << tree.name << " ";
  cout << "\n";
  if(tree.inherited.size()){
    cout << " is ";
    for(auto i : tree.inherited){
      cout << i.name << " ";
    }
  }
  for(auto i : tree.children){
    SoltreeView(i, count+1);
  }
}

int main(int argc, char **argv){

  args::ArgumentParser parser("This is a solidity parse program.", "This goes after the options.");
  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

  args::Group inputGroup(parser, "read source options:", args::Group::Validators::Xor);
  args::ValueFlag<string> file(inputGroup, "NAME", "read the source file", {'f', "file"});
  args::Flag sinput(inputGroup, "stdin", "read the stdin", {'d', "stdin"});

  try {
    parser.ParseCLI(argc ,argv);


    if(file || sinput){
      string source;

      if(file){
        source = ReadFile(args::get(file));

        cout << source << endl;

      } else if(sinput){
        cout << "stdin input!!!" << endl;
      }
      
      auto tree = SolParser(source);

      SoltreeView(tree);

    }
  } catch (args::Completion& e){
    cout << e.what() << endl;
    return 0;
  } catch (const args::Help&){
    cout << parser;
    return 0;
  } catch (const args::ParseError& e){
    cerr << e.what() << std::endl;
    cerr << parser;
    return -1;
  } catch (const args::ValidationError e){
    std::cerr << parser;
    return 1;
  }
  return 0;

}
