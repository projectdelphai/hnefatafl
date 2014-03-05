#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QApplication>
#include <QPushButton>
#include <QSignalMapper>
#include <QLabel>

#include "json/json.h"
#include "multiplayer.h"
#include <queue>

#include <iostream>

using namespace std;

class Window : public QWidget
{
  Q_OBJECT

  public:
    Window(QWidget *parent = 0);
    Json::Value root;
   
  private slots: 
    void ButtonClicked(const QString text);
    void resetBoard();
    void startServer();
    void connectToPlayer();

  private:
    QGridLayout *grid;
    QSignalMapper *signalMapper;
    string original_position;
    string new_position;
    string player;
    bool isPieceChosen = false;
    QLabel *status;
    MultiPlayer *network = new MultiPlayer();

    void updateBoard();
    void freeze_window();
};

#endif
