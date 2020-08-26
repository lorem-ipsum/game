#include "snake.h"

#include <QDebug>
#include <QPainter>
#include <QTextStream>
#include <QTimer>

Snake::Snake(QWidget* parent) : QWidget(parent) {
  grids.push_front({19, 19});
  grids.push_front({20, 19});
  dir = RIGHT;

  QTimer* timer = new QTimer(this);
  timer->start(80);
  connect(timer, SIGNAL(timeout()), this, SLOT(oneMove()));
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

Snake::~Snake() {}

void Snake::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter* painter = new QPainter(this);
  painter->setPen(Qt::NoPen);
  for (POS body : grids) {
    painter->fillRect(5 + 16 * body.first, 5 + 16 * body.second, 14, 14,
                      Qt::gray);
  }
  painter->end();
}

void Snake::oneMove() {
  qDebug() << "oneMove!";

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

  if (!validMove(next)) {
    emit gameOver();
  } else {
    // TODO
    // 前进一格，要考虑食物

    qDebug() << "ELSE\n";

    grids.push_front(next);
    grids.pop_back();
  }
}

bool Snake::validMove(const POS& next) {
  qDebug() << "next is"
           << "(" << next.first << ", " << next.second << ")\n";

  if (grids.contains(next)) return false;

  if (next.first < 0 || next.first >= 40 || next.second < 0 ||
      next.second >= 40)
    return false;

  return true;
}