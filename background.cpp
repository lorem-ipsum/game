#include "background.h"

#include <QPainter>

Background::Background(QWidget* parent) : QWidget(parent) {}
Background::~Background() {}

void Background::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  QPainter* painter = new QPainter(this);
  QPen* pen = new QPen(Qt::black, 2, Qt::SolidLine);
  painter->setPen(*pen);
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 40; ++j) {
      painter->drawRect(4 + 28 * i, 4 + 28 * j, 28, 28);
    }
  }
  painter->end();
}