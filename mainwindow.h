#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QKeyEvent>
#include <QMainWindow>

#include "background.h"
#include "snake.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void keyPressEvent(QKeyEvent *event) {
    if (event->type() == QEvent::KeyPress) {
      QKeyEvent *keyevent = static_cast<QKeyEvent *>(event);
      if (keyevent->key() == Qt::Key_Up) {
        emit UP_pressed();
      }
      if (keyevent->key() == Qt::Key_Down) {
        emit DOWN_pressed();
      }
      if (keyevent->key() == Qt::Key_Right) {
        emit RIGHT_pressed();
      }
      if (keyevent->key() == Qt::Key_Left) {
        emit LEFT_pressed();
      }
    }
  }

 private:
  Background *bg;
  Snake *snake;

 signals:
  // Move around
  void UP_pressed();
  void DOWN_pressed();
  void RIGHT_pressed();
  void LEFT_pressed();

  //  public slots:
  //   void newBug();
};
#endif  // MAINWINDOW_H
