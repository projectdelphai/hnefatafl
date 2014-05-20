#include "core.h"

#include <fstream>
#include <string>
#include <iostream>
#include "json/json.h"
#include <stdlib.h>
#include <sstream>

#include <QDir>

using namespace std;

string Core::query_next_move(string original_position, string new_position, string piecesPath) {
  pieces = piecesPath;
  std::ifstream ifs(pieces);
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  string value = "success";
  if (valid_move(original_position, new_position)) {
    make_move(original_position, new_position);
    if (root[original_position].asString() == "king") {
      if (new_position == "a1" || new_position == "k1" || new_position == "a11" || new_position == "k11") {
        value = "ww";
      }
    }
    check_for_self_capture = false;
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
  std::ifstream ifs(pieces);
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  for (string position : captured) {
    root[position] = "none";
  }

  ofstream file;
  file.open(pieces);
  file << root;
  file.close();
}

vector<string> Core::check_for_capture(string new_position) {
  std::ifstream ifs(pieces);
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
      if (!check_for_self_capture) {
        check_for_self_capture = true;
        vector<string> captured = check_for_capture(position);
        remove_captured(captured);
      }
      vector<string> adjacent_pieces_2 = get_adjacent_pieces(root, position);
      int allies = 0;
      for (string position : adjacent_pieces_2) {
        if (ally == 'k' && root[position].asString().at(0) == 'w') {
          allies = allies + 1;
        }
        if (ally == 'w' && root[position].asString().at(0) == 'k') {
          allies = allies + 1;
        }
        if (root[position].asString().at(0) == ally) {
          allies = allies + 1;
        }
      }
      if (allies > 1) {
        if (root[position].asString() == "king") {
          if (allies > 3 && ally != 'w') {
            ready_for_capture.push_back(position);
          }
        } else {
          ready_for_capture.push_back(position);
        }
      }
    }
  }
  return ready_for_capture;
}

bool Core::valid_piece(Json::Value root, string position) {
  Json::Value node = root[position];
  if (node.empty() == false && node != "none") {
    return true;
  } else {
    return false;
  }
}

vector<string> Core::get_adjacent_pieces(Json::Value root, string position) {
  char first_char = position.at(0);
  string second = position.substr(1);
  vector<string> pieces;
  // check for piece above
  if (second != "11") {
    stringstream ss;
    ss << atoi(second.c_str())+1;
    string secondString = ss.str();
    if (valid_piece(root, first_char+secondString)) {
      pieces.push_back(first_char+secondString);
    }
  }
  // check for piece below
  if (second != "1") {
    stringstream ss;
    ss << atoi(second.c_str())-1;
    string secondString = ss.str();
    if (valid_piece(root, first_char+secondString)) {
      pieces.push_back(first_char+secondString);
    }
  }
  // check for piece to right
  if (first_char != 'k') {
    if (valid_piece(root, std::string()+(static_cast<char>(first_char+1))+second)) {
      pieces.push_back(std::string()+(static_cast<char>(first_char+1))+second);
    }
  }
  // check for piece to left
  if (first_char != 'a') {
    if (valid_piece(root, std::string()+(static_cast<char>(first_char-1))+second)) {
      pieces.push_back(std::string()+(static_cast<char>(first_char-1))+second);
    }
  }
  return pieces;
}

bool Core::valid_move(string original_position, string new_position) {
  std::ifstream ifs(pieces);
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  string current_turn = "b";
  string corner_pieces = "a1 a11 k1 k11";
  if (root[original_position].empty() || root[original_position] == "none") {
    status = "Starting piece doesn't exist";
    return false;
  }
  if (root[new_position].empty() || root[new_position] == "none") {
    // checks if not diagonal move
    if (original_position.at(0) == new_position.at(0) || original_position.substr(1) == new_position.substr(1)) {
      // checks if corner 
      std::size_t corner_found = corner_pieces.find(new_position);
      if (corner_found != std::string::npos) {
        if (root[original_position].asString() != "king") {
          status = "Cannot place piece in corner";
          return false;
        }
        return true;
      } else {
        // checks if piece is in way
        // vertical check
        if (original_position.at(0) == new_position.at(0)) {
          int start = atoi(original_position.substr(1).c_str());
          int end = atoi(new_position.substr(1).c_str());
          std::string pieceInWay = "false";
          for (int i=start+1; i < end+1; i++) {
            stringstream ss;
            ss << i;
            string s = ss.str();
            Json::Value node = root[original_position.at(0) + s];
            if (node.empty() == false && node != "none") {
              pieceInWay = "true";
            }
          }
          if (pieceInWay == "true") {
            status = "There is a piece in the way";
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
            end = static_cast<char>(original_position.at(0));
          }
          for (char i=start; i < end; i = static_cast<char>(i+1)) {
            Json::Value node = root[std::string() + i + new_position.substr(1)];
            if (node.empty() == false && node != "none") {
              pieceInWay = "true";
            }
          }
          if (pieceInWay == "true") {
            status = "There is a piece in the way";
            return false;
          } else {
            return true;
          }
        }
      }
    } else {
      status = "Pieces cannot move diagonally";
      return false;
    }
  } else {
    status = "That space is already taken";
    return false;
  }
}

void Core::make_move(string original_position, string new_position) {
  std::ifstream ifs(pieces);
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  Json::Value piece = root[original_position];
  root[original_position] = "none";
  root[new_position] = piece;

  map<string,string> players;
  players["w"] = "b";
  players["b"] = "w";
  string old_player = root["player"].asString();
  string current_player = players[old_player];
  root["player"] = current_player;

  ofstream file;
  file.open(pieces);
  file << root;
  file.close();
}
