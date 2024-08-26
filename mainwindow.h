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

    void reset();

    void moveWidgets();

private slots:
    void undo();

    void restart();

    void addBoardSize();

    void reduceBoardSize();

    void addChessNumber();

    void reduceChessNumber();

    void addWindowSize();

    void reduceWindowSize();
};
