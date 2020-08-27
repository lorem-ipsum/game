# Qt贪吃蛇项目设计文档

## 大致框架

本程序在Ubuntu 20.04系统上编写，未使用Qt Designer，所有ui设计均通过代码实现。

游戏窗口由类`MainWindow`实现，它继承自类`QMainWindow`，因此可以引入菜单栏(Menubar)和工具栏(Toolbar)。`MainWindow`类有以下若干子元素：

- `bg`为类`Background`的实例，负责绘出40*40的地图。
- `snake`为类`Snake`的实例，负责存放地图上动态生成的一切元素，包括贪吃蛇的身体、随机出现的果实和用户自定义的障碍物。
- `label`为类`QLabel`的实例，负责显示游戏分数和游戏结束的信息。
- `cp`为类`QLabel`的实例，负责显示版权信息。
- `menubar`为类`QMenuBar`的实例，负责生成菜单栏。
- `toolbar`为类`QToolBar`的实例，负责生成工具栏
- `btns`为类`QButtonGroup`的实例，负责展示和维护开始游戏、暂停等操作按钮。

## 细节介绍

### `Background`类

```cpp
class Background : public QWidget {
  Q_OBJECT
 public:
  Background(QWidget* parent = nullptr);
  ~Background();

 protected:
  void paintEvent(QPaintEvent* event); // 绘制静态的地图

 private:
};
```

### `Bug`类

`Bug`类为`Snake`类的组成部分，提供了随机生成果实与展示果实位置的功能。

```cpp
#define POS std::pair<int, int>

class Bug : public QWidget {
 public:
  // 构造函数接受动态组件信息，生成果实时检查是否与其他动态组件重叠
  explicit Bug(QList<POS> body, QList<POS> obstacles,
               QWidget* parent = nullptr);
  ~Bug();

  void generateRandomPs();

  POS getPs() const;

  POS ps;
};
```

### `Snake`类

```cpp
#define POS std::pair<int, int> // 用POS表示单个格点的位置

enum DIR { UP, DOWN, RIGHT, LEFT }; // 用枚举类型DIR表示蛇的前进方向

class Snake : public QWidget {
  Q_OBJECT

 public:
  explicit Snake(QWidget* parent);
  ~Snake();

  QList<POS> getBody() const;
  QList<POS> getObstacles() const;
  DIR getDir() const;
  int getTime() const;
  POS getBug() const;
  void restart();

  bool validMove(const POS& next); // 判断前进的下一步是否会与障碍或自身相撞

  void load(QJsonArray body, QJsonArray obstacles, QJsonArray bug, DIR dir, int time); // 从文件读入当前状态

  QTimer* timer; // 用来计时

  bool timeToToggleObstacles; // 判断当前时刻用户是否可以自主添加障碍

 protected:
  void paintEvent(QPaintEvent* event); // 绘制一切动态元素，包括蛇身、果实和障碍

  void mousePressEvent(QMouseEvent* event); // 在添加障碍时使用

  void toggleObstacle(int i, int j); // 在添加障碍时使用

 private:
  QList<POS> body_;      // 蛇身
  QList<POS> obstacles_; // 障碍
  DIR dir_;              // 当前蛇的前进方向
  int time_;             // 当前时刻
  Bug* bug;              // 当前时刻果实的位置
  int afterEating_;      // 用于判断蛇是否在进食后的三步以内

 public slots:
  void oneMove();        // 蛇前进一步

  // 以下四个函数当键盘事件请求蛇转向时调用，会检查转向是否合法，若不合法则忽略请求
  void dirUP();
  void dirDOWN();
  void dirRIGHT();
  void dirLEFT();

 signals:
  void gameOver();     // 发出游戏结束的信号
  void timeFlies(int); // 发出时间前进的信号，参数即为当前时刻
};
```

### `MainWindow`类

```cpp
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void keyPressEvent(QKeyEvent *event); // 被调用时发送方向键按下的信号

 private:
  Background *bg;
  Snake *snake;

  void connectArrowKeysToSnake();
  void disConnectArrowKeysToSnake();

 signals:
  // 当按下方向键时触发以下4个信号
  void UP_pressed();
  void DOWN_pressed();
  void RIGHT_pressed();
  void LEFT_pressed();
};
```

所有除转向之外的逻辑信息均在`MainWindow`的构造函数中完成。
