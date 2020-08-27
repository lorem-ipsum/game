#ifndef SNAKE_H
#define SNAKE_H

#include <QJsonArray>
#include <QLabel>
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

  QList<POS> getBody() const { return body_; }
  QList<POS> getObstacles() const { return obstacles_; }
  DIR getDir() const { return dir_; }
  int getTime() const { return time_; }
  POS getBug() const { return bug->getPs(); }

  void restart();  // Code of shit. Is there a better approach?

  bool validMove(const POS& next);

  void load(QJsonArray body, QJsonArray obstacles, QJsonArray bug, DIR dir,
            int time);  // TODO

  QTimer* timer;

  bool timeToToggleObstacles;

 protected:
  void paintEvent(QPaintEvent* event);

  void mousePressEvent(QMouseEvent* event);

  void toggleObstacle(int i, int j);

 private:
  QList<POS> body_;
  QList<POS> obstacles_;
  DIR dir_;
  int time_;
  Bug* bug;
  int afterEating_;

 public slots:
  void oneMove();

  void dirUP();
  void dirDOWN();
  void dirRIGHT();
  void dirLEFT();

 signals:
  void gameOver();
  void timeFlies(int);
};

#endif  // SNAKE_H