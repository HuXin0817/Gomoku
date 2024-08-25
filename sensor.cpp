#include "sensor.h"

bool Sensor::handledGameOver;

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
        painter.setOpacity(0.9);
        player = nowBoard->getNowPlayer();
    }
    if (isPressed)
    {
        player = pressedPlayer;
    }
    painter.setRenderHint(QPainter::Antialiasing);

    QPointF point(BOARD_PIECE_SPACING / 2, BOARD_PIECE_SPACING / 2);
    QPointF shadowPoint(point.x() + 1, point.y() + 1);
    painter.setPen(QPen(LineColor, 0));

    switch (player)
    {
    case ChessPlayer::NONE:
        break;
    case ChessPlayer::BLACK:
    {
        painter.setBrush(LineColor);
        painter.drawEllipse(shadowPoint, BOARD_PIECE_WIDTH(), BOARD_PIECE_WIDTH());
        QRadialGradient gradient(point.x(), point.y(), BOARD_PIECE_WIDTH());
        gradient.setColorAt(0, BlackMidPieceColor);
        gradient.setColorAt(1, BlackFringePieceColor);
        painter.setBrush(gradient);
        painter.drawEllipse(point, BOARD_PIECE_WIDTH(), BOARD_PIECE_WIDTH());
        break;
    }
    case ChessPlayer::WRITE:
    {
        painter.setBrush(LineColor);
        painter.drawEllipse(shadowPoint, BOARD_PIECE_WIDTH(), BOARD_PIECE_WIDTH());
        QRadialGradient gradient(point.x(), point.y(), BOARD_PIECE_WIDTH());
        gradient.setColorAt(0, WriteMidPieceColor);
        gradient.setColorAt(1, WriteFringePieceColor);
        painter.setPen(QPen(WriteEdgePieceColor, 0));
        painter.setBrush(gradient);
        painter.drawEllipse(point, BOARD_PIECE_WIDTH(), BOARD_PIECE_WIDTH());
        painter.setPen(QPen(LineColor, 0));
        break;
    }
    }
    if (markBox)
    {
        painter.setBrush(LineColor);
        painter.drawEllipse(point, BOARD_STAR_POINT_WIDTH(), BOARD_STAR_POINT_WIDTH());
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
    flashing(0.7, 2500);
    update();
}

void Sensor::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    isMouseOn = false;
    if (!nowBoard->isGameOver())
    {
        animation.stop();
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

void Sensor::unmark()
{
    markBox = false;
    update();
}

void Sensor::press()
{
    opacityEffect.setOpacity(1.0);
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
    if (!nowBoard->getMoveRecords().empty())
    {
        auto &lastPoint = nowBoard->getMoveRecords().back();
        (*widgets)[lastPoint.first][lastPoint.second]->unmark();
    }
    nowBoard->addPiece(x, y);
    markBox = true;
    if (!handledGameOver && nowBoard->isGameOver())
    {
        handledGameOver = true;
        auto &lastPoint = nowBoard->getMoveRecords().back();
        (*widgets)[lastPoint.first][lastPoint.second]->unmark();
        auto pieces = nowBoard->winPieces();
        for (auto [px, py] : pieces)
        {
            (*widgets)[px][py]->flashing(0.3, 1500);
        }
    }
    update();
}
