#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), widgets(Config::CHESS_NUMBER)
{
    board = std::make_unique<Board>();
    for (auto &w : widgets)
    {
        w = std::vector<std::unique_ptr<Sensor>>(Config::CHESS_NUMBER);
    }
    setWindowOpacity(0.95);
    int boardSize = static_cast<int>(Config::BOARD_SIZE());
    setFixedSize(boardSize, boardSize);
    Sensor::handledGameOver = false;

    for (int i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (int j = 0; j < Config::CHESS_NUMBER; j++)
        {
            widgets[i][j] = std::make_unique<Sensor>(this, board.get(), i, j, &widgets);
            int mx = static_cast<int>(transPos(i) - Config::BOARD_PIECE_SPACING / 2);
            int my = static_cast<int>(transPos(j) - Config::BOARD_PIECE_SPACING / 2);
            widgets[i][j]->move(mx, my);
            int space = static_cast<int>(Config::BOARD_PIECE_SPACING);
            widgets[i][j]->resize(space, space);
        }
    }

    auto *restartShortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
    auto *undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    auto *addBoardSizeShortcut = new QShortcut(QKeySequence("Ctrl+Up"), this);
    auto *reduceBoardSizeShortcut = new QShortcut(QKeySequence("Ctrl+Down"), this);
    auto *addChessNumberShortcut = new QShortcut(QKeySequence("Ctrl+Right"), this);
    auto *reduceChessNumberShortcut = new QShortcut(QKeySequence("Ctrl+Left"), this);
    auto *addChessWinNumberShortcut = new QShortcut(QKeySequence("Ctrl+Shift+Right"), this);
    auto *reduceChessWinNumberShortcut = new QShortcut(QKeySequence("Ctrl+Shift+Left"), this);

    connect(restartShortcut, &QShortcut::activated, this, &MainWindow::restart);
    connect(undoShortcut, &QShortcut::activated, this, &MainWindow::undo);
    connect(addBoardSizeShortcut, &QShortcut::activated, this, &MainWindow::addBoardSize);
    connect(reduceBoardSizeShortcut, &QShortcut::activated, this, &MainWindow::reduceBoardSize);
    connect(addChessNumberShortcut, &QShortcut::activated, this, &MainWindow::addChessNumber);
    connect(reduceChessNumberShortcut, &QShortcut::activated, this, &MainWindow::reduceChessNumber);
    connect(addChessWinNumberShortcut, &QShortcut::activated, this, &MainWindow::addChessWinNumber);
    connect(reduceChessWinNumberShortcut, &QShortcut::activated, this, &MainWindow::reduceChessWinNumber);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), BackGroundColor);
    painter.setPen(QPen(LineColor, Config::BOARD_LINE_WIDTH()));
    for (int i = 0; i < Config::CHESS_NUMBER; i++)
    {
        for (int j = 0; j < Config::CHESS_NUMBER; j++)
        {
            QPointF point1(transPos(i), transPos(j));
            if (i + 1 < Config::CHESS_NUMBER)
            {
                QPointF point2(transPos(i + 1), transPos(j));
                painter.drawLine(point1, point2);
            }
            if (j + 1 < Config::CHESS_NUMBER)
            {
                QPointF point2(transPos(i), transPos(j + 1));
                painter.drawLine(point1, point2);
            }
        }
    }
    painter.setBrush(LineColor);
    auto starPositions = Config::StarPositions();
    for (int i : starPositions)
    {
        for (int j : starPositions)
        {
            QPointF point(transPos(i), transPos(j));
            painter.drawEllipse(point, Config::BOARD_STAR_POINT_WIDTH(), Config::BOARD_STAR_POINT_WIDTH());
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
    Config::BOARD_PIECE_SPACING *= 1.1;
    reload(board->getMoveRecords());
}

void MainWindow::reduceBoardSize()
{
    if (Config::BOARD_PIECE_SPACING / 1.1 > 30)
    {
        Config::BOARD_PIECE_SPACING /= 1.1;
        reload(board->getMoveRecords());
    }
}

void MainWindow::addChessNumber()
{
    Config::CHESS_NUMBER += 2;
    reload(board->getMoveRecords());
}

void MainWindow::reduceChessNumber()
{
    if (Config::CHESS_NUMBER - 2 > 0)
    {
        Config::CHESS_NUMBER -= 2;
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
    delete this;
    auto *mainWindow = new MainWindow();
    for (auto [x, y] : moveRecord)
    {
        if (x < Config::CHESS_NUMBER && y < Config::CHESS_NUMBER)
        {
            mainWindow->widgets[x][y]->press();
        }
    }
    mainWindow->move(oldPos);
    mainWindow->show();
}

void MainWindow::addChessWinNumber()
{
    if (Config::WIN_PIECE_NUMBER + 1 <= Config::CHESS_NUMBER)
    {
        Config::WIN_PIECE_NUMBER++;
        restart();
    }
}

void MainWindow::reduceChessWinNumber()
{
    if (Config::WIN_PIECE_NUMBER - 1 > 1)
    {
        Config::WIN_PIECE_NUMBER--;
        restart();
    }
}

double MainWindow::transPos(int x)
{
    return Config::BOARD_MARGIN() + x * Config::BOARD_PIECE_SPACING;
}
