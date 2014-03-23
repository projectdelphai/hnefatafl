#ifndef OPTIONS_H
#define OPTIONS_H

#include <window.h>

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class Options : public QWidget
{
  Q_OBJECT

  public:
    Options(QWidget *parent = 0);

  private slots:
    void togglePlayerMode();
    void makeClientBlack();
    void makeClientWhite();
    void startServer();
    void resetBoard();

  private:
    QPushButton *playerMode;
    QPushButton *black;
    QPushButton *white;
    QLineEdit *address;   
};

#endif
