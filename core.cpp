#include <fstream>
#include <string>
#include <iostream>
#include "json/json.h"
#include <stdlib.h>
#include <sstream>

using namespace std;

bool valid_move(string player, string original_position, string new_position);
bool query_next_move(string player);
void make_move(string original_position, string new_position);
vector<string> check_for_capture(string new_position);
void remove_captured(vector<string> captured);
vector<string> get_adjacent_pieces(Json::Value root, string new_position);
bool valid_piece(Json::Value root, string position);
void win(string player);

int main() {
  // turn movement
  bool game_over = false;
  bool end_game = false;
  while (!game_over) {
    cout << "Black's Turn" << endl;
    end_game = query_next_move("b");
    if (end_game) {
      win("b");
      game_over = true;
    }
    cout << "White's Turn" << endl;
    end_game = query_next_move("w");
    if (end_game) {
      win("w");
      game_over = true;
    }
  }  

  return 0;
}

void win(string player) {
  if (player == "w") {
    cout << "White wins!" << endl;
  } else {
    cout << "Black wins!" << endl;
  }
}

bool query_next_move(string player) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  bool parseSuccess = reader.parse(json_raw, root, false);

  string original_position;
  string new_position;
  bool valid_move_made = false;
  bool end_game = false;

  while (!valid_move_made) {
    
    cout << "Original Position?: " << endl;
    cin >> original_position;
    cout << "New Position?: " << endl;
    cin >> new_position;
    
    if (valid_move(player, original_position, new_position)) {
      cout << "Valid Move" << endl;
      make_move(original_position, new_position);
      if (player == "w") {
        if (new_position == "a1" || new_position == "k0" || new_position == "a11" || new_position == "k11") {
          end_game = true;
        }
      }
      vector<string> captured = check_for_capture(new_position);
      for (string position : captured) {
        if (root[position].asString() == "king") {
          end_game = true;
        }
      }
      remove_captured(captured);
      valid_move_made = true;
    } else {
      cout << "Invalid Move" << endl;
    }
  }
  return end_game;
}

void remove_captured(vector<string> captured) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  bool parseSuccess = reader.parse(json_raw, root, false);

  for (string position : captured) {
    root[position] = "none";
  }

  ofstream file;
  file.open("pieces");
  file << root;
  file.close();
}

vector<string> check_for_capture(string new_position) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  bool parseSuccess = reader.parse(json_raw, root, false);

  string self_piece = root[new_position].asString();
  char ally = self_piece.at(0);
  vector<string> adjacent_pieces = get_adjacent_pieces(root, new_position);
  vector<string> ready_for_capture;
  for (string position : adjacent_pieces) {
    if (root[position].asString().at(0) != ally) {
      vector<string> adjacent_pieces_2 = get_adjacent_pieces(root, position);
      int allies = 0;
      for (string position : adjacent_pieces_2) {
        if (root[position].asString().at(0) == ally) {
          allies = allies + 1;
        }
      }
      if (allies > 1) {
        ready_for_capture.push_back(position);
      }
    }
  }
  return ready_for_capture;
}

bool valid_piece(Json::Value root, string position) {
  Json::Value node = root[position];
  if (node.isNull() == 0) {
    return true;
  } else {
    return false;
  }
}

vector<string> get_adjacent_pieces(Json::Value root, string position) {
  char first_char = position.at(0);
  char second_char = position.at(1);
  vector<string> pieces;
  if (second_char != 'k') {
    if (valid_piece(root, first_char+std::to_string((second_char-'0')+1))) {
      pieces.push_back(first_char+std::to_string((second_char-'0')+1));
    }
  }
  if (second_char != 'a') {
    if (valid_piece(root, first_char+std::to_string((second_char-'0')-1))) {
      pieces.push_back(first_char+std::to_string((second_char-'0')-1));
    }
  }
  if (first_char != 11) {
    if (valid_piece(root, std::string()+(static_cast<char>(first_char+1))+second_char)) {
      pieces.push_back(std::string()+(first_char++)+second_char);
    }
  }
  if (first_char != 1) {
    if (valid_piece(root, std::string()+(static_cast<char>(first_char-1))+second_char)) {
      pieces.push_back(std::string()+(static_cast<char>(first_char-1))+second_char);
    }
  }
  return pieces;
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
  if (root[original_position].empty()) {
    cout << "no piece exists at original position (v1)" << endl;
    return false;
  } else if (root[original_position] == "none") {
    cout << "no piece exists at original position (v2)" << endl;
    return false;
  }
  if (root[new_position].empty() || root[new_position] == "none") {
    // checks if not diagonal move
    if (original_position.at(0) == new_position.at(0) || original_position.at(1) == new_position.at(1)) {
      // checks if corner 
      std::size_t corner_found = corner_pieces.find(new_position);
      if (corner_found != std::string::npos) {
        cout << "corner found" << endl;
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
            if (node.empty() == false && node != "none") {
              pieceInWay = "true";
            }
          }
          if (pieceInWay == "true") {
            cout << "piece is in vertical way" << endl;
            return false;
          } else {
            return true;
          }
        // horizontal check
        } else {
          std::string pieceInWay = "false";
          char start;
          char end;
          if (original_position.at(0) < new_position.at(0)) {
            start = static_cast<char>(original_position.at(0)+1);
            end = new_position.at(0);
          } else {
            start = new_position.at(0);
            end = static_cast<char>(original_position.at(0)-1);
          }
          for (char i=start; i < end; i = static_cast<char>(i+1)) {
            Json::Value node = root[std::string() + i + new_position.at(1)];
            if (node.empty() == false && node != "none") {
              pieceInWay = "true";
            }
          }
          if (pieceInWay == "true") {
            cout << "piece is in horizontal way" << endl;
            return false;
          } else {
            return true;
          }
        }
      }
    } else {
      cout << "diagonal move" << endl;
      return false;
    }
  } else {
    cout << "space taken" << endl;
    return false;
  }
}

void make_move(string original_position, string new_position) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  bool parseSuccess = reader.parse(json_raw, root, false);

  Json::Value piece = root[original_position];
  root[original_position] = "none";
  root[new_position] = piece;

  ofstream file;
  file.open("pieces");
  file << root;
  file.close();
}
