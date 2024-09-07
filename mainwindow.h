#pragma once

#include <QMainWindow>
#include <QMouseEvent>
#include <QShortcut>
#include <QParallelAnimationGroup>

#include "sensor.h"

#define Shortcut(key) new QShortcut(QKeySequence(key), this), &QShortcut::activated, this

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
    board_sensors sensors;

    void reload(const std::vector<point> &moveRecord = {});

    void reloadSize();

    std::vector<point> centerPieces() const;

    static double getMinWindowSize();

    void handleResizeEvent();

private slots:

    void undo();

    void addBoardSize();

    void reduceBoardSize();

    void addChessNumber();

    void reduceChessNumber();

    void restart();

    void setBlackAIPlayer() { setAIPlayer(ChessPlayer::BLACK); }

    void setWriteAIPlayer() { setAIPlayer(ChessPlayer::WHITE); }

    void setAIPlayer(ChessPlayer player);

    void notifyAIPlayer();
};
