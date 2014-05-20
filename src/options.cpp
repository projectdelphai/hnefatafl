#include <options.h>
#include <window.h>

#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>

Options::Options(QWidget *parent) : QWidget(parent)
{

  QVBoxLayout *content = new QVBoxLayout(this);
  QHBoxLayout *topBox = new QHBoxLayout();
  QHBoxLayout *secondBox = new QHBoxLayout();
  playerMode = new QPushButton("Single");
  black = new QPushButton("Black");
  white = new QPushButton("White");
  address = new QLineEdit("1300,127.0.0.1:1400");
  QPushButton *startServer = new QPushButton("Connect");
  QPushButton *newGame = new QPushButton("New Game");
  connect(playerMode, SIGNAL(clicked()), this, SLOT(togglePlayerMode()));
  connect(black, SIGNAL(clicked()), this, SLOT(makeClientBlack()));
  connect(white, SIGNAL(clicked()), this, SLOT(makeClientWhite()));
  connect(startServer, SIGNAL(clicked()), this, SLOT(startServer()));
  connect(newGame, SIGNAL(clicked()), this, SLOT(resetBoard()));
  secondBox->addWidget(playerMode);
  secondBox->addWidget(black);
  secondBox->addWidget(white);
  secondBox->addWidget(newGame);
  topBox->addWidget(address);
  topBox->addWidget(startServer);

  content->addLayout(topBox);
  content->addLayout(secondBox);
}

void Options::setPlayerMode(bool single) {
  if (single) {
    playerMode->setText("Single");
  } else {
    playerMode->setText("Multi");
  }
}

void Options::togglePlayerMode() {
  string currentMode = playerMode->text().toUtf8().constData();
  if (currentMode == "Multi") {
    ((Window*)this->parentWidget())->singlePlayer = true;
    playerMode->setText("Single");
  } else if (currentMode == "Single") {
    ((Window*)this->parentWidget())->singlePlayer = false;
    playerMode->setText("Multi");
  } else {
  }
}

void Options::makeClientBlack() {
  ((Window*)this->parentWidget())->clientPlayer = "b";
  black->setDisabled(true);
  white->setDisabled(true);
  ((Window*)this->parentWidget())->clientPlayerChosen = true;
}

void Options::makeClientWhite() {
  ((Window*)this->parentWidget())->clientPlayer = "w";
  black->setDisabled(true);
  white->setDisabled(true);
  ((Window*)this->parentWidget())->clientPlayerChosen = true;
}

void Options::startServer() {
  ((Window*)this->parentWidget())->startServer();
}

void Options::resetBoard() {
  ((Window*)this->parentWidget())->resetBoard();
}
