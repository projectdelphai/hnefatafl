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
      QFrame *frame = new QFrame();
      frame->setLineWidth(1);
      frame->setFrameShape(QFrame::Box);
      grid->addWidget(frame, i, j);
    }
  }
  
  std::ifstream ifs("pieces");
  std::string json_raw( (std::istreambuf_iterator<char>(ifs) ),
      (std::istreambuf_iterator<char>() ) );
  Json::Value root;
  Json::Reader reader;
  reader.parse(json_raw, root, false);

  int horizValues[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  for( Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++ ) {
    string position = itr.key().asString();
    int first = position.at(0)-'a';
    string x = position.substr(1);
    int second = horizValues[atoi(x.c_str())-1];
    QPixmap image(45,45);
    image.fill(palette().color(QPalette::Window));
    QPainter painter(&image);
    char piece = root[position].asString().at(0);
    if (piece == 'w' || piece == 'k') {
      painter.setBrush(Qt::white);
    } else {
      painter.setBrush(Qt::black);
    }
    painter.drawEllipse(9,9,30,30);
    QLabel *label = new QLabel();
    label->setPixmap(image);
    grid->addWidget(label, second, first, Qt::AlignJustify);
  }
}
