#include <fstream>
#include <string>
#include <iostream>
#include "json/json.h"

using namespace std;

int main() {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  bool parseSuccess = reader.parse(json_raw, root, false);
  cout << "Black's Turn" << endl;
  string move;
  cin >> move;
  cout << move << endl;
  cout << "Hello World!" << endl;
  return 0;
}
