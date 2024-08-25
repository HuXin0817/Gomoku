#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), widgets(CHESS_NUMBER)
{
    board = std::make_unique<Board>();
    for (auto &w : widgets)
    {
        w = std::vector<std::unique_ptr<Sensor>>(CHESS_NUMBER);
    }
    setWindowOpacity(0.95);
    int boardSize = static_cast<int>(BOARD_SIZE());
    setFixedSize(boardSize, boardSize);
    Sensor::handledGameOver = false;

    for (int i = 0; i < CHESS_NUMBER; i++)
    {
        for (int j = 0; j < CHESS_NUMBER; j++)
        {
            widgets[i][j] = std::make_unique<Sensor>(this, board.get(), i, j, &widgets);
            int mx = static_cast<int>(transPos(i) - BOARD_PIECE_SPACING / 2);
            int my = static_cast<int>(transPos(j) - BOARD_PIECE_SPACING / 2);
            widgets[i][j]->move(mx, my);
            int space = static_cast<int>(BOARD_PIECE_SPACING);
            widgets[i][j]->resize(space, space);
        }
    }

    auto *menuBar = new QMenuBar(this);
    auto *gameControlMenu = new QMenu("Game", this);
    auto *restartAction = new QAction("Restart", this);
    auto *undoAction = new QAction("Undo", this);
    auto *addBoardSizeAction = new QAction("Add Board Size", this);
    auto *reduceBoardSizeAction = new QAction("Reduce Board Size", this);
    auto *addChessNumberAction = new QAction("Add Chess Number", this);
    auto *reduceChessNumberAction = new QAction("Reduce Chess Number", this);
    auto *addChessWinNumberAction = new QAction("Add Chess Win Number", this);
    auto *reduceChessWinNumberAction = new QAction("Reduce Chess Win Number", this);

    restartAction->setShortcut(QKeySequence("Ctrl+R"));
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    addBoardSizeAction->setShortcut(QKeySequence("Ctrl+Up"));
    reduceBoardSizeAction->setShortcut(QKeySequence("Ctrl+Down"));
    addChessNumberAction->setShortcut(QKeySequence("Ctrl+Right"));
    reduceChessNumberAction->setShortcut(QKeySequence("Ctrl+Left"));
    addChessWinNumberAction->setShortcut(QKeySequence("Ctrl+Shift+Right"));
    reduceChessWinNumberAction->setShortcut(QKeySequence("Ctrl+Shift+Left"));

    gameControlMenu->addAction(restartAction);
    gameControlMenu->addAction(undoAction);
    gameControlMenu->addSeparator();
    gameControlMenu->addAction(addBoardSizeAction);
    gameControlMenu->addAction(reduceBoardSizeAction);
    gameControlMenu->addAction(addChessNumberAction);
    gameControlMenu->addAction(reduceChessNumberAction);
    gameControlMenu->addAction(addChessWinNumberAction);
    gameControlMenu->addAction(reduceChessWinNumberAction);
    menuBar->addMenu(gameControlMenu);
    setMenuBar(menuBar);

    connect(restartAction, &QAction::triggered, this, &MainWindow::restart);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);
    connect(addBoardSizeAction, &QAction::triggered, this, &MainWindow::addBoardSize);
    connect(reduceBoardSizeAction, &QAction::triggered, this, &MainWindow::reduceBoardSize);
    connect(addChessNumberAction, &QAction::triggered, this, &MainWindow::addChessNumber);
    connect(reduceChessNumberAction, &QAction::triggered, this, &MainWindow::reduceChessNumber);
    connect(addChessWinNumberAction, &QAction::triggered, this, &MainWindow::addChessWinNumber);
    connect(reduceChessWinNumberAction, &QAction::triggered, this, &MainWindow::reduceChessWinNumber);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), BackGroundColor);
    painter.setPen(QPen(LineColor, BOARD_LINE_WIDTH()));
    for (int i = 0; i < CHESS_NUMBER; i++)
    {
        for (int j = 0; j < CHESS_NUMBER; j++)
        {
            QPointF point1(transPos(i), transPos(j));
            if (i + 1 < CHESS_NUMBER)
            {
                QPointF point2(transPos(i + 1), transPos(j));
                painter.drawLine(point1, point2);
            }
            if (j + 1 < CHESS_NUMBER)
            {
                QPointF point2(transPos(i), transPos(j + 1));
                painter.drawLine(point1, point2);
            }
        }
    }
    painter.setBrush(LineColor);
    auto starPositions = StarPositions();
    for (int i : starPositions)
    {
        for (int j : starPositions)
        {
            QPointF point(transPos(i), transPos(j));
            painter.drawEllipse(point, BOARD_STAR_POINT_WIDTH(), BOARD_STAR_POINT_WIDTH());
        }
    }
}

void MainWindow::restart() { reload({}); }

void MainWindow::undo()
{
    auto moveRecord = board->getMoveRecords();
    moveRecord.pop_back();
    reload(moveRecord);
}

void MainWindow::addBoardSize()
{
    BOARD_PIECE_SPACING *= 1.1;
    reload(board->getMoveRecords());
}

void MainWindow::reduceBoardSize()
{
    if (BOARD_PIECE_SPACING / 1.1 > 30)
    {
        BOARD_PIECE_SPACING /= 1.1;
        reload(board->getMoveRecords());
    }
}

void MainWindow::addChessNumber()
{
    CHESS_NUMBER += 2;
    reload(board->getMoveRecords());
}

void MainWindow::reduceChessNumber()
{
    if (CHESS_NUMBER - 2 > 0)
    {
        CHESS_NUMBER -= 2;
        reload(board->getMoveRecords());
    }
}

void MainWindow::reload(const std::vector<std::pair<int, int>> &moveRecord)
{
    auto oldPos = pos();
    for (auto &widget : widgets)
    {
        for (auto &w : widget)
        {
            w->close();
        }
    }
    close();
    auto *mainWindow = new MainWindow();
    for (auto [x, y] : moveRecord)
    {
        mainWindow->widgets[x][y]->press();
    }
    mainWindow->move(oldPos);
    mainWindow->show();
}

void MainWindow::addChessWinNumber()
{
    if (Board::WIN_PIECE_NUMBER + 1 <= CHESS_NUMBER)
    {
        Board::WIN_PIECE_NUMBER++;
        restart();
    }
}

void MainWindow::reduceChessWinNumber()
{
    if (Board::WIN_PIECE_NUMBER - 1 > 1)
    {
        Board::WIN_PIECE_NUMBER--;
        restart();
    }
}

double MainWindow::transPos(int x)
{
    return BOARD_MARGIN() + x * BOARD_PIECE_SPACING;
}
