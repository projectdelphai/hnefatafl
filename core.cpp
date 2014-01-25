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

  // turn movement
  cout << "Black's Turn" << endl;
  cout << "Original Position?: " << endl;
  string original_position;
  cin >> original_position;
  cout << "New Position?: " << endl;
  string new_position;
  cin >> new_position;
  if (root[new_position].empty()) {
    if (original_position.at(0) == new_position.at(0) || original_position.at(1) == new_position.at(1)) {
      cout << "Valid Move!" << endl;
    } else {
      cout << "Invalid Move" << endl;
    }
  } else {
    cout << "Invalid Move" << endl;
  }

  return 0;
}
