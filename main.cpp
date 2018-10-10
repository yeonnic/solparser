#include "./tokenize.h"

int main(int argc, char **argv){

  if(argc != 2){
    cout << "Usage : solparser file_name" << endl;
    return -1;
  }

  ifstream f(argv[1]);
  string s;

  if(f.is_open()){
    f.seekg(0, ios::end);

    size_t size = f.tellg();

    s.resize(size);

    f.seekg(0, ios::beg);

    f.read(&s[0], size);

    cout << s << endl;

    auto tokens = Tokenize(s);

    for(auto test: tokens){
      cout << test.str << endl;
    }

  } else {
    cout << "file open error!" << endl;
  }

  return 0;

}
