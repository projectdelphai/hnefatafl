#include <window.h>

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

#include "json/json.h"

#include <iostream>
#include <fstream>

using namespace std;

Window::Window(QWidget *parent) : QWidget(parent)
{
  QGridLayout *grid = new QGridLayout(this);
  grid->setSpacing(0);

  for (int i=0; i<11; i++) {
    for (int j=0; j<11; j++) {
      QPushButton *btn = new QPushButton();
      btn->setMinimumWidth(55);
      btn->setMinimumHeight(55);
      grid->addWidget(btn, i, j);
    }
  }
  
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  int horizValues[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  signalMapper = new QSignalMapper(this);
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
    } else {
      pal.setColor(QPalette::Button, QColor(Qt::gray).dark());
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
  string player = std::string() + piece.at(0);

  if (isPieceChosen) {
    if (piece.empty() == true || piece == "none") {
      new_position = position;
    }
  } else {
    if (piece.empty() != true && piece != "none") {
      old_position = position;
      isPieceChosen = true;
    }
  }
}
