#pragma once

#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "board.h"
#include "common.h"

class Sensor;

using board_sensors = std::vector<std::vector<std::unique_ptr<Sensor>>>;

class Sensor final : public QWidget
{
    Q_OBJECT

public:
    Sensor(QWidget *parent, Board *nowBoard, int x, int y, board_sensors *widgets);

    void flashing(double midValue, int duration);

    void stopFlashing() { animation.stop(); }

    void press();

    void clear();

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    int x;
    int y;
    bool isMouseOn = false;
    bool isPressed = false;
    ChessPlayer pressedPlayer = ChessPlayer::NONE;
    Board *nowBoard;
    board_sensors *widgets;
    QGraphicsOpacityEffect opacityEffect;
    QPropertyAnimation animation;
};
