#pragma once

#include <QMainWindow>
#include <QMouseEvent>
#include <QShortcut>
#include <QParallelAnimationGroup>

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
    board_sensors widgets;

    void reload(const std::vector<point> &moveRecord = {});

    void reloadSize();

    std::vector<point> centerPieces() const;

    static double getMinWindowSize();

    void fixSize(bool wait);

    void fixWidgetsSize()
    {
        for (int i = 0; i < Config::CHESS_NUMBER; i++)
        {
            for (int j = 0; j < Config::CHESS_NUMBER; j++)
            {
                auto space = Config::BOARD_PIECE_SPACING + 1;
                widgets[i][j]->setFixedSize(space, space);
                widgets[i][j]->show();
            }
        }
    }

    bool event(QEvent *event) override;

private slots:
    void undo();

    void restart() { reload(); }

    void addBoardSize();

    void reduceBoardSize();

    void addChessNumber();

    void reduceChessNumber();
};
