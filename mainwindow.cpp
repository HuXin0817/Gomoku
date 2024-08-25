#include "mainwindow.h"

#include <utility>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(BOARD_SIZE, BOARD_SIZE);
    Sensor::lastPoint = std::pair<int, int>(-1, -1);
    Sensor::handledGameOver = false;

    for (int i = 0; i < CHESS_NUMBER; i++)
    {
        for (int j = 0; j < CHESS_NUMBER; j++)
        {
            if (i + 1 < CHESS_NUMBER)
            {
                lines.emplace_back(transPosition(i), transPosition(j), transPosition(i + 1), transPosition(j));
            }
            if (j + 1 < CHESS_NUMBER)
            {
                lines.emplace_back(transPosition(i), transPosition(j), transPosition(i), transPosition(j + 1));
            }
        }
    }

    for (int i : StarPositions)
    {
        for (int j : StarPositions)
        {
            starPoints.emplace_back(transPosition(i), transPosition(j));
        }
    }

    for (int i = 0; i < CHESS_NUMBER; i++)
    {
        for (int j = 0; j < CHESS_NUMBER; j++)
        {
            widgets[i][j] = std::make_unique<Sensor>(this, &board, i, j, widgets);
            widgets[i][j]->move(transPosition(i) - BOARD_PIECE_SPACING / 2, transPosition(j) - BOARD_PIECE_SPACING / 2);
            widgets[i][j]->resize(BOARD_PIECE_SPACING, BOARD_PIECE_SPACING);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), BackGroundColor);
    painter.setPen(QPen(LineColor, BOARD_LINE_WIDTH));
    for (auto &line : lines)
    {
        painter.drawLine(line);
    }
    painter.setBrush(LineColor);
    for (const auto &point : starPoints)
    {
        painter.drawEllipse(point, BOARD_STAR_POINT_WIDTH, BOARD_STAR_POINT_WIDTH);
    }
}
