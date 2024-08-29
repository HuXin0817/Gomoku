#include "sensor.h"

Sensor::Sensor(QWidget *parent, Board *nowBoard, int x, int y, std::vector<std::vector<std::unique_ptr<Sensor>>> *widgets)
    : QWidget(parent), x(x), y(y), nowBoard(nowBoard), widgets(widgets), opacityEffect(this), animation(&opacityEffect, "opacity", this)
{
    setGraphicsEffect(&opacityEffect);
}

void Sensor::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if (!isPressed && !isMouseOn)
    {
        return;
    }
    QPainter painter(this);
    ChessPlayer player;
    if (isMouseOn)
    {
        player = nowBoard->getNowPlayer();
        if (player == ChessPlayer::BLACK)
        {
            painter.setOpacity(0.6);
        }
        else
        {
            painter.setOpacity(0.7);
        }
    }
    if (isPressed)
    {
        player = pressedPlayer;
    }
    painter.setRenderHint(QPainter::Antialiasing);

    double BOARD_PIECE_SPACING = width();
    QPointF point(BOARD_PIECE_SPACING / 2, BOARD_PIECE_SPACING / 2);
    QPointF shadowPoint(point.x() * 1.05, point.y() * 1.05);
    painter.setPen(QPen(LineColor, 0));

    double BOARD_PIECE_WIDTH = Config::BOARD_PIECE_WIDTH(BOARD_PIECE_SPACING);
    switch (player)
    {
    case ChessPlayer::NONE:
        break;
    case ChessPlayer::BLACK:
    {
        painter.setBrush(LineColor);
        painter.drawEllipse(shadowPoint, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
        QRadialGradient gradient(point, BOARD_PIECE_WIDTH);
        gradient.setColorAt(0, BlackMidPieceColor);
        gradient.setColorAt(1, BlackFringePieceColor);
        painter.setBrush(gradient);
        painter.drawEllipse(point, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
        break;
    }
    case ChessPlayer::WRITE:
    {
        painter.setBrush(LineColor);
        painter.drawEllipse(shadowPoint, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
        QRadialGradient gradient(point, BOARD_PIECE_WIDTH);
        gradient.setColorAt(0, WriteMidPieceColor);
        gradient.setColorAt(1, WriteFringePieceColor);
        painter.setPen(QPen(WriteEdgePieceColor, 0));
        painter.setBrush(gradient);
        painter.drawEllipse(point, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
        painter.setPen(QPen(LineColor, 0));
        break;
    }
    }
}

void Sensor::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);
    if (isPressed)
    {
        return;
    }
    if (!nowBoard->judgeIsPos(x, y))
    {
        return;
    }
    isMouseOn = true;
    update();
}

void Sensor::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    isMouseOn = false;
    if (!nowBoard->isGameOver())
    {
        opacityEffect.setOpacity(1);
    }
    update();
}

void Sensor::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    press();
}

void Sensor::flashing(double midValue, int duration)
{
    animation.setDuration(duration);
    animation.setKeyValueAt(0.0, 1.0);
    animation.setKeyValueAt(0.5, midValue);
    animation.setKeyValueAt(1.0, 1.0);
    animation.setEasingCurve(QEasingCurve::Linear);
    animation.setLoopCount(-1);
    animation.start();
}

void Sensor::press()
{
    if (nowBoard->isGameOver())
    {
        return;
    }
    if (isPressed)
    {
        return;
    }
    if (!nowBoard->judgeIsPos(x, y))
    {
        return;
    }
    isPressed = true;
    for (auto &widget : *widgets)
    {
        for (auto &w : widget)
        {
            w->stopFlashing();
            w->opacityEffect.setOpacity(1);
            w->isMouseOn = false;
            w->update();
        }
    }
    pressedPlayer = nowBoard->getNowPlayer();
    nowBoard->addPiece(x, y);
    if (nowBoard->isGameOver())
    {
        std::vector<point> pieces = nowBoard->winPieces();
        for (auto [px, py] : pieces)
        {
            (*widgets)[px][py]->flashing(0.4, 1500);
        }
    }
    update();
}

void Sensor::clear()
{
    isMouseOn = false;
    isPressed = false;
    pressedPlayer = ChessPlayer::NONE;
    animation.stop();
    opacityEffect.setOpacity(1);
    update();
}
