#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QSignalMapper>
#include "json/json.h"

#include <iostream>

using namespace std;

class Window : public QWidget
{
  Q_OBJECT

  public:
    Window(QWidget *parent = 0);
   
  private slots: 
    void ButtonClicked(const QString text);

  private:
    QSignalMapper *signalMapper;
    string old_position;
    string new_position;
    bool isPieceChosen;
    Json::Value root;
};

#endif
