#include "sensor.h"

std::pair<int, int> Sensor::lastPoint(-1, -1);
bool Sensor::handledGameOver = false;

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
    if (markBox)
    {
        painter.setBrush(LineColor);
        painter.drawEllipse(point, BOARD_STAR_POINT_WIDTH, BOARD_STAR_POINT_WIDTH);
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
        stopFlashing();
        opacityEffect->setOpacity(1);
    }
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
    if (lastPoint.first != -1 && lastPoint.second != -1)
    {
        widgets[lastPoint.first][lastPoint.second]->unmark();
    }
    lastPoint = {x, y};
    markBox = true;
    if (!handledGameOver && nowBoard->isGameOver())
    {
        handledGameOver = true;
        widgets[lastPoint.first][lastPoint.second]->unmark();
        auto pieces = nowBoard->winPieces();
        for (auto [px, py] : pieces)
        {
            widgets[px][py]->flashing(0.3, 1500);
        }
    }
    update();
}

void Sensor::flashing(double midValue, int duration)
{
    animation = std::make_unique<QPropertyAnimation>(opacityEffect.get(), "opacity", this);
    animation->setDuration(duration);
    animation->setKeyValueAt(0.0, 1.0);
    animation->setKeyValueAt(0.5, midValue);
    animation->setKeyValueAt(1.0, 1.0);

    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setLoopCount(-1);
    animation->start();
}

void Sensor::unmark()
{
    markBox = false;
    update();
}
