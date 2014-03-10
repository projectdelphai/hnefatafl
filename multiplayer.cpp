#include "multiplayer.h"

#include <iostream>
#include <thread>
#include <queue>
#include "asio.hpp"
#include "core.h"

using asio::ip::tcp;
using namespace std;

void MultiPlayer::startConnection(string ip, string port1, string port2) {
  std::thread server(&MultiPlayer::Producer, this, port1);
  server.detach();
  std::thread reader(&MultiPlayer::Consumer, this, ip, port2);
  reader.detach();
}

void MultiPlayer::add(string message) {
  mtx.lock();
  moves.push(message);
  mtx.unlock();
}

void MultiPlayer::Producer(string port) {
  try {
    asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), atoi(port.c_str())));

    for (;;) {
      if (!moves.empty()) {
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        mtx.lock();
        string message = moves.front();
        moves.pop();
        mtx.unlock();
        asio::error_code ignored_error;
        asio::write(socket, asio::buffer(message), ignored_error);
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void MultiPlayer::Consumer(string ip, string port) {
  while (true) {
    
    try {
      asio::io_service io_service;
      
      tcp::resolver resolver(io_service);
      tcp::resolver::query query(ip, port);
      tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
      
      tcp::socket socket(io_service);
      asio::connect(socket, endpoint_iterator);
      
      for (;;) {
        std::array<char, 8> buf;
        asio::error_code error;
        
        socket.read_some(asio::buffer(buf), error);
        
        if (error == asio::error::eof) {
          break;
        } else if (error) {
          throw asio::system_error(error);
        } else {
          string message = buf.data();
          parseIncoming(message);
        }
      }
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
}

void MultiPlayer::parseIncoming(string rawMessage) {
  if (rawMessage.back() == '&') {
    rawMessage.pop_back();
    if (rawMessage.back() == '&') {
      cout << rawMessage << endl;
      return;
    } else {
    }
  } else {
    if (rawMessage.front() == '!') {
      clientPlayer = rawMessage.back();
    }
    return;
  }
  size_t found = rawMessage.find(":");
  size_t found2 = rawMessage.find_last_of(":");
  string player = rawMessage.substr(0, found);
  string original_position = rawMessage.substr(found+1, found2-2); 
  string new_position = rawMessage.substr(found2+1, -1); 
  Core *core = new Core();
  core->query_next_move(original_position, new_position);
  update = true;
}
