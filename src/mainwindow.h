#pragma once

#include <QMainWindow>
#include <QMouseEvent>
#include <QParallelAnimationGroup>
#include <QShortcut>

#include "sensor.h"

class MainWindow final : public QMainWindow {
  Q_OBJECT

  public:
  explicit MainWindow(QWidget* parent = nullptr);

  protected:
  void
  paintEvent(QPaintEvent* event) override;

  void
  resizeEvent(QResizeEvent* event) override;

  private:
  std::unique_ptr<Board> board;
  std::vector<std::vector<std::unique_ptr<Sensor>>> sensors;

  void
  reload(const std::vector<Point>& moveRecord = {});

  void
  reloadSize();

  std::vector<Point>
  centerPieces() const;

  static double
  getMinWindowSize();

  void
  handleResizeEvent();

  private slots:

  void
  undo();

  void
  addBoardSize();

  void
  reduceBoardSize();

  void
  addChessNumber();

  void
  reduceChessNumber();

  void
  restart();
};
