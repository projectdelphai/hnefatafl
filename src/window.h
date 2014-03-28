#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QApplication>
#include <QPushButton>
#include <QSignalMapper>
#include <QLabel>
#include <QLineEdit>

#include "json/json.h"
#include "multiplayer.h"
#include <deque>

#include <iostream>

using namespace std;

class Window : public QWidget
{
  Q_OBJECT

  public:
    Window(QWidget *parent = 0);
    bool singlePlayer = false;
    string clientPlayer;
    bool clientPlayerChosen = false;
   
  public slots: 
    void ButtonClicked(const QString text);
    void startServer();
    void update();
    void showOptions();
    void resetBoard();

  private:
    QGridLayout *grid;
    QSignalMapper *signalMapper;
    string original_position;
    string new_position;
    string player;
    bool isPieceChosen = false;
    QLabel *status;
    void updateBoard();
    MultiPlayer *network = new MultiPlayer();
    QPushButton *playerMode;
    QPushButton *black;
    QPushButton *white;
    void freeze_window(bool enabled);
    Json::Value getRoot();
    QLineEdit *address;
    string getHome();
};

#endif
