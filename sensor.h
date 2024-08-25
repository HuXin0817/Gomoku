#pragma once

#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QPushButton>

#include "board.h"
#include "common.h"

class Sensor final : public QWidget
{
public:
    Sensor(QWidget *parent, Board *nowBoard, int x, int y) : QWidget(parent), x(x), y(y), nowBoard(nowBoard) {}

    void flashing();

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    int x, y;
    bool isMouseOn = false;
    bool isPressed = false;
    ChessPlayer pressedPlayer = ChessPlayer::NONE;
    Board *nowBoard;
    std::unique_ptr<QPropertyAnimation> animation;
};
