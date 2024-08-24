#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(BOARD_SIZE, BOARD_SIZE);
    for (int i = 0; i < CHESS_NUMBER; i++) {
        for (int j = 0; j < CHESS_NUMBER; j++) {
            if (i + 1 < CHESS_NUMBER) {
                lines.emplace_back(transPosition(i), transPosition(j), transPosition(i + 1), transPosition(j));
            }
            if (j + 1 < CHESS_NUMBER) {
                lines.emplace_back(transPosition(i), transPosition(j), transPosition(i), transPosition(j + 1));
            }
        }
    }

    for (int i: StarPositions) {
        for (int j: StarPositions) {
            starPoints.emplace_back(transPosition(i), transPosition(j));
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), BackGroundColor);

    painter.setPen(QPen(LineColor, BOARD_LINE_WIDTH));
    for (auto &line: lines) {
        painter.drawLine(line);
    }

    painter.setBrush(LineColor);
    for (const auto &point: starPoints) {
        painter.drawEllipse(point, BOARD_STAR_POINT_WIDTH, BOARD_STAR_POINT_WIDTH);
    }

    painter.setPen(QPen(LineColor, 0));
    for (int i = 0; i < CHESS_NUMBER; i++) {
        for (int j = 0; j < CHESS_NUMBER; j++) {
            auto player = board.getMapInfo(i, j);
            QPoint point(transPosition(i), transPosition(j));
            QPoint shadowPoint(point.x() + 1, point.y() + 1);
            switch (player) {
                case ChessPlayer::NONE:
                    break;
                case ChessPlayer::BLACK: {
                    painter.setBrush(LineColor);
                    painter.drawEllipse(shadowPoint, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);

                    QRadialGradient gradient(point.x(), point.y(), BOARD_PIECE_WIDTH);
                    gradient.setColorAt(0, BlackMidPieceColor);
                    gradient.setColorAt(1, BlackFringePieceColor);
                    painter.setBrush(gradient);
                    painter.drawEllipse(point, BOARD_PIECE_WIDTH, BOARD_PIECE_WIDTH);
                    break;
                }
                case ChessPlayer::WRITE: {
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
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    auto posX = event->pos().x();
    auto posY = event->pos().y();
    auto x = (posX + BOARD_PIECE_SPACING / 2 - BOARD_MARGIN) / BOARD_PIECE_SPACING;
    auto y = (posY + BOARD_PIECE_SPACING / 2 - BOARD_MARGIN) / BOARD_PIECE_SPACING;
    board.addPiece(x, y);
    update();
}
