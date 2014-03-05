#include "multiplayer.h"

#include <iostream>
#include <thread>
#include <queue>
#include "asio.hpp"

using asio::ip::tcp;
using namespace std;

void MultiPlayer::startServerConnection() {
  std::thread server(&MultiPlayer::setUpServer, this);
  server.detach();
}

void MultiPlayer::startPlayerConnection() {
  std::thread server(&MultiPlayer::connectToPlayer, this);
  server.detach();
}

void MultiPlayer::setUpServer() {
  try {
    asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1300));

    std::cout << "Starting to listen on port 1300" << std::endl;
    for (;;) {
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      if (!moves.empty()) {
        string message = moves.back();
        cout << moves.back() << endl;
        
        asio::error_code ignored_error;
        asio::write(socket, asio::buffer(message), ignored_error);
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  std:: cout << "Finished listening" << std::endl;
}

void MultiPlayer::connectToPlayer() {
  try {
    asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", "1300");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    asio::connect(socket, endpoint_iterator);

    for (;;) {
      std::array<char, 300> buf;
      asio::error_code error;

      size_t len = socket.read_some(asio::buffer(buf), error);

      if (error == asio::error::eof) {
        break;
      } else if (error) {
        throw asio::system_error(error);
      }

      cout.write(buf.data(), len);
      cout << endl;
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
