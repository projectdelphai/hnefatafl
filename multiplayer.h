#ifndef MUTLIPLAYER_H
#define MULTIPLAYER_H

#include "asio.hpp"
#include <queue>

using namespace std;

class MultiPlayer
{
  public:
    queue<string> moves;
    void setUpServer();
    void connectToPlayer();
    void startServerConnection();
    void startPlayerConnection();
};

#endif
