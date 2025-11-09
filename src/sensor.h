#pragma once

#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPropertyAnimation>
#include <QWidget>

#include "board.h"

class Sensor;

class Sensor final : public QWidget {
  Q_OBJECT

  public:
  Sensor(QWidget* parent,
         Board* board,
         int x,
         int y,
         std::vector<std::vector<std::unique_ptr<Sensor>>>* s);

  void
  flashing();

  void
  stopFlashing();

  void
  setMouseOn(bool m) {
    isMouseOn = m;
  }

  void
  press();

  void
  clear();

  protected:
  void
  enterEvent(QEnterEvent* event) override;

  void
  leaveEvent(QEvent* event) override;

  void
  paintEvent(QPaintEvent* event) override;

  void
  mousePressEvent(QMouseEvent* event) override;

  private:
  Point point;
  bool isMouseOn = false;
  bool isPressed = false;
  ChessPlayer pressedPlayer = ChessPlayer::None;
  Board* nowBoard = nullptr;
  std::vector<std::vector<std::unique_ptr<Sensor>>>* sensors = nullptr;
  QGraphicsOpacityEffect* opacityEffect = nullptr;
  QPropertyAnimation* opacityAnimation = nullptr;

  void
  drawShadowPoint(QPainter& painter);

  void
  drawUpLine(QPainter& painter);

  void
  drawDownLine(QPainter& painter);

  void
  drawLeftLine(QPainter& painter);

  void
  drawRightLine(QPainter& painter);

  void
  drawStarPoint(QPainter& painter);
};
