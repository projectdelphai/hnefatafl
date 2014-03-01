#include <window.h>
#include <json/json.h>

#include <QSignalMapper>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QLabel>
#include <QColor>
#include <QPushButton>
#include <QSignalMapper>

#include <iostream>
#include <fstream>
#include "core.h"

using namespace std;

Window::Window(QWidget *parent) : QWidget(parent)
{
  grid = new QGridLayout(this);
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

  updateBoard();
}

void Window::updateBoard() {
  int horizValues[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  for( Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++ ) {
    string position = itr.key().asString();
    int first = position.at(0)-'a';
    string x = position.substr(1);
    int second = horizValues[atoi(x.c_str())-1];
    QPushButton *btn = new QPushButton();
    btn->setMinimumWidth(55);
    btn->setMinimumHeight(55);
    QPalette pal = btn->palette();
    char piece = root[position].asString().at(0);
    if (piece == 'w' || piece == 'k') {
      pal.setColor(QPalette::Button, Qt::blue);
    } else if (piece == 'b') {
      pal.setColor(QPalette::Button, QColor(Qt::gray).dark());
    } else {
    }
    btn->setPalette(pal);
    connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
    QString pos = QString::fromStdString(position);
    signalMapper->setMapping(btn, pos);
    grid->addWidget(btn, second, first, Qt::AlignJustify);
  }
  connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(ButtonClicked(const QString &)));

}

void Window::ButtonClicked(const QString text) {
  cout << text.toUtf8().constData() << endl;
  string position = text.toUtf8().constData();
  string piece = root[position].asString();
  if (piece.empty() == false && piece != "none") {
    player = std::string() + piece.at(0);
  }

  if (isPieceChosen) {
    if (piece.empty() == true || piece == "none") {
      new_position = position;
      Core *core = new Core();
      string success = core->query_next_move(player, original_position, new_position);
      if (success == "success") {
        cout << success << endl;
        isPieceChosen = false;
        disconnect(signalMapper, 0, this, 0);
      }
      updateBoard();
    }
  } else {
    if (piece.empty() != true && piece != "none") {
      original_position = position;
      isPieceChosen = true;
    }
  }
}
