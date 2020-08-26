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
}

MainWindow::~MainWindow() {}
