#include "mainwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QTextStream>

#include "background.h"
#include "snake.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  resize(1500, 1500);

  Background *bg = new Background(this);
  bg->setGeometry(120, 120, 648, 648);

  Snake *snake = new Snake(this);
  snake->setGeometry(120, 120, 648, 648);

  connect(this, SIGNAL(UP_pressed()), snake, SLOT(dirUP()));
  connect(this, SIGNAL(DOWN_pressed()), snake, SLOT(dirDOWN()));
  connect(this, SIGNAL(RIGHT_pressed()), snake, SLOT(dirRIGHT()));
  connect(this, SIGNAL(LEFT_pressed()), snake, SLOT(dirLEFT()));
}

MainWindow::~MainWindow() {}
