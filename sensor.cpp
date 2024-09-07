#include "sensor.h"

Sensor::Sensor(QWidget *parent, Board *board, int x, int y, board_sensors *s) : QWidget(parent), x(x), y(y)
{
    nowBoard = board;
    sensors = s;
    opacityEffect = new QGraphicsOpacityEffect(this);
    opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
    setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(1);
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
    case ChessPlayer::WHITE:
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
    if (Config::AI_CHESS_PLAYER[nowBoard->getNowPlayer()])
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
    isMouseOn = true;
    update();
}

void Sensor::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if (Config::AI_CHESS_PLAYER[nowBoard->getNowPlayer()])
    {
        return;
    }
    isMouseOn = false;
    update();
}

void Sensor::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (Config::AI_CHESS_PLAYER[nowBoard->getNowPlayer()])
    {
        QApplication::beep();
        return;
    }
    press();
}

void Sensor::flashing()
{
    opacityAnimation->setDuration(1500);
    opacityAnimation->setKeyValueAt(0.0, 1.0);
    opacityAnimation->setKeyValueAt(0.5, 0.4);
    opacityAnimation->setKeyValueAt(1.0, 1.0);
    opacityAnimation->setEasingCurve(QEasingCurve::Linear);
    opacityAnimation->setLoopCount(-1);
    opacityAnimation->start();
}

void Sensor::stopFlashing()
{
    opacityAnimation->stop();
    opacityEffect->setOpacity(1);
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
    isPressed = true;
    isMouseOn = false;
    pressedPlayer = nowBoard->getNowPlayer();
    nowBoard->addPiece(x, y);
    if (nowBoard->isGameOver())
    {
        auto pieces = nowBoard->winPieces();
        for (auto [px, py] : pieces)
        {
            (*sensors)[px][py]->flashing();
        }
    }
    update();
    std::thread th(&Sensor::notifyAI, this);
    th.detach();
}

void Sensor::clear()
{
    isMouseOn = false;
    isPressed = false;
    pressedPlayer = ChessPlayer::NONE;
    opacityAnimation->stop();
    opacityEffect->setOpacity(1);
    update();
}

void Sensor::drawShadowPoint(QPainter &painter)
{
    if (IS_DARK_THEME())
    {
        return;
    }
    double BOARD_PIECE_SPACING = width();
    auto shadowPointPos = BOARD_PIECE_SPACING / 2 * 1.05;
    QPointF point(shadowPointPos, shadowPointPos);
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
    double w = width();
    double h = height();
    QPointF point(w / 2, h / 2);
    painter.drawEllipse(point, Config::BOARD_STAR_POINT_WIDTH(), Config::BOARD_STAR_POINT_WIDTH());
}

void Sensor::drawRightLine(QPainter &painter)
{
    if (x + 1 == Config::CHESS_NUMBER)
    {
        return;
    }
    double w = width();
    double h = height();
    QPointF point1(w, h / 2);
    QPointF point2(w / 2, h / 2);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.drawLine(point1, point2);
}

void Sensor::drawLeftLine(QPainter &painter)
{
    if (x == 0)
    {
        return;
    }
    double w = width();
    double h = height();
    QPointF point1(0, h / 2);
    QPointF point2(w / 2, h / 2);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.drawLine(point1, point2);
}

void Sensor::drawDownLine(QPainter &painter)
{
    if (y + 1 == Config::CHESS_NUMBER)
    {
        return;
    }
    double w = width();
    double h = height();
    QPointF point1(w / 2, h);
    QPointF point2(w / 2, h / 2);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.drawLine(point1, point2);
}

void Sensor::drawUpLine(QPainter &painter)
{
    if (y == 0)
    {
        return;
    }
    double w = width();
    double h = height();
    QPointF point1(w / 2, 0);
    QPointF point2(w / 2, h / 2);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    painter.drawLine(point1, point2);
}

void Sensor::notifyAI()
{
    if (Config::AI_CHESS_PLAYER[nowBoard->getNowPlayer()])
    {
        auto nextPoint = nowBoard->getBestPoint();
        (*sensors)[nextPoint.x][nextPoint.y]->press();
    }
}
