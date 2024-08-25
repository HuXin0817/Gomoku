#pragma once

#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <utility>

#include "board.h"
#include "common.h"

class Sensor final : public QWidget
{
public:
    Sensor(QWidget *parent, Board *nowBoard, int x, int y, std::unique_ptr<Sensor> (*widgets)[CHESS_NUMBER]);

    void flashing(double midValue, int duration);

    void handlePress();

    void unmark();

    static bool handledGameOver;
    static std::pair<int, int> lastPoint;

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
    bool markBox = false;
    ChessPlayer pressedPlayer = ChessPlayer::NONE;
    Board *nowBoard;
    std::unique_ptr<Sensor> (*widgets)[CHESS_NUMBER];
    QGraphicsOpacityEffect opacityEffect;
    QPropertyAnimation animation;
};
