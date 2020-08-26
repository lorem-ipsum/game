#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>

class Background : public QWidget {
  Q_OBJECT
 public:
  Background(QWidget* parent = nullptr);
  ~Background();

 protected:
  void paintEvent(QPaintEvent* event);

 private:
};

#endif  // BACKGROUND_H