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
  cout << "Original Position?: " << endl;
  string original_position;
  cin >> original_position;
  cout << "New Position?: " << endl;
  string new_position;
  cin >> new_position;
  if (root[new_position].empty()) {
    cout << "Valid Move!" << endl;
  } else {
    cout << "Invalid Move, try again." << endl;
  }
  return 0;
}
