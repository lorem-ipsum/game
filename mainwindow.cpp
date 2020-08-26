#include "mainwindow.h"

#include <QAction>
#include <QApplication>  // To use 'qApp', this file must be #included.
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTextStream>

#include "background.h"
#include "bug.h"
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

  // Actions
  QAction *exitAction = new QAction(this);
  exitAction->setText("Exit");
  exitAction->setShortcut(QKeySequence("Ctrl+W"));
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  QAction *pauseAction = new QAction(this);
  pauseAction->setText("Pause");
  pauseAction->setShortcut(QKeySequence("Ctrl+G"));
  connect(pauseAction, &QAction::triggered, [=]() { snake->timer->stop(); });

  QAction *resumeAction = new QAction(this);
  resumeAction->setText("Resume");
  resumeAction->setShortcut(QKeySequence("Ctrl+R"));
  connect(resumeAction, &QAction::triggered,
          [=]() { snake->timer->start(80); });

  QAction *restartAction = new QAction(this);
  restartAction->setText("Restart");
  restartAction->setShortcut(QKeySequence("Ctrl+T"));
  connect(restartAction, &QAction::triggered, [=]() { snake->restart(); });

  // Menu
  QMenuBar *menubar = new QMenuBar(this);

  QMenu *fileMenu = new QMenu("File");
  fileMenu->addAction(exitAction);
  menubar->addMenu(fileMenu);

  QMenu *editMenu = new QMenu("Edit");
  editMenu->addAction(pauseAction);
  editMenu->addAction(resumeAction);
  editMenu->addAction(restartAction);
  menubar->addMenu(editMenu);
}

MainWindow::~MainWindow() {}
