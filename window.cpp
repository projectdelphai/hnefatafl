#include <window.h>
#include <json/json.h>

#include <QSignalMapper>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QLabel>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "core.h"

using namespace std;

Window::Window(QWidget *parent) : QWidget(parent)
{
  QVBoxLayout *vbox = new QVBoxLayout(this);
  grid = new QGridLayout();
  grid->setSpacing(0); 

  int horizValues[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Reader reader;
  reader.parse(json_raw, root, false);
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
  status = new QLabel("Status");
  QPushButton *connectToPlayer = new QPushButton("Connect");
  QPushButton *newGame = new QPushButton("New Game");
  connect(newGame, SIGNAL(clicked()), this, SLOT(resetBoard()));
  topStatus->addWidget(status);
  topStatus->addWidget(connectToPlayer);
  topStatus->addWidget(newGame);

  vbox->addLayout(topStatus);
  vbox->addLayout(grid);

  updateBoard();
}

void Window::resetBoard() {
  std::ifstream src("pieces.bk", std::ios::binary);
  std::ofstream dst("pieces", std::ios::binary);
  dst << src.rdbuf();

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
  
  disconnect(signalMapper, 0, this, 0);
  //updateBoard();
}

void Window::updateBoard() {
  int horizValues[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Reader reader;
  reader.parse(json_raw, root, false);

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

void Window::freeze_window() {
  for (int j=0; j<grid->count(); j++) {
      grid->itemAt(j)->widget()->setEnabled(false);
  }
}

void Window::ButtonClicked(const QString text) {
  string position = text.toUtf8().constData();
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
        string success = core->query_next_move(original_position, new_position);
        if (success == "bw") {
          status->setText("Black wins!");
          freeze_window();
         
        } else if (success == "ww") {
          status->setText("White wins!");
          freeze_window();
        } else if (success == "invalid") {
          QString error = QString::fromStdString(core->status);
          status->setText(error);
        }
        isPieceChosen = false;
        disconnect(signalMapper, 0, this, 0);
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
  
