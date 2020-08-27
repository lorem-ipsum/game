#ifndef SNAKE_H
#define SNAKE_H

#include <QList>
#include <QMouseEvent>
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

  void restart();  // Code of shit. Is there a better approach?

  bool validMove(const POS& next);

  QTimer* timer;

  bool timeToToggleObstacles;

 protected:
  void paintEvent(QPaintEvent* event);

  void mousePressEvent(QMouseEvent* event);

  void toggleObstacle(int i, int j);

 private:
  QList<POS> body;
  QList<POS> obstacles;
  DIR dir;
  Bug* bug;
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