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
    Sensor(QWidget *parent, Board *nowBoard, int x, int y, std::unique_ptr<Sensor> (*widgets)[CHESS_NUMBER])
        : QWidget(parent), x(x), y(y), nowBoard(nowBoard), widgets(widgets)
    {
        opacityEffect = std::make_unique<QGraphicsOpacityEffect>(this);
        setGraphicsEffect(opacityEffect.get());
    }

    void flashing(double midValue, int duration);

    void stopFlashing() { animation->stop(); }

    void unmark();

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    int x;
    int y;
    static bool handledGameOver;
    static std::pair<int, int> lastPoint;
    bool isMouseOn = false;
    bool isPressed = false;
    bool markBox = false;
    ChessPlayer pressedPlayer = ChessPlayer::NONE;
    Board *nowBoard;
    std::unique_ptr<QPropertyAnimation> animation;
    std::unique_ptr<Sensor> (*widgets)[CHESS_NUMBER];
    std::unique_ptr<QGraphicsOpacityEffect> opacityEffect;
};
