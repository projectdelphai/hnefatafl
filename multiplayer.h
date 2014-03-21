#ifndef MUTLIPLAYER_H
#define MULTIPLAYER_H

#include "asio.hpp"
#include <deque>
#include <mutex>
#include <condition_variable>

using namespace std;

class MultiPlayer
{
  public:
    deque<string> moves;
    mutex mtx;
    void add(string message);
    void Producer(string address);
    void Consumer(string ip, string port);
    void startConnection(string ip, string port1, string port2);
    void parseIncoming(string message);
    bool update;
    string clientPlayer;
    bool verified;
    string currentMessage;
};

#endif
