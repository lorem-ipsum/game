#include <QList>
#include <QWidget>

typedef std::pair<int, int> POS;
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

 public slots:
  void oneMove();

 signals:
  void gameOver();
};