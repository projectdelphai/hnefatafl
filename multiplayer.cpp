#include "multiplayer.h"

#include <iostream>
#include <thread>
#include <queue>
#include "asio.hpp"

using asio::ip::tcp;
using namespace std;

void MultiPlayer::startProducer() {
  std::thread server(&MultiPlayer::Producer, this);
  server.detach();
}

void MultiPlayer::startConsumer() {
  std::thread server(&MultiPlayer::Consumer, this);
  server.detach();
}

void MultiPlayer::add(string message) {
  mtx.lock();
  cout << "adding move" << endl;
  moves.push(message);
  cout << moves.back() << endl;
  mtx.unlock();
}

void MultiPlayer::Producer() {
  try {
    asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1300));

    std::cout << "Starting to listen on port 1300" << std::endl;
    for (;;) {
      if (!moves.empty()) {
        cout << "listening" << endl;
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        mtx.lock();
        string message = moves.front();
        moves.pop();
        mtx.unlock();
        asio::error_code ignored_error;
        asio::write(socket, asio::buffer(message), ignored_error);
        cout << "finished" << endl;
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  std:: cout << "Finished listening" << std::endl;
}

void MultiPlayer::Consumer() {
  while (true) {
    
    try {
      asio::io_service io_service;
      
      tcp::resolver resolver(io_service);
      tcp::resolver::query query("localhost", "1300");
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
          parseIncoming(rawMessage);
        }
      }
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
}

void parseIncoming(string rawMessage) {
  cout << rawMessage << endl;
}
