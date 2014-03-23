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
   
  private slots: 
    void ButtonClicked(const QString text);
    void resetBoard();
    void startServer();
    void update();
    void makeClientWhite();
    void makeClientBlack();
    void togglePlayerMode();
    void showOptions();

  private:
    QGridLayout *grid;
    QSignalMapper *signalMapper;
    string original_position;
    string new_position;
    string player;
    string clientPlayer;
    bool isPieceChosen = false;
    QLabel *status;
    void updateBoard();
    MultiPlayer *network = new MultiPlayer();
    QPushButton *playerMode;
    QPushButton *black;
    QPushButton *white;
    bool clientPlayerChosen = false;
    void freeze_window(bool enabled);
    Json::Value getRoot();
    QLineEdit *address;
    bool singlePlayer = false;
};

#endif
