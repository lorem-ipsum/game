#include "snake.h"

#include <QDebug>
#include <QPainter>
#include <QTextStream>
#include <QTimer>

Snake::Snake(QWidget* parent) : QWidget(parent) {
  grids.push_front({19, 19});
  grids.push_front({20, 19});
  dir = RIGHT;
  afterEating = 0;

  bug = new Bug(this);

  timer = new QTimer(this);
  // timer->start(80);
  connect(timer, SIGNAL(timeout()), this, SLOT(oneMove()));
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

Snake::~Snake() {}

void Snake::restart() {
  // Code of shit. Is there a better approach?
  grids = {};
  grids.push_front({19, 19});
  grids.push_front({20, 19});
  dir = RIGHT;
  afterEating = 0;
  delete bug;
  bug = new Bug(this);
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
  for (POS body : grids) {
    painter->fillRect(5 + 16 * body.first, 5 + 16 * body.second, 14, 14,
                      Qt::gray);
  }

  // Draw the bug
  painter->fillRect(5 + 16 * bug->getPs().first, 5 + 16 * bug->getPs().second,
                    14, 14, Qt::red);

  painter->end();
}

void Snake::oneMove() {
  // qDebug() << "oneMove!";

  POS next = grids.front();
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

  if (!validMove(next)) {  //撞到墙，之后还要考虑障碍 TODO
    emit gameOver();
  } else if (next != bug->getPs() &&
             afterEating == 0) {  //没有吃到事物，且并非进食后片刻
    grids.push_front(next);
    grids.pop_back();
  } else {  //吃到了事物，或者刚刚进食
    grids.push_front(next);
    ++afterEating;
    if (afterEating == 3) afterEating = 0;
  }

  if (next == bug->getPs()) {  //生成新食物
    do {
      bug->generateRandomPs();
    } while (grids.contains(bug->getPs()));
  }
}

bool Snake::validMove(const POS& next) {
  // qDebug() << "next is"
  //          << "(" << next.first << ", " << next.second << ")\n";

  if (grids.contains(next)) return false;

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
