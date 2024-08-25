#pragma once

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>

#include "common.h"
#include "board.h"
#include "sensor.h"

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Board board;
    std::vector<QLine> lines;
    std::vector<QPoint> starPoints;
    std::unique_ptr<Sensor> widgets[CHESS_NUMBER][CHESS_NUMBER];

    static int transPosition(int x) { return BOARD_MARGIN + x * BOARD_PIECE_SPACING; }
};
