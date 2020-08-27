#include "snake.h"

#include <QDebug>
#include <QPainter>
#include <QTextStream>
#include <QTimer>

Snake::Snake(QWidget* parent) : QWidget(parent) {
  body.push_front({19, 19});
  body.push_front({20, 19});
  dir = RIGHT;
  afterEating = 0;
  obstacles = {};
  timeToToggleObstacles = true;

  bug = new Bug(body, obstacles, this);

  timer = new QTimer(this);
  // timer->start(80);
  connect(timer, SIGNAL(timeout()), this, SLOT(oneMove()));
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

Snake::~Snake() {}

void Snake::restart() {
  // Code of shit. Is there a better approach?
  body = {};
  body.push_front({19, 19});
  body.push_front({20, 19});
  dir = RIGHT;
  afterEating = 0;
  obstacles = {};
  timeToToggleObstacles = true;

  delete bug;
  bug = new Bug(body, obstacles, this);
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
  for (POS grid : body) {
    painter->fillRect(5 + 28 * grid.first, 5 + 28 * grid.second, 26, 26,
                      Qt::green);
  }

  // Draw the obstacles
  for (POS obs : obstacles) {
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

  POS next = body.front();
  switch (dir) {
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
             afterEating == 0) {  //没有吃到事物，且并非进食后片刻
    body.push_front(next);
    body.pop_back();
  } else {  //吃到了事物，或者刚刚进食
    body.push_front(next);
    ++afterEating;
    if (afterEating == 3) afterEating = 0;
  }

  if (next == bug->getPs()) {  //生成新食物
    do {
      bug->generateRandomPs();
    } while (body.contains(bug->getPs()) || obstacles.contains(bug->getPs()));
  }
}

bool Snake::validMove(const POS& next) {
  // qDebug() << "next is"
  //          << "(" << next.first << ", " << next.second << ")\n";

  if (body.contains(next) || obstacles.contains(next)) return false;

  if (next.first < 0 || next.first >= 40 || next.second < 0 ||
      next.second >= 40)
    return false;

  return true;
}

void Snake::dirUP() {
  qDebug() << "UP";
  if (dir == UP || dir == DOWN) return;
  dir = UP;
}
void Snake::dirDOWN() {
  qDebug() << "DOWN";
  if (dir == UP || dir == DOWN) return;
  dir = DOWN;
}
void Snake::dirRIGHT() {
  qDebug() << "RIGHT";
  if (dir == RIGHT || dir == LEFT) return;
  dir = RIGHT;
}
void Snake::dirLEFT() {
  qDebug() << "LEFT";
  if (dir == RIGHT || dir == LEFT) return;
  dir = LEFT;
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
  if (obstacles.contains(obs)) {
    obstacles.removeOne(obs);
  } else {
    obstacles.append(obs);
  }
  update();
}