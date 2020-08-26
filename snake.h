#ifndef SNAKE_H
#define SNAKE_H

#include <QList>
#include <QTimer>
#include <QWidget>
#include <utility>

#include "bug.h"

#ifndef POS
#define POS std::pair<int, int>
#endif

enum DIR { UP, DOWN, RIGHT, LEFT };

class Snake : public QWidget {
  Q_OBJECT

 public:
  explicit Snake(QWidget* parent);
  ~Snake();

  bool validMove(const POS& next);

 protected:
  void paintEvent(QPaintEvent* event);

 private:
  QList<POS> grids;
  DIR dir;
  Bug* bug;
  QTimer* timer;
  int afterEating;

 public slots:
  void oneMove();

  void dirUP();
  void dirDOWN();
  void dirRIGHT();
  void dirLEFT();

 signals:
  void gameOver();
};

#endif  // SNAKE_H