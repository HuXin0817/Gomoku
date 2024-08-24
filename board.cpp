#include "board.h"

bool Board::checkLine(int x, int y, int dx, int dy) {
    int count = 0;
    int cx = x, cy = y;
    while (checkInBoard(cx, cy) && chessMap[cx][cy] == chessMap[x][y]) {
        cx -= dx;
        cy -= dy;
        count++;
    }
    cx = x, cy = y;
    while (checkInBoard(cx, cy) && chessMap[cx][cy] == chessMap[x][y]) {
        cx += dx;
        cy += dy;
        count++;
    }
    return count == WIN_PIECE_NUMBER + 1;
}

bool Board::checkWin(int x, int y) {
    if (checkLine(x, y, 1, 0)) {
        return true;
    }
    if (checkLine(x, y, 0, 1)) {
        return true;
    }
    if (checkLine(x, y, 1, 1)) {
        return true;
    }
    if (checkLine(x, y, 1, -1)) {
        return true;
    }
    return false;
}

void Board::addPiece(int x, int y) {
    if (isGameOver) {
        return;
    }
    if (x < 0 || x >= CHESS_NUMBER) {
        return;
    }
    if (y < 0 || y >= CHESS_NUMBER) {
        return;
    }
    if (chessMap[x][y] != ChessPlayer::NONE) {
        return;
    }
    chessMap[x][y] = nowPlayer;
    nowPlayer = nowPlayer == ChessPlayer::BLACK ? ChessPlayer::WRITE : ChessPlayer::BLACK;
    isGameOver = checkWin(x, y);
}
