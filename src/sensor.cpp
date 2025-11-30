#include "sensor.h"

Sensor::Sensor(QWidget* parent,
               Board* board,
               int x,
               int y,
               std::vector<std::vector<std::unique_ptr<Sensor>>>* s)
    : QWidget(parent), point(x, y) {
  nowBoard = board;
  sensors = s;
  opacityEffect = new QGraphicsOpacityEffect(this);
  opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
  setGraphicsEffect(opacityEffect);
  opacityEffect->setOpacity(1);
}

void
Sensor::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  drawDownLine(painter);
  drawUpLine(painter);
  drawLeftLine(painter);
  drawRightLine(painter);
  drawStarPoint(painter);

  if (!isPressed && !isMouseOn) {
    return;
  }

  ChessPlayer player = ChessPlayer::None;
  if (isMouseOn) {
    player = nowBoard->getNowPlayer();
    painter.setOpacity(0.7);
  }
  if (isPressed) {
    player = pressedPlayer;
  }

  double BOARD_PIECE_SPACING = width();
  QPointF p(BOARD_PIECE_SPACING / 2, BOARD_PIECE_SPACING / 2);

  auto BOARD_PIECE_WIDTH = Config::BoardPieceWidth(BOARD_PIECE_SPACING);
  switch (player) {
    case ChessPlayer::None:
      break;
    case ChessPlayer::Black: {
      drawShadowPoint(painter);
      QRadialGradient gradient(p, BOARD_PIECE_WIDTH);
      gradient.setColorAt(0, BlackMidPieceColor());
      gradient.setColorAt(1, BlackFringePieceColor());
      painter.setBrush(gradient);
      painter.setPen(QPen(BlackFringePieceColor(), 0));
      painter.drawEllipse(p, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
      break;
    }
    case ChessPlayer::White: {
      drawShadowPoint(painter);
      QRadialGradient gradient(p, BOARD_PIECE_WIDTH);
      gradient.setColorAt(0, WriteMidPieceColor());
      gradient.setColorAt(1, WriteFringePieceColor());
      painter.setBrush(gradient);
      painter.setPen(QPen(WriteEdgePieceColor(), 0));
      painter.drawEllipse(p, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
      break;
    }
  }
}

void
Sensor::enterEvent(QEnterEvent* event) {
  QWidget::enterEvent(event);
  if (isPressed) {
    return;
  }
  if (!nowBoard->judgeIsPos(point.x, point.y)) {
    return;
  }
  isMouseOn = true;
  update();
}

void
Sensor::leaveEvent(QEvent* event) {
  QWidget::leaveEvent(event);
  isMouseOn = false;
  update();
}

void
Sensor::mousePressEvent(QMouseEvent* event) {
  QWidget::mousePressEvent(event);
  press();
}

void
Sensor::flashing() {
  opacityAnimation->setDuration(1500);
  opacityAnimation->setKeyValueAt(0.0, 1.0);
  opacityAnimation->setKeyValueAt(0.5, 0.4);
  opacityAnimation->setKeyValueAt(1.0, 1.0);
  opacityAnimation->setEasingCurve(QEasingCurve::Linear);
  opacityAnimation->setLoopCount(-1);
  opacityAnimation->start();
}

void
Sensor::stopFlashing() {
  opacityAnimation->stop();
  opacityEffect->setOpacity(1);
}

void
Sensor::press() {
  if (nowBoard->isGameOver()) {
    return;
  }
  if (isPressed) {
    return;
  }
  isPressed = true;
  isMouseOn = false;
  pressedPlayer = nowBoard->getNowPlayer();
  nowBoard->addPiece(point.x, point.y);
  if (nowBoard->isGameOver()) {
    auto pieces = nowBoard->winPieces();
    for (auto [px, py] : pieces) {
      (*sensors)[px][py]->flashing();
    }
  }
  update();
}

void
Sensor::clear() {
  isMouseOn = false;
  isPressed = false;
  pressedPlayer = ChessPlayer::None;
  opacityAnimation->stop();
  opacityEffect->setOpacity(1);
  update();
}

void
Sensor::drawShadowPoint(QPainter& painter) {
  if (IsDarkTheme()) {
    return;
  }
  double BOARD_PIECE_SPACING = width();
  auto shadowPointPos = BOARD_PIECE_SPACING / 2 * 1.05;
  QPointF p(shadowPointPos, shadowPointPos);
  auto BOARD_PIECE_WIDTH = Config::BoardPieceWidth(BOARD_PIECE_SPACING);
  painter.setPen(QPen(LineColor, 0));
  painter.setBrush(LineColor);
  painter.drawEllipse(p, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
}

void
Sensor::drawStarPoint(QPainter& painter) {
  int c = 0;
  for (auto p : Config::StarPositions()) {
    c += p == point.x;
    c += p == point.y;
  }
  if (c != 2) {
    return;
  }
  painter.setPen(QPen(LineColor, Config::BoardLineWidth()));
  painter.setBrush(LineColor);
  double w = width();
  double h = height();
  QPointF p(w / 2, h / 2);
  painter.drawEllipse(p, Config::BoardStarPointWidth(), Config::BoardStarPointWidth());
}

void
Sensor::drawRightLine(QPainter& painter) {
  if (point.x + 1 == Config::ChessNumber) {
    return;
  }
  double w = width();
  double h = height();
  QPointF point1(w, h / 2);
  QPointF point2(w / 2, h / 2);
  painter.setPen(QPen(LineColor, Config::BoardLineWidth()));
  painter.drawLine(point1, point2);
}

void
Sensor::drawLeftLine(QPainter& painter) {
  if (point.x == 0) {
    return;
  }
  double w = width();
  double h = height();
  QPointF point1(0, h / 2);
  QPointF point2(w / 2, h / 2);
  painter.setPen(QPen(LineColor, Config::BoardLineWidth()));
  painter.drawLine(point1, point2);
}

void
Sensor::drawDownLine(QPainter& painter) {
  if (point.y + 1 == Config::ChessNumber) {
    return;
  }
  double w = width();
  double h = height();
  QPointF point1(w / 2, h);
  QPointF point2(w / 2, h / 2);
  painter.setPen(QPen(LineColor, Config::BoardLineWidth()));
  painter.drawLine(point1, point2);
}

void
Sensor::drawUpLine(QPainter& painter) {
  if (point.y == 0) {
    return;
  }
  double w = width();
  double h = height();
  QPointF point1(w / 2, 0);
  QPointF point2(w / 2, h / 2);
  painter.setPen(QPen(LineColor, Config::BoardLineWidth()));
  painter.drawLine(point1, point2);
}
