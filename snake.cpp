#include "snake.h"

#include <QDebug>
#include <QPainter>
#include <QTextStream>
#include <QTimer>

Snake::Snake(QWidget* parent) : QWidget(parent) {
  body_.push_front({19, 19});
  body_.push_front({20, 19});
  afterEating_ = 0;
  obstacles_ = {};
  dir_ = RIGHT;
  time_ = 0;
  timeToToggleObstacles = true;

  bug = new Bug(body_, obstacles_, this);

  timer = new QTimer(this);
  // timer->start(80);
  connect(timer, SIGNAL(timeout()), this, SLOT(oneMove()));
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

Snake::~Snake() {}

void Snake::restart() {
  // Code of shit. Is there a better approach?
  body_ = {};
  body_.push_front({19, 19});
  body_.push_front({20, 19});
  dir_ = RIGHT;
  time_ = 0;
  afterEating_ = 0;
  obstacles_ = {};
  timeToToggleObstacles = true;

  delete bug;
  bug = new Bug(body_, obstacles_, this);
  delete timer;
  timer = new QTimer(this);
  // timer->start(80);
  connect(timer, SIGNAL(timeout()), this, SLOT(oneMove()));
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));

  update();
}

void Snake::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  QPainter* painter = new QPainter(this);
  painter->setPen(Qt::NoPen);

  // Draw the snake
  for (POS grid : body_) {
    painter->fillRect(5 + 28 * grid.first, 5 + 28 * grid.second, 26, 26,
                      Qt::green);
  }

  // Draw the obstacles
  for (POS obs : obstacles_) {
    painter->fillRect(5 + 28 * obs.first, 5 + 28 * obs.second, 26, 26,
                      Qt::gray);
  }

  // Draw the bug
  painter->fillRect(5 + 28 * bug->getPs().first, 5 + 28 * bug->getPs().second,
                    26, 26, Qt::red);

  painter->end();
}

void Snake::oneMove() {
  // qDebug() << "oneMove!";
  ++time_;

  POS next = body_.front();
  switch (dir_) {
    case UP:
      next = {next.first, next.second - 1};
      break;
    case DOWN:
      next = {next.first, next.second + 1};
      break;
    case RIGHT:
      next = {next.first + 1, next.second};
      break;
    case LEFT:
      next = {next.first - 1, next.second};
      break;

    default:
      break;
  }

  if (!validMove(next)) {  //撞到墙 or 障碍
    emit gameOver();
  } else if (next != bug->getPs() &&
             afterEating_ == 0) {  //没有吃到事物，且并非进食后片刻
    body_.push_front(next);
    body_.pop_back();
  } else {  //吃到了事物，或者刚刚进食
    body_.push_front(next);
    ++afterEating_;
    if (afterEating_ == 3) afterEating_ = 0;
  }

  if (next == bug->getPs()) {  //生成新食物
    do {
      bug->generateRandomPs();
    } while (body_.contains(bug->getPs()) || obstacles_.contains(bug->getPs()));
  }
}

bool Snake::validMove(const POS& next) {
  // qDebug() << "next is"
  //          << "(" << next.first << ", " << next.second << ")\n";

  if (body_.contains(next) || obstacles_.contains(next)) return false;

  if (next.first < 0 || next.first >= 40 || next.second < 0 ||
      next.second >= 40)
    return false;

  return true;
}

void Snake::dirUP() {
  qDebug() << "UP";
  if (dir_ == UP || dir_ == DOWN) return;
  dir_ = UP;
}
void Snake::dirDOWN() {
  qDebug() << "DOWN";
  if (dir_ == UP || dir_ == DOWN) return;
  dir_ = DOWN;
}
void Snake::dirRIGHT() {
  qDebug() << "RIGHT";
  if (dir_ == RIGHT || dir_ == LEFT) return;
  dir_ = RIGHT;
}
void Snake::dirLEFT() {
  qDebug() << "LEFT";
  if (dir_ == RIGHT || dir_ == LEFT) return;
  dir_ = LEFT;
}

void Snake::mousePressEvent(QMouseEvent* event) {
  if (!timeToToggleObstacles) return;
  if (event->button() == Qt::LeftButton) {
    int i = event->x() / 28;
    int j = event->y() / 28;
    if (i < 0 || i >= 40 || j < 0 || j >= 40) return;
    toggleObstacle(i, j);
  }
}

void Snake::toggleObstacle(int i, int j) {
  POS obs{i, j};
  if (obstacles_.contains(obs)) {
    obstacles_.removeOne(obs);
  } else {
    obstacles_.append(obs);
  }
  update();
}

void Snake::load(QJsonArray body, QJsonArray obstacles, QJsonArray rbug,
                 DIR dir, int time) {
  // Load body

  body_ = {};
  for (auto grid : body) {
    int i = grid.toArray().at(0).toInt();
    int j = grid.toArray().at(1).toInt();
    body_.append({i, j});
  }

  // Load obstacles

  obstacles_ = {};
  for (auto obs : obstacles) {
    int i = obs.toArray().at(0).toInt();
    int j = obs.toArray().at(1).toInt();
    obstacles_.push_front({i, j});
  }

  // Load bugs

  bug->ps = {rbug.toVariantList().at(0).toInt(),
             rbug.toVariantList().at(1).toInt()};

  // Load direction

  dir_ = dir;

  // Load time

  time_ = time;

  update();
}
