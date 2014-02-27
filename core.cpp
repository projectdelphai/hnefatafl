#include "core.h"

#include <fstream>
#include <string>
#include <iostream>
#include "json/json.h"
#include <stdlib.h>
#include <sstream>

using namespace std;

string Core::query_next_move(string player, string original_position, string new_position) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  string value = "success";
  if (valid_move(original_position, new_position)) {
    make_move(original_position, new_position);
    if (player == "w") {
      if (new_position == "a1" || new_position == "k0" || new_position == "a11" || new_position == "k11") {
        value = "ww";
      }
    }
    vector<string> captured = check_for_capture(new_position);
    for (string position : captured) {
      if (root[position].asString() == "king") {
        value = "bw";
      }
    }
    remove_captured(captured);
  } else {
    value = "invalid";
  }
  return value;
}

void Core::remove_captured(vector<string> captured) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  for (string position : captured) {
    root[position] = "none";
  }

  ofstream file;
  file.open("pieces");
  file << root;
  file.close();
}

vector<string> Core::check_for_capture(string new_position) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

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

bool Core::valid_piece(Json::Value root, string position) {
  Json::Value node = root[position];
  if (node.isNull() == 0) {
    return true;
  } else {
    return false;
  }
}

vector<string> Core::get_adjacent_pieces(Json::Value root, string position) {
  char first_char = position.at(0);
  string second = position.substr(1);
  vector<string> pieces;
  if (second != "11") {
    if (valid_piece(root, first_char+std::to_string(atoi(second.c_str())+1))) {
      pieces.push_back(first_char+std::to_string((atoi(second.c_str()))+1));
    }
  }
  if (second != "1") {
    if (valid_piece(root, first_char+std::to_string((atoi(second.c_str()))-1))) {
      pieces.push_back(first_char+std::to_string((atoi(second.c_str()))-1));
    }
  }
  if (first_char != 'k') {
    if (valid_piece(root, std::string()+(static_cast<char>(first_char+1))+second)) {
      pieces.push_back(std::string()+(first_char++)+second);
    }
  }
  if (first_char != 'a') {
    if (valid_piece(root, std::string()+(static_cast<char>(first_char-1))+second)) {
      pieces.push_back(std::string()+(static_cast<char>(first_char-1))+second);
    }
  }
  return pieces;
}

bool Core::valid_move(string original_position, string new_position) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

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
    if (original_position.at(0) == new_position.at(0) || original_position.substr(1) == new_position.substr(1)) {
      // checks if corner 
      std::size_t corner_found = corner_pieces.find(new_position);
      if (corner_found != std::string::npos) {
        cout << "corner found" << endl;
        return false;
      } else {
        // checks if piece is in way
        // vertical check
        if (original_position.at(0) == new_position.at(0)) {
          int start = atoi(original_position.substr(1).c_str());
          int end = atoi(new_position.substr(1).c_str());
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
            Json::Value node = root[std::string() + i + new_position.substr(1)];
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

void Core::make_move(string original_position, string new_position) {
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  Json::Value piece = root[original_position];
  root[original_position] = "none";
  root[new_position] = piece;

  ofstream file;
  file.open("pieces");
  file << root;
  file.close();
}
