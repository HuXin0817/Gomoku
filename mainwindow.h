#pragma once

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QMenuBar>

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
    std::vector<std::vector<Sensor *>> widgets;
    bool over = false;

    static double transPosition(int x) { return BOARD_MARGIN() + x * BOARD_PIECE_SPACING; }

    void reload(const std::vector<std::pair<int, int>> &moveRecord);

private slots:
    void undo();

    void restart();

    void addBoardSize();

    void reduceBoardSize();

    void addChessNumber();

    void reduceChessNumber();

    void addChessWinNumber();

    void reduceChessWinNumber();
};
