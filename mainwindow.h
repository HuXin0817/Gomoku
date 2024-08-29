#pragma once

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QShortcut>

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

    void resizeEvent(QResizeEvent *event) override;

private:
    std::unique_ptr<Board> board;
    std::vector<std::vector<std::unique_ptr<Sensor>>> widgets;

    void reload(const std::vector<point> &moveRecord);

    void reloadSize();

    std::vector<point> centerPieces() const;

    static int getMinWindowSize();

    void fixSize()
    {
        double width = size().width();
        double height = size().height();
        double midX = width / 2;
        double midY = height / 2;
        for (double i = 0; i < Config::CHESS_NUMBER; i++)
        {
            for (double j = 0; j < Config::CHESS_NUMBER; j++)
            {
                double x = midX + (i - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING;
                double y = midY + (j - Config::CHESS_NUMBER / 2) * Config::BOARD_PIECE_SPACING;
                widgets[i][j]->move(x, y);
                double space = Config::BOARD_PIECE_SPACING;
                widgets[i][j]->setFixedSize(space, space);
                widgets[i][j]->show();
            }
        }
    }

private slots:
    void undo();

    void restart() { reload({}); }

    void addBoardSize();

    void reduceBoardSize();

    void addChessNumber();

    void reduceChessNumber();
};
