#include "sensor.h"

Sensor::Sensor(QWidget *parent, Board *nowBoard, int x, int y, board_sensors *widgets)
    : QWidget(parent), x(x), y(y), nowBoard(nowBoard), widgets(widgets), opacityEffect(this), animation(&opacityEffect, "opacity", this)
{
    setGraphicsEffect(&opacityEffect);
    opacityEffect.setOpacity(1);
}

void Sensor::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawDownLine(painter);
    drawUpLine(painter);
    drawLeftLine(painter);
    drawRightLine(painter);
    drawStarPoint(painter);

    if (!isPressed && !isMouseOn)
    {
        return;
    }

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

void Sensor::drawStarPoint(QPainter &painter)
{
    int c = 0;
    for (auto p : Config::StarPositions())
    {
        c += p == x;
        c += p == y;
    }
    if (c != 2)
    {
        return;
    }

    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.setBrush(LineColor);
    double width = width();
    double height = height();
    QPointF point(width / 2, height / 2);
    painter.drawEllipse(point, Config::BOARD_STAR_POINT_WIDTH(), Config::BOARD_STAR_POINT_WIDTH());
}

void Sensor::drawRightLine(QPainter &painter)
{
    if (x + 1 == Config::CHESS_NUMBER)
    {
        return;
    }
    double width = width();
    double height = height();
    QPointF point1(width, height / 2);
    QPointF point2(width / 2, height / 2);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.drawLine(point1, point2);
}

void Sensor::drawLeftLine(QPainter &painter)
{
    if (x == 0)
    {
        return;
    }
    double width = width();
    double height = height();
    QPointF point1(0, height / 2);
    QPointF point2(width / 2, height / 2);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.drawLine(point1, point2);
}

void Sensor::drawDownLine(QPainter &painter)
{
    if (y + 1 == Config::CHESS_NUMBER)
    {
        return;
    }
    double width = width();
    double height = height();
    QPointF point1(width / 2, height);
    QPointF point2(width / 2, height / 2);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.drawLine(point1, point2);
}

void Sensor::drawUpLine(QPainter &painter)
{
    if (y == 0)
    {
        return;
    }
    double width = width();
    double height = height();
    QPointF point1(width / 2, 0);
    QPointF point2(width / 2, height / 2);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.drawLine(point1, point2);
}
