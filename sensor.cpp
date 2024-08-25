#include "sensor.h"

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
        painter.setOpacity(0.7);
        player = nowBoard->getNowPlayer();
    }
    if (isPressed)
    {
        player = pressedPlayer;
    }
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint point(BOARD_PIECE_SPACING / 2, BOARD_PIECE_SPACING / 2);
    QPoint shadowPoint(point.x() + 1, point.y() + 1);
    painter.setPen(QPen(LineColor, 0));

    switch (player)
    {
    case ChessPlayer::NONE:
        break;
    case ChessPlayer::BLACK:
    {
        painter.setBrush(LineColor);
        painter.drawEllipse(shadowPoint, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
        QRadialGradient gradient(point.x(), point.y(), BOARD_PIECE_WIDTH);
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
        QRadialGradient gradient(point.x(), point.y(), BOARD_PIECE_WIDTH);
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
    update();
}

void Sensor::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (isPressed)
    {
        return;
    }
    if (!nowBoard->judgeIsPos(x, y))
    {
        return;
    }
    isPressed = true;
    pressedPlayer = nowBoard->getNowPlayer();
    nowBoard->addPiece(x, y);
}

void Sensor::flashing()
{
    auto *opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(opacityEffect);
    animation = std::make_unique<QPropertyAnimation>(opacityEffect, "opacity", this);
    animation->setDuration(1700);
    animation->setKeyValueAt(0.0, 1.0);
    animation->setKeyValueAt(0.5, 0.3);
    animation->setKeyValueAt(1.0, 1.0);

    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setLoopCount(-1);
    animation->start();
}
