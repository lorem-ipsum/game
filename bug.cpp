#include "bug.h"

Bug::Bug(QList<POS> body, QList<POS> obstacles, QWidget* parent)
    : QWidget(parent) {
  do {
    generateRandomPs();
  } while (body.contains(ps) || obstacles.contains(ps));
}

Bug::~Bug() {}

void Bug::generateRandomPs() {
  QTime time = QTime::currentTime();
  qsrand(time.msec() + time.second() * 1000);
  int i = qrand() % 40;
  int j = qrand() % 40;
  ps = {i, j};
}

// void Bug::paintEvent(QPaintEvent* event) {
//   Q_UNUSED(event);

//   QPainter* painter = new QPainter(this);
//   painter->setPen(Qt::NoPen);

//   // qDebug() << "ps:(" << ps.first << "," << ps.second << ")";

//   painter->fillRect(5 + 16 * ps.first, 5 + 16 * ps.second, 14, 14, Qt::red);
//   painter->end();
// }

POS Bug::getPs() const { return ps; }
