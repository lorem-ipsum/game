#include "mainwindow.h"

#include <QAction>
#include <QApplication>  // To use 'qApp', this file must be #included.
#include <QButtonGroup>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTextStream>
#include <QToolBar>

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

  QAction *pauseAction = new QAction(this);
  pauseAction->setText("Pause");
  pauseAction->setShortcut(QKeySequence("Ctrl+G"));

  QAction *resumeAction = new QAction(this);
  resumeAction->setText("Resume");
  resumeAction->setShortcut(QKeySequence("Ctrl+R"));

  QAction *restartAction = new QAction(this);
  restartAction->setText("Restart");
  restartAction->setShortcut(QKeySequence("Ctrl+T"));

  QAction *saveAction = new QAction(this);
  saveAction->setText("Save game");
  saveAction->setShortcut(QKeySequence("Ctrl+S"));

  QAction *loadAction = new QAction(this);
  loadAction->setText("Load game");
  loadAction->setShortcut(QKeySequence("Ctrl+L"));

  QAction *gameOverAction = new QAction(this);

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
    btns->buttons()[i]->setFont(QFont("Helvetica"));
    btns->buttons()[i]->setGeometry(1280, 640 + 80 * i, 80, 40);
  }

  // Toolbar
  QToolBar *toolBar = new QToolBar(this);
  toolBar->setGeometry(30, 30, 1100, 40);
  toolBar->addActions({startAction, pauseAction, resumeAction, restartAction,
                       saveAction, loadAction, exitAction});
  toolBar->setMovable(false);

  // this->addToolBar(Qt::RightToolBarArea, toolBar);
  this->addToolBar(toolBar);
  // toolBar->setGeometry(20, 20, 100, 50);
  // toolBar->setStyleSheet(
  //     "background-color:rgb(200,40,43);color:rgb(204,204,204)");

  // Initial state

  startAction->setEnabled(true);
  pauseAction->setEnabled(false);
  resumeAction->setEnabled(false);
  restartAction->setEnabled(false);
  saveAction->setEnabled(false);
  loadAction->setEnabled(true);

  startBtn->setEnabled(true);
  pauseBtn->setEnabled(false);
  resumeBtn->setEnabled(false);
  restartBtn->setEnabled(false);
  saveBtn->setEnabled(false);
  loadBtn->setEnabled(true);

  // setFocusProxy(startBtn);

  connect(startAction, &QAction::triggered, [=]() {
    snake->timer->start(80);

    // disable actions and hide buttons
    startAction->setEnabled(false);
    pauseAction->setEnabled(true);
    resumeAction->setEnabled(false);
    restartAction->setEnabled(false);
    saveAction->setEnabled(false);
    loadAction->setEnabled(false);

    startBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    resumeBtn->setEnabled(false);
    restartBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    loadBtn->setEnabled(false);

    snake->timeToToggleObstacles = false;

    setFocusProxy(snake);
    snake->setFocus();
    // snake->setFocusPolicy(Qt::StrongFocus);
  });

  connect(pauseAction, &QAction::triggered, [=]() {
    snake->timer->stop();

    // disable actions and hide buttons
    startAction->setEnabled(false);
    pauseAction->setEnabled(false);
    resumeAction->setEnabled(true);
    restartAction->setEnabled(true);
    saveAction->setEnabled(true);
    loadAction->setEnabled(false);

    startBtn->setEnabled(false);
    pauseBtn->setEnabled(false);
    resumeBtn->setEnabled(true);
    restartBtn->setEnabled(true);
    saveBtn->setEnabled(true);
    loadBtn->setEnabled(false);

    snake->timeToToggleObstacles = false;

    setFocusProxy(snake);
    snake->setFocus();
    // snake->setFocusPolicy(Qt::StrongFocus);
  });

  connect(resumeAction, &QAction::triggered, [=]() {
    snake->timer->start(80);

    // disable actions and hide buttons
    startAction->setEnabled(false);
    pauseAction->setEnabled(true);
    resumeAction->setEnabled(false);
    restartAction->setEnabled(false);
    saveAction->setEnabled(false);
    loadAction->setEnabled(false);

    startBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    resumeBtn->setEnabled(false);
    restartBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    loadBtn->setEnabled(false);

    snake->timeToToggleObstacles = false;

    setFocusProxy(snake);
    snake->setFocus();
    // snake->setFocusPolicy(Qt::StrongFocus);
  });

  connect(restartAction, &QAction::triggered, [=]() {
    snake->restart();

    // disable actions and hide buttons
    startAction->setEnabled(true);
    pauseAction->setEnabled(false);
    resumeAction->setEnabled(false);
    restartAction->setEnabled(false);
    saveAction->setEnabled(false);
    loadAction->setEnabled(true);

    startBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    resumeBtn->setEnabled(false);
    restartBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    loadBtn->setEnabled(true);

    snake->timeToToggleObstacles = true;

    setFocusProxy(snake);
    snake->setFocus();
    // snake->setFocusPolicy(Qt::StrongFocus);
  });

  connect(saveAction, &QAction::triggered, [=]() {
    // Save game
    qDebug() << "Save";

    QFile saveFile("save.json");

    saveFile.open(QIODevice::WriteOnly);

    QFileDialog::getSaveFileName(this, tr("Choose a directory to save"),
                                 "save.json", tr("JSON Files(*json)"));

    // Write JSON to 'saveFile'

    QJsonObject rootObject;

    // Save body
    QJsonArray bodyArray;
    for (POS bodygrid : snake->getBody()) {
      QJsonArray grid;
      grid.append(QJsonValue(bodygrid.first));
      grid.append(QJsonValue(bodygrid.second));
      bodyArray.append(grid);
    }
    rootObject.insert("body", bodyArray);

    // Save obstacles
    QJsonArray obsArray;
    for (POS obsgrid : snake->getObstacles()) {
      QJsonArray obs;
      obs.append(QJsonValue(obsgrid.first));
      obs.append(QJsonValue(obsgrid.second));
      obsArray.append(obs);
    }
    rootObject.insert("obstacles", obsArray);

    // Save bug
    QJsonArray bugArray;
    bugArray.append(QJsonValue(snake->getBug().first));
    bugArray.append(QJsonValue(snake->getBug().second));
    rootObject.insert("bug", bugArray);

    // Save dir
    // Code of shit, could anyone help me?
    if (snake->getDir() == UP)
      rootObject.insert("dir", "UP");
    else if (snake->getDir() == DOWN)
      rootObject.insert("dir", "DOWN");
    else if (snake->getDir() == RIGHT)
      rootObject.insert("dir", "RIGHT");
    else if (snake->getDir() == LEFT)
      rootObject.insert("dir", "LEFT");

    // Save time
    rootObject.insert("time", QJsonValue(snake->getTime()));

    // Save doc
    QJsonDocument jsonDoc;
    jsonDoc.setObject(rootObject);
    saveFile.write(jsonDoc.toJson());
    saveFile.close();

    // disable actions and hide buttons
    // startAction->setEnabled(false);
    // pauseAction->setEnabled(false);
    // resumeAction->setEnabled(false);
    // restartAction->setEnabled(false);
    // saveAction->setEnabled(false);
    // loadAction->setEnabled(false);

    snake->timeToToggleObstacles = false;

    setFocusProxy(snake);
    snake->setFocus();
    // snake->setFocusPolicy(Qt::StrongFocus);
  });

  connect(loadAction, &QAction::triggered, [=]() {
    // Load game
    qDebug() << "Load";

    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Choose a file to load in"), "", tr("JSON Files(*json)"));
    // qDebug() << "filename : " << fileName;

    if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (file.open(QFile::ReadOnly)) {
        QByteArray allData = file.readAll();
        file.close();
        // qDebug() << array;

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

        if (json_error.error != QJsonParseError::NoError) {
          qDebug() << "json error!";
          exit(1);
        } else {
          QJsonObject rootObj = jsonDoc.object();
          QJsonArray body = rootObj.value("body").toArray();
          QJsonArray obstacles = rootObj.value("obstacles").toArray();
          QJsonArray bug = rootObj.value("bug").toArray();
          QString strdir = rootObj.value("dir").toString();

          // Code of shit. Is there a simple way of converting string to enum?
          DIR dir = RIGHT;
          if (strdir == "UP")
            dir = UP;
          else if (strdir == "DOWN")
            dir = DOWN;
          else if (strdir == "RIGHT")
            dir = RIGHT;
          else if (strdir == "LEFT")
            dir = LEFT;

          int time = rootObj.value("time").toInt();

          snake->load(body, obstacles, bug, dir, time);
        }
      }
    }

    // disable actions and hide buttons
    // startAction->setEnabled(false);
    // pauseAction->setEnabled(false);
    // resumeAction->setEnabled(false);
    // restartAction->setEnabled(false);
    // saveAction->setEnabled(false);
    // loadAction->setEnabled(false);

    snake->timeToToggleObstacles = false;

    setFocusProxy(snake);
    snake->setFocus();
    // snake->setFocusPolicy(Qt::StrongFocus);
  });

  connect(snake, SIGNAL(gameOver()), gameOverAction, SLOT(trigger()));

  connect(gameOverAction, &QAction::triggered, [=]() {
    qDebug() << "Game Over";
    // disable actions and hide buttons
    startAction->setEnabled(false);
    pauseAction->setEnabled(false);
    resumeAction->setEnabled(false);
    restartAction->setEnabled(true);
    saveAction->setEnabled(false);
    loadAction->setEnabled(false);

    startBtn->setEnabled(false);
    pauseBtn->setEnabled(false);
    resumeBtn->setEnabled(false);
    restartBtn->setEnabled(true);
    saveBtn->setEnabled(false);
    loadBtn->setEnabled(false);

    setFocusProxy(snake);
    snake->setFocus();
  });

  // snake->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() {}
