#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  board = std::make_unique<Board>();
  setWindowTitle("Gomoku");
  reload(board->getMoveRecords());

  connect(new QShortcut(QKeySequence("Ctrl+R"), this),
          &QShortcut::activated,
          this,
          &MainWindow::restart);
  connect(new QShortcut(QKeySequence("Ctrl+Z"), this),
          &QShortcut::activated,
          this,
          &MainWindow::undo);
  connect(new QShortcut(QKeySequence("Ctrl+Up"), this),
          &QShortcut::activated,
          this,
          &MainWindow::addBoardSize);
  connect(new QShortcut(QKeySequence("Ctrl+Down"), this),
          &QShortcut::activated,
          this,
          &MainWindow::reduceBoardSize);
  connect(new QShortcut(QKeySequence("Ctrl+Right"), this),
          &QShortcut::activated,
          this,
          &MainWindow::addChessNumber);
  connect(new QShortcut(QKeySequence("Ctrl+Left"), this),
          &QShortcut::activated,
          this,
          &MainWindow::reduceChessNumber);
}

void
MainWindow::paintEvent(QPaintEvent* event) {
  QMainWindow::paintEvent(event);
  QPainter painter(this);
  painter.fillRect(rect(), BackGroundColor());
}

void
MainWindow::undo() {
  if (board->getMoveRecords().empty()) {
    QApplication::beep();
    return;
  }
  if (board->isGameOver()) {
    auto pieces = board->winPieces();
    for (auto [x, y] : pieces) {
      sensors[x][y]->stopFlashing();
    }
  }
  auto back = board->undo();
  sensors[back.x][back.y]->clear();
}

void
MainWindow::addBoardSize() {
  auto OLD_BOARD_PIECE_SPACING = Config::BoardPieceSpacing;
  Config::BoardPieceSpacing *= 1.1;
  if (Config::BoardSize() > getMinWindowSize()) {
    Config::BoardPieceSpacing = OLD_BOARD_PIECE_SPACING;
    QApplication::beep();
    return;
  }
  reloadSize();
}

void
MainWindow::reduceBoardSize() {
  if (Config::BoardPieceSpacing / 1.1 > 25) {
    Config::BoardPieceSpacing /= 1.1;
    reloadSize();
  } else {
    QApplication::beep();
  }
}

void
MainWindow::addChessNumber() {
  Config::ChessNumber += 2;
  if (Config::BoardSize() > getMinWindowSize()) {
    Config::ChessNumber -= 2;
    QApplication::beep();
    return;
  }
  reload(centerPieces());
}

void
MainWindow::reduceChessNumber() {
  if (Config::ChessNumber - 2 > 0) {
    Config::ChessNumber -= 2;
    auto moves = centerPieces();
    for (auto [x, y] : moves) {
      if (x >= Config::ChessNumber || y >= Config::ChessNumber) {
        Config::ChessNumber += 2;
        QApplication::beep();
        return;
      }
    }
    reload(moves);
  } else {
    QApplication::beep();
  }
}

void
MainWindow::reload(const std::vector<Point>& moveRecord) {
  if (!sensors.empty()) {
    for (auto [x, y] : board->getMoveRecords()) {
      sensors[x][y]->clear();
    }
  }
  board->restart();
  reloadSize();
  for (auto [x, y] : moveRecord) {
    if (x < Config::ChessNumber && y < Config::ChessNumber) {
      sensors[x][y]->press();
    }
  }
  update();
}

void
MainWindow::reloadSize() {
  while (sensors.size() > Config::ChessNumber) {
    sensors.pop_back();
  }
  while (sensors.size() < Config::ChessNumber) {
    sensors.emplace_back();
  }
  for (int i = 0; i < Config::ChessNumber; i++) {
    while (sensors[i].size() > Config::ChessNumber) {
      sensors[i].pop_back();
    }
    while (sensors[i].size() < Config::ChessNumber) {
      int j = sensors[i].size();
      sensors[i].emplace_back(std::make_unique<Sensor>(this, board.get(), i, j, &sensors));
      sensors[i][j]->setGeometry(0, 0, 0, 0);
    }
  }
  if (!isFullScreen()) {
    QSize newSize(Config::BoardSize(), Config::BoardSize());

    auto* animationGroup = new QParallelAnimationGroup(this);
    auto* sizeAnimation = new QPropertyAnimation(this, "size");
    sizeAnimation->setDuration(150);
    sizeAnimation->setEasingCurve(QEasingCurve::OutQuad);
    sizeAnimation->setStartValue(size());
    sizeAnimation->setEndValue(newSize);

    auto* minSizeAnimation = new QPropertyAnimation(this, "minimumSize");
    minSizeAnimation->setDuration(150);
    minSizeAnimation->setEasingCurve(QEasingCurve::OutQuad);
    minSizeAnimation->setStartValue(minimumSize());
    minSizeAnimation->setEndValue(newSize);

    animationGroup->addAnimation(sizeAnimation);
    animationGroup->addAnimation(minSizeAnimation);

    animationGroup->start();
  }
  handleResizeEvent();
}

void
MainWindow::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  handleResizeEvent();
}

std::vector<Point>
MainWindow::centerPieces() const {
  int maxX = 0;
  int maxY = 0;
  int minX = Config::ChessNumber;
  int minY = Config::ChessNumber;
  auto moveRecords = board->getMoveRecords();
  for (auto [x, y] : moveRecords) {
    maxX = std::max(x, maxX);
    maxY = std::max(y, maxY);
    minX = std::min(x, minX);
    minY = std::min(y, minY);
  }
  for (auto& [x, y] : moveRecords) {
    x = x + (Config::ChessNumber - maxX - minX) / 2;
    y = y + (Config::ChessNumber - maxY - minY) / 2;
  }
  return moveRecords;
}

double
MainWindow::getMinWindowSize() {
  auto* screen = QGuiApplication::primaryScreen();
  auto screenWidth = screen->geometry().width();
  auto screenHeight = screen->geometry().height();
  auto minSize = std::min(screenHeight, screenWidth);
  return minSize - 50;
}

void
MainWindow::handleResizeEvent() {
  double width = size().width();
  double height = size().height();
  auto midX = width / 2;
  auto midY = height / 2;

  auto* animationGroup = new QParallelAnimationGroup(this);
  for (int i = 0; i < Config::ChessNumber; i++) {
    for (int j = 0; j < Config::ChessNumber; j++) {
      auto x = midX + (i - Config::ChessNumber / 2) * Config::BoardPieceSpacing - 0.5;
      auto y = midY + (j - Config::ChessNumber / 2) * Config::BoardPieceSpacing - 0.5;
      sensors[i][j]->setMouseOn(false);

      auto* posAnimation = new QPropertyAnimation(sensors[i][j].get(), "pos");
      posAnimation->setDuration(500);
      posAnimation->setStartValue(sensors[i][j]->pos());
      posAnimation->setEndValue(QPoint(x, y));
      posAnimation->setEasingCurve(QEasingCurve::OutQuad);
      animationGroup->addAnimation(posAnimation);

      auto space = Config::BoardPieceSpacing + 1;
      auto* sizeAnimation = new QPropertyAnimation(sensors[i][j].get(), "size");
      sizeAnimation->setDuration(500);
      sizeAnimation->setStartValue(sensors[i][j]->size());
      sizeAnimation->setEndValue(QSize(space, space));
      sizeAnimation->setEasingCurve(QEasingCurve::OutQuad);
      animationGroup->addAnimation(sizeAnimation);

      sensors[i][j]->show();
    }
  }

  animationGroup->start();
}

void
MainWindow::restart() {
  reload();
}
