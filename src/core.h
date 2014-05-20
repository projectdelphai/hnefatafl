#ifndef CORE_H
#define CORE_H

#include <string>
#include "json/json.h"

using namespace std;

class Core
{
  public:
    string query_next_move(string original_position, string new_position, string piecesPath = "pieces");
    string status;

  private:
    string pieces;
    bool check_for_self_capture;
    bool valid_move(string original_position, string new_position);
    void make_move(string original_position, string new_position);
    vector<string> check_for_capture(string new_position);
    void remove_captured(vector<string> captured);
    vector<string> get_adjacent_pieces(Json::Value root, string new_position);
    bool valid_piece(Json::Value root, string position);
};

#endif
