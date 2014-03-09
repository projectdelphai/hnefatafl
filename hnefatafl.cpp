#include "window.h"
#include "core.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char* argv[]) {
  int width = 600;
  int height = 600;
  int screenWidth;
  int screenHeight;
  int x, y;

  QApplication app(argc, argv);
  QDesktopWidget *desktop = QApplication::desktop();

  screenWidth = desktop->width();
  screenHeight = desktop->height();
  x = (screenWidth - width) / 2;
  y = (screenHeight - height) / 2;

  Window window;
  window.resize(width, height);
  window.move(x,y);
  window.setWindowTitle("Hnefatafl");
  window.show();
  
  return app.exec();
}
