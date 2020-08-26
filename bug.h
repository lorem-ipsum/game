#ifndef BUG_H
#define BUG_H

#include <QDebug>
#include <QPainter>
#include <QTime>
#include <QWidget>
#include <utility>

#ifndef POS
#define POS std::pair<int, int>
#endif

class Bug : public QWidget {
 public:
  explicit Bug(QWidget* parent = nullptr);
  ~Bug();

  void generateRandomPs();
  // void paintEvent(QPaintEvent* event);

  POS getPs() const;

 private:
  POS ps;
};

#endif  // BUG_H