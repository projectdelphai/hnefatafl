#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QSignalMapper>

class Window : public QWidget
{
  Q_OBJECT

  public:
    Window(QWidget *parent = 0);
    
  private:
    QSignalMapper *signalMapper;
};

#endif
