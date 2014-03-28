#include <window.h>
#include <options.h>
#include <json/json.h>

#include <QDesktopWidget>
#include <QSignalMapper>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QLabel>
#include <QTimer>
#include <QLineEdit>
#include <QDir>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "core.h"
#include <deque>
#include <thread>

using namespace std;

Window::Window(QWidget *parent) : QWidget(parent)
{
  QVBoxLayout *vbox = new QVBoxLayout(this);
  grid = new QGridLayout();
  grid->setSpacing(0); 

  int horizValues[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  Json::Value root = getRoot(); 
  signalMapper = new QSignalMapper(this);

  for (int i=0; i<11; i++) {
    for (int j=0; j<11; j++) {
      QPushButton *btn = new QPushButton();
      btn->setMinimumWidth(55);
      btn->setMinimumHeight(55);
      string second = std::to_string(horizValues[i]+1);
      string first = std::string() + "abcdefghijk"[j];
      connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
      QString pos = QString::fromStdString(first+second);
      signalMapper->setMapping(btn, pos);
      grid->addWidget(btn, i, j);
    }
  }

  QHBoxLayout *topStatus = new QHBoxLayout();
  QHBoxLayout *bottomStatus = new QHBoxLayout();
  status = new QLabel("Status");
  QPushButton *opts = new QPushButton("Opts");
  connect(opts, SIGNAL(clicked()), this, SLOT(showOptions()));
  bottomStatus->addWidget(status);
  topStatus->addWidget(opts);
  vbox->addLayout(topStatus);
  vbox->addLayout(grid);
  vbox->addLayout(bottomStatus);

  updateBoard();

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(1000);
}

void Window::showOptions() {
  int width = 300;
  int height = 300;
  int screenWidth;
  int screenHeight;
  int x, y;

  QDesktopWidget *desktop = QApplication::desktop();
  screenWidth = desktop->width();
  screenHeight = desktop->height();
  x = (screenWidth - width)/2;
  y = (screenHeight - height)/2;
  
  Options *options = new Options();
  options->setParent(this, Qt::Dialog);
  options->resize(width, height);
  options->move(x,y);
  options->setWindowTitle("Options");
  options->show();
}

string Window::getHome() {
  string homePath = QDir::homePath().toUtf8().constData();
  string pieces = homePath + "/.hnefatafl-data";
  return pieces;
}

void Window::update() {
  if (network->update) {
    updateBoard();
    network->update = false;
    clientPlayer = network->clientPlayer;
    Json::Value root = getRoot();
    if (root["player"].asString() == player) {
      freeze_window(false);
    }
  }
}

void Window::startServer() {
  if (clientPlayerChosen) {
    string rawAddress = (address->text()).toUtf8().constData();
    string firstAddress = rawAddress.substr(0, rawAddress.find(","));
    string secondAddress = rawAddress.substr(rawAddress.find(",")+1, -1);
    size_t found = firstAddress.find(":");
    string port1 = firstAddress.substr(found+1, -1);
    found = secondAddress.find(":");
    string ip = secondAddress.substr(0, found);
    string port2 = secondAddress.substr(found+1, -1);
    network->startConnection(ip, port1, port2);
    network->add("!player="+clientPlayer);
    Json::Value root = getRoot();
    if (clientPlayer != root["player"].asString()) {
      freeze_window(true);
    }
  }
}

void Window::resetBoard() {
  string pieces = "{\"player\":\"b\",\"d1\":\"b01\",\"e1\":\"b02\",\"f1\":\"b03\",\"g1\":\"b04\",\"h1\":\"b05\",\"f2\":\"b06\",\"k4\":\"b07\",\"k5\":\"b08\",\"k6\":\"b09\",\"k7\":\"b10\",\"k8\":\"b11\",\"j6\":\"b12\",\"d11\":\"b13\",\"e11\":\"b14\",\"f11\":\"b15\",\"g11\":\"b16\",\"h11\":\"b17\",\"f10\":\"b18\",\"a8\":\"b19\",\"a7\":\"b20\",\"a6\":\"b21\",\"a5\":\"b22\",\"a4\":\"b23\",\"b6\":\"b24\",\"f4\":\"w01\",\"e5\":\"w02\",\"f5\":\"w03\",\"g5\":\"w04\",\"d6\":\"w05\",\"e6\":\"w06\",\"f6\":\"king\",\"g6\":\"w08\",\"h6\":\"w09\",\"e7\":\"w10\",\"f7\":\"w11\",\"g7\":\"w12\",\"f8\":\"w13\"}";
  ofstream savefile;
  savefile.open(getHome());
  savefile << pieces;
  savefile.close();

  for (int i=0; i<11; i++) {
    for (int j=0; j<11; j++) {
      QPushButton *btn = (QPushButton*)grid->itemAtPosition(i,j)->widget();
      QPushButton *defaultBtn = new QPushButton();
      QPalette pal = btn->palette();
      QPalette pal2 = defaultBtn->palette();
      pal.setColor(QPalette::Button, pal2.color(QPalette::Button));
      btn->setPalette(pal);
    }
  }

  updateBoard();
}

Json::Value Window::getRoot() {
  Json::Value root = new Json::Value();
  std::ifstream ifs(getHome());
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Reader reader;
  bool parseSuccess = reader.parse(json_raw, root, false);
  if (parseSuccess) {
    return root;
  } else {
    cout << "Error in parsing: " << endl;
    cout << reader.getFormatedErrorMessages() << endl;
  }
  return root;
}

void Window::updateBoard() {
  disconnect(signalMapper, 0, this, 0);
  int horizValues[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

  Json::Value root = getRoot(); 

  for( Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++ ) {
    if (itr.key().asString() == "player") {
      player = root[itr.key().asString()].asString();
    } else {
      string position = itr.key().asString();
      int first = position.at(0)-'a';
      string x = position.substr(1);
      int second = horizValues[atoi(x.c_str())-1];
      QPushButton *btn = (QPushButton*)grid->itemAtPosition(second, first)->widget();
      QPalette pal = btn->palette();
      char piece = root[position].asString().at(0);
      if (piece == 'w') {
        pal.setColor(QPalette::Button, Qt::blue);
      } else if (piece == 'b') {
        pal.setColor(QPalette::Button, QColor(Qt::gray).dark());
      } else if (piece == 'k') {
        pal.setColor(QPalette::Button, QColor(Qt::blue).dark());
      } else {
        QPushButton *btn2 = new QPushButton();
        QPalette pal2 = btn2->palette();
        pal.setColor(QPalette::Button, pal2.color(QPalette::Button));
      }
      btn->setPalette(pal);
    }
  }
  connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(ButtonClicked(const QString &)));
}

void Window::freeze_window(bool enabled) {
  for (int j=0; j<grid->count(); j++) {
    if (enabled) {
      grid->itemAt(j)->widget()->setEnabled(false);
    } else {
      grid->itemAt(j)->widget()->setEnabled(true);
    }
  }
}

void Window::ButtonClicked(const QString text) {
  string position = text.toUtf8().constData();
  Json::Value root = getRoot(); 
  string piece = root[position].asString();
  string color;
  if (piece.empty() == false && piece != "none") {
    color = std::string() + piece.at(0);
    if (color == "k") {
      color = "w";
    }
  }
  if (color == player || isPieceChosen == true) {
    if (isPieceChosen) {
      if (piece.empty() == true || piece == "none") {
        new_position = position;
        Core *core = new Core();
        string success = core->query_next_move(original_position, new_position, getHome());
        if (!singlePlayer) {
          freeze_window(true);
        }
        if (success == "success") {
          string message = player + ":" + original_position + ":" + new_position + "&";
          network->add(message);
        } else if (success == "bw") {
          status->setText("Black wins!");
          freeze_window(true);
        } else if (success == "ww") {
          status->setText("White wins!");
          freeze_window(true);
        } else if (success == "invalid") {
          QString error = QString::fromStdString(core->status);
          status->setText(error);
        }
        isPieceChosen = false;
        updateBoard();
      }
    } else {
      if (piece.empty() != true && piece != "none") {
        original_position = position;
        isPieceChosen = true;
      }
    }
  }
}
