#include "board.h"

int Board::WIN_PIECE_NUMBER = 5;

bool Board::checkLine(int x, int y, int dx, int dy) const
{
    int count = 0;
    int cx = x, cy = y;
    while (checkInBoard(cx, cy) && chessMap[cx][cy] == chessMap[x][y])
    {
        cx -= dx;
        cy -= dy;
        count++;
    }
    cx = x, cy = y;
    while (checkInBoard(cx, cy) && chessMap[cx][cy] == chessMap[x][y])
    {
        cx += dx;
        cy += dy;
        count++;
    }
    return count > WIN_PIECE_NUMBER;
}

bool Board::checkWin(int x, int y) const
{
    if (checkLine(x, y, 1, 0))
    {
        return true;
    }
    if (checkLine(x, y, 0, 1))
    {
        return true;
    }
    if (checkLine(x, y, 1, 1))
    {
        return true;
    }
    if (checkLine(x, y, 1, -1))
    {
        return true;
    }
    return false;
}

void Board::addPiece(int x, int y)
{
    if (!judgeIsPos(x, y))
    {
        return;
    }
    chessMap[x][y] = nowPlayer;
    moveRecords.emplace_back(x, y);
    nowPlayer = nowPlayer == ChessPlayer::BLACK ? ChessPlayer::WRITE : ChessPlayer::BLACK;
    gameOver = checkWin(x, y);
}

bool Board::judgeIsPos(int x, int y) const
{
    if (gameOver)
    {
        return false;
    }
    if (x < 0 || x >= CHESS_NUMBER)
    {
        return false;
    }
    if (y < 0 || y >= CHESS_NUMBER)
    {
        return false;
    }
    if (chessMap[x][y] != ChessPlayer::NONE)
    {
        return false;
    }
    return true;
}

std::vector<std::pair<int, int>> Board::winPieces() const
{
    std::vector<std::pair<int, int>> pos;
    for (int i = 0; i < CHESS_NUMBER; i++)
    {
        for (int j = 0; j < CHESS_NUMBER; j++)
        {
            if (findOne(i, j, 1, 0, pos))
            {
                return pos;
            }
            if (findOne(i, j, 1, 1, pos))
            {
                return pos;
            }
            if (findOne(i, j, 1, -1, pos))
            {
                return pos;
            }
            if (findOne(i, j, 0, 1, pos))
            {
                return pos;
            }
        }
    }
    return {};
}

bool Board::findOne(int x, int y, int dx, int dy, std::vector<std::pair<int, int>> &pos) const
{
    pos.clear();
    int mx = x, my = y;
    if (chessMap[x][y] == ChessPlayer::NONE)
    {
        return false;
    }
    while (checkInBoard(mx, my) && chessMap[mx][my] == chessMap[x][y])
    {
        pos.emplace_back(mx, my);
        mx += dx;
        my += dy;
    }
    return pos.size() >= WIN_PIECE_NUMBER;
}

bool Board::checkInBoard(int x, int y)
{
    return x >= 0 && x < CHESS_NUMBER && y >= 0 && y < CHESS_NUMBER;
}
