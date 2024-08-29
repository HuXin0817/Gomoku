#include "sensor.h"

Sensor::Sensor(QWidget *parent, Board *nowBoard, int x, int y, board_sensors *widgets)
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
        painter.setOpacity(0.7);
    }
    if (isPressed)
    {
        player = pressedPlayer;
    }
    painter.setRenderHint(QPainter::Antialiasing);

    double BOARD_PIECE_SPACING = width();
    QPointF point(BOARD_PIECE_SPACING / 2, BOARD_PIECE_SPACING / 2);

    auto BOARD_PIECE_WIDTH = Config::BOARD_PIECE_WIDTH(BOARD_PIECE_SPACING);
    switch (player)
    {
    case ChessPlayer::NONE:
        break;
    case ChessPlayer::BLACK:
    {
        drawShadowPoint(painter);
        QRadialGradient gradient(point, BOARD_PIECE_WIDTH);
        gradient.setColorAt(0, BlackMidPieceColor());
        gradient.setColorAt(1, BlackFringePieceColor());
        painter.setBrush(gradient);
        painter.setPen(QPen(BlackFringePieceColor(), 0));
        painter.drawEllipse(point, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
        break;
    }
    case ChessPlayer::WRITE:
    {
        drawShadowPoint(painter);
        QRadialGradient gradient(point, BOARD_PIECE_WIDTH);
        gradient.setColorAt(0, WriteMidPieceColor());
        gradient.setColorAt(1, WriteFringePieceColor());
        painter.setBrush(gradient);
        painter.setPen(QPen(WriteEdgePieceColor(), 0));
        painter.drawEllipse(point, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
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
    update();
}

void Sensor::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    press();
}

void Sensor::flashing()
{
    animation.setDuration(1500);
    animation.setKeyValueAt(0.0, 1.0);
    animation.setKeyValueAt(0.5, 0.4);
    animation.setKeyValueAt(1.0, 1.0);
    animation.setEasingCurve(QEasingCurve::Linear);
    animation.setLoopCount(-1);
    animation.start();
}

void Sensor::stopFlashing()
{
    animation.stop();
    opacityEffect.setOpacity(1);
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
    isMouseOn = false;
    opacityEffect.setOpacity(1);
    pressedPlayer = nowBoard->getNowPlayer();
    nowBoard->addPiece(x, y);
    if (nowBoard->isGameOver())
    {
        auto pieces = nowBoard->winPieces();
        for (auto [px, py] : pieces)
        {
            (*widgets)[px][py]->flashing();
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

void Sensor::drawShadowPoint(QPainter &painter)
{
    if (IS_DARK_THEME())
    {
        return;
    }
    double BOARD_PIECE_SPACING = width();
    double pos = BOARD_PIECE_SPACING / 2 * 1.05;
    QPointF point(pos, pos);
    auto BOARD_PIECE_WIDTH = Config::BOARD_PIECE_WIDTH(BOARD_PIECE_SPACING);
    painter.setPen(QPen(LineColor, 0));
    painter.setBrush(LineColor);
    painter.drawEllipse(point, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
}
