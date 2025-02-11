#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  board = std::make_unique<Board>();
  setWindowTitle("Gomoku");
  reload(board->getMoveRecords());

  connect(Shortcut("Ctrl+R"), &MainWindow::restart);
  connect(Shortcut("Ctrl+Z"), &MainWindow::undo);
  connect(Shortcut("Ctrl+Up"), &MainWindow::addBoardSize);
  connect(Shortcut("Ctrl+Down"), &MainWindow::reduceBoardSize);
  connect(Shortcut("Ctrl+Right"), &MainWindow::addChessNumber);
  connect(Shortcut("Ctrl+Left"), &MainWindow::reduceChessNumber);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
  QMainWindow::paintEvent(event);
  QPainter painter(this);
  painter.fillRect(rect(), BackGroundColor());
}

void MainWindow::undo()
{
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

void MainWindow::addBoardSize()
{
  auto OLD_BOARD_PIECE_SPACING = Config::BOARD_PIECE_SPACING;
  Config::BOARD_PIECE_SPACING *= 1.1;
  if (Config::BOARD_SIZE() > getMinWindowSize()) {
    Config::BOARD_PIECE_SPACING = OLD_BOARD_PIECE_SPACING;
    QApplication::beep();
    return;
  }
  reloadSize();
}

void MainWindow::reduceBoardSize()
{
  if (Config::BOARD_PIECE_SPACING / 1.1 > 25) {
    Config::BOARD_PIECE_SPACING /= 1.1;
    reloadSize();
  } else {
    QApplication::beep();
  }
}

void MainWindow::addChessNumber()
{
  Config::CHESS_NUMBER += 2;
  if (Config::BOARD_SIZE() > getMinWindowSize()) {
    Config::CHESS_NUMBER -= 2;
    QApplication::beep();
    return;
  }
  reload(centerPieces());
}

void MainWindow::reduceChessNumber()
{
  if (Config::CHESS_NUMBER - 2 > 0) {
    Config::CHESS_NUMBER -= 2;
    auto moves = centerPieces();
    for (auto [x, y] : moves) {
      if (x >= Config::CHESS_NUMBER || y >= Config::CHESS_NUMBER) {
        Config::CHESS_NUMBER += 2;
        QApplication::beep();
        return;
      }
    }
    reload(moves);
  } else {
    QApplication::beep();
  }
}

void MainWindow::reload(const std::vector<Point> &moveRecord)
{
  if (!sensors.empty()) {
    for (auto [x, y] : board->getMoveRecords()) {
      sensors[x][y]->clear();
    }
  }
  board->restart();
  reloadSize();
  for (auto [x, y] : moveRecord) {
    if (x < Config::CHESS_NUMBER && y < Config::CHESS_NUMBER) {
      sensors[x][y]->press();
    }
  }
  update();
}

void MainWindow::reloadSize()
{
  while (sensors.size() > Config::CHESS_NUMBER) {
    sensors.pop_back();
  }
  while (sensors.size() < Config::CHESS_NUMBER) {
    sensors.emplace_back();
  }
  for (int i = 0; i < Config::CHESS_NUMBER; i++) {
    while (sensors[i].size() > Config::CHESS_NUMBER) {
      sensors[i].pop_back();
    }
    while (sensors[i].size() < Config::CHESS_NUMBER) {
      int j = sensors[i].size();
      sensors[i].emplace_back(std::make_unique<Sensor>(this, board.get(), i, j, &sensors));
      sensors[i][j]->setGeometry(0, 0, 0, 0);
    }
  }
  if (!isFullScreen()) {
    QSize newSize(Config::BOARD_SIZE(), Config::BOARD_SIZE());

    auto *animationGroup = new QParallelAnimationGroup(this);
    auto *sizeAnimation  = new QPropertyAnimation(this, "size");
    sizeAnimation->setDuration(150);
    sizeAnimation->setEasingCurve(QEasingCurve::OutQuad);
    sizeAnimation->setStartValue(size());
    sizeAnimation->setEndValue(newSize);

    auto *minSizeAnimation = new QPropertyAnimation(this, "minimumSize");
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);
  handleResizeEvent();
}

std::vector<Point> MainWindow::centerPieces() const
{
  int  maxX        = 0;
  int  maxY        = 0;
  int  minX        = Config::CHESS_NUMBER;
  int  minY        = Config::CHESS_NUMBER;
  auto moveRecords = board->getMoveRecords();
  for (auto [x, y] : moveRecords) {
    maxX = std::max(x, maxX);
    maxY = std::max(y, maxY);
    minX = std::min(x, minX);
    minY = std::min(y, minY);
  }
  for (auto &[x, y] : moveRecords) {
    x = x + (Config::CHESS_NUMBER - maxX - minX) / 2;
    y = y + (Config::CHESS_NUMBER - maxY - minY) / 2;
  }
  return moveRecords;
}

double MainWindow::getMinWindowSize()
{
  auto *screen       = QGuiApplication::primaryScreen();
  auto  screenWidth  = screen->geometry().width();
  auto  screenHeight = screen->geometry().height();
  auto  minSize      = std::min(screenHeight, screenWidth);
  return minSize - 50;
}

void MainWindow::handleResizeEvent()
{
  double width  = size().width();
  double height = size().height();
  auto   midX   = width / 2;
  auto   midY   = height / 2;

  auto *animationGroup = new QParallelAnimationGroup(this);
  for (int i = 0; i < Config::CHESS_NUMBER; i++) {
    for (int j = 0; j < Config::CHESS_NUMBER; j++) {
      auto x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING - 0.5;
      auto y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING - 0.5;
      sensors[i][j]->setMouseOn(false);

      auto *posAnimation = new QPropertyAnimation(sensors[i][j].get(), "pos");
      posAnimation->setDuration(500);
      posAnimation->setStartValue(sensors[i][j]->pos());
      posAnimation->setEndValue(QPoint(x, y));
      posAnimation->setEasingCurve(QEasingCurve::OutQuad);
      animationGroup->addAnimation(posAnimation);

      auto  space         = Config::BOARD_PIECE_SPACING + 1;
      auto *sizeAnimation = new QPropertyAnimation(sensors[i][j].get(), "size");
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

void MainWindow::restart() { reload(); }
