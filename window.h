#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGridLayout>
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
    QGridLayout *grid;
    QSignalMapper *signalMapper;
    string original_position;
    string new_position;
    string player;
    bool isPieceChosen = false;
    Json::Value root;

    void updateBoard();
};

#endif
