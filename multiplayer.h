#ifndef MUTLIPLAYER_H
#define MULTIPLAYER_H

#include "asio.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

class MultiPlayer
{
  public:
    queue<string> moves;
    mutex mtx;
    void add(string message);
    void Producer();
    void Consumer();
    void startProducer();
    void startConsumer();
    void parseIncoming(string message);
    bool update;
};

#endif
