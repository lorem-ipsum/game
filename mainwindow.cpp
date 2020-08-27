#include "mainwindow.h"

#include <QAction>
#include <QApplication>  // To use 'qApp', this file must be #included.
#include <QButtonGroup>
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

  bg = new Background(this);
  bg->setGeometry(120, 120, 1280, 1280);

  snake = new Snake(this);
  snake->setGeometry(120, 120, 1280, 1280);

  connect(this, SIGNAL(UP_pressed()), snake, SLOT(dirUP()));
  connect(this, SIGNAL(DOWN_pressed()), snake, SLOT(dirDOWN()));
  connect(this, SIGNAL(RIGHT_pressed()), snake, SLOT(dirRIGHT()));
  connect(this, SIGNAL(LEFT_pressed()), snake, SLOT(dirLEFT()));

  connect(snake, &Snake::gameOver, [=]() {
    snake->timer->stop();
    // TODO
  });

  // Actions
  QAction *exitAction = new QAction(this);
  exitAction->setText("Exit");
  exitAction->setShortcut(QKeySequence("Ctrl+W"));
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  QAction *startAction = new QAction(this);
  startAction->setText("Start");
  startAction->setShortcut(QKeySequence("Ctrl+B"));
  connect(startAction, &QAction::triggered, [=]() {
    snake->timer->start(80);

    setFocusProxy(snake);

    // hide buttons
  });

  QAction *pauseAction = new QAction(this);
  pauseAction->setText("Pause");
  pauseAction->setShortcut(QKeySequence("Ctrl+G"));
  connect(pauseAction, &QAction::triggered, [=]() {
    snake->timer->stop();
    setFocusProxy(snake);

    // hide buttons
  });

  QAction *resumeAction = new QAction(this);
  resumeAction->setText("Resume");
  resumeAction->setShortcut(QKeySequence("Ctrl+R"));
  connect(resumeAction, &QAction::triggered, [=]() {
    snake->timer->start(80);
    setFocusProxy(snake);

    // hide buttons
  });

  QAction *restartAction = new QAction(this);
  restartAction->setText("Restart");
  restartAction->setShortcut(QKeySequence("Ctrl+T"));
  connect(restartAction, &QAction::triggered, [=]() {
    snake->restart();
    setFocusProxy(snake);

    // hide buttons
  });

  QAction *saveAction = new QAction(this);
  saveAction->setText("Save game");
  saveAction->setShortcut(QKeySequence("Ctrl+S"));
  connect(saveAction, &QAction::triggered, [=]() {
    // Save game
    qDebug() << "Save";
    setFocusProxy(snake);

    // hide buttons
  });

  QAction *loadAction = new QAction(this);
  loadAction->setText("Load game");
  loadAction->setShortcut(QKeySequence("Ctrl+L"));
  connect(loadAction, &QAction::triggered, [=]() {
    // Load game
    qDebug() << "Load";
    setFocusProxy(snake);

    // hide buttons
  });

  // Menu
  QMenuBar *menubar = new QMenuBar(this);

  QMenu *fileMenu = new QMenu("File");
  fileMenu->addAction(saveAction);
  fileMenu->addAction(loadAction);
  fileMenu->addAction(exitAction);
  menubar->addMenu(fileMenu);

  QMenu *editMenu = new QMenu("Edit");
  editMenu->addAction(startAction);
  editMenu->addAction(pauseAction);
  editMenu->addAction(resumeAction);
  editMenu->addAction(restartAction);
  menubar->addMenu(editMenu);

  QPushButton *startBtn = new QPushButton("Start", this);
  QPushButton *pauseBtn = new QPushButton("Pause", this);
  QPushButton *resumeBtn = new QPushButton("Resume", this);
  QPushButton *restartBtn = new QPushButton("Restart", this);
  QPushButton *saveBtn = new QPushButton("Save", this);
  QPushButton *loadBtn = new QPushButton("Load", this);
  QPushButton *exitBtn = new QPushButton("Exit", this);

  connect(startBtn, SIGNAL(clicked()), startAction, SLOT(trigger()));
  connect(pauseBtn, SIGNAL(clicked()), pauseAction, SLOT(trigger()));
  connect(resumeBtn, SIGNAL(clicked()), resumeAction, SLOT(trigger()));
  connect(restartBtn, SIGNAL(clicked()), restartAction, SLOT(trigger()));
  connect(saveBtn, SIGNAL(clicked()), saveAction, SLOT(trigger()));
  connect(loadBtn, SIGNAL(clicked()), loadAction, SLOT(trigger()));
  connect(exitBtn, SIGNAL(clicked()), exitAction, SLOT(trigger()));

  QButtonGroup *btns = new QButtonGroup(this);
  btns->addButton(startBtn, 0);
  btns->addButton(pauseBtn, 1);
  btns->addButton(resumeBtn, 2);
  btns->addButton(restartBtn, 3);
  btns->addButton(saveBtn, 4);
  btns->addButton(loadBtn, 5);
  btns->addButton(exitBtn, 6);

  for (int i = 0; i < 7; ++i) {
    btns->buttons()[i]->setGeometry(1280, 640 + 80 * i, 80, 40);
  }
  // int i = 0;
  // for (auto btn : btns->buttons()) {

  //   ++i;
  // }

  //   btn->setFont(QFont("Helvetica"));
  //   btn->setGeometry(1280, 640 + 80 * i, 80, 40);

  snake->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() {}
