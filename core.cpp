#include <fstream>
#include <string>
#include <iostream>
#include "json/json.h"
#include <stdlib.h>
#include <sstream>

using namespace std;

bool valid_move(string player, string original_position, string new_position);
void query_next_move(string player);

int main() {
  // turn movement
  bool game_over = false;
  while (!game_over) {
    cout << "Black's Turn" << endl;
    query_next_move("b");
    cout << "White's Turn" << endl;
    query_next_move("w");
  }  

  return 0;
}

void query_next_move(string player) {
  string original_position;
  string new_position;
  
  cout << "Original Position?: " << endl;
  cin >> original_position;
  cout << "New Position?: " << endl;
  cin >> new_position;

  if (valid_move(player, original_position, new_position)) {
    cout << "Valid Move" << endl;
  } else {
    cout << "Invalid Move" << endl;
  }
}

bool valid_move(string player, string original_position, string new_position) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  bool parseSuccess = reader.parse(json_raw, root, false);

  string current_turn = "b";
  string corner_pieces = "a1 a10 k1 k10";
  if (root[new_position].empty()) {
    // checks if not diagonal move
    if (original_position.at(0) == new_position.at(0) || original_position.at(1) == new_position.at(1)) {
      // checks if corner 
      std::size_t corner_found = corner_pieces.find(new_position);
      if (corner_found != std::string::npos) {
        return false;
      } else {
        // checks if piece is in way
        // vertical check
        if (original_position.at(0) == new_position.at(0)) {
          int start = (char)original_position.at(1) - 48;
          int end = (char)new_position.at(1) - 48;
          std::string pieceInWay = "false";
          for (int i=start+1; i < end+1; i++) {
            Json::Value node = root[original_position.at(0) + std::to_string(i)];
            if (node.isNull() == 0) {
              pieceInWay = "true";
            }
          }
          if (pieceInWay == "true") {
            return false;
          } else {
            return true;
          }
        // horizontal check
        } else {
          std::string pieceInWay = "false";
          std::string alphabet = "abcdefghijk";
          char start = original_position.at(0);
          char end = new_position.at(0);
          std::string substring = alphabet.substr(alphabet.find(start)+1, alphabet.find(end));
          for (char c : substring) {
            Json::Value node = root[c + new_position.at(1)];
            if (node.isNull() == 0) {
              pieceInWay = "true";
            }
          }
          if (pieceInWay == "true") {
            return false;
          } else {
            return true;
          }
        }
      }
    } else {
      return false;
    }
  } else {
    return false;
  }

}
