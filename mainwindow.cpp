#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFont>
#include <QShortcut>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ═══════════════════════════════════════════════════════
    //  菜单栏
    // ═══════════════════════════════════════════════════════
    menuBar_ = new QMenuBar(this);

    // ── 游戏 ──
    gameMenu_ = new QMenu("游戏(&G)", this);

    newGameAction_ = new QAction("新游戏(&N)", this);
    newGameAction_->setShortcut(QKeySequence("Ctrl+N"));
    gameMenu_->addAction(newGameAction_);

    undoAction_ = new QAction("撤销(&U)", this);
    undoAction_->setShortcut(QKeySequence("Ctrl+Z"));
    undoAction_->setEnabled(false);
    gameMenu_->addAction(undoAction_);

    gameMenu_->addSeparator();

    exitAction_ = new QAction("退出(&X)", this);
    exitAction_->setShortcut(QKeySequence("Ctrl+Q"));
    gameMenu_->addAction(exitAction_);

    menuBar_->addMenu(gameMenu_);

    // ── 帮助 ──
    helpMenu_ = new QMenu("帮助(&H)", this);

    instructionsAction_ = new QAction("游戏说明(&I)", this);
    helpMenu_->addAction(instructionsAction_);

    aboutAction_ = new QAction("关于(&A)", this);
    helpMenu_->addAction(aboutAction_);

    menuBar_->addMenu(helpMenu_);

    setMenuBar(menuBar_);

    // ═══════════════════════════════════════════════════════
    //  信号连接
    // ═══════════════════════════════════════════════════════
    connect(newGameAction_,     &QAction::triggered, this, &MainWindow::menuNewGame);
    connect(undoAction_,        &QAction::triggered, this, &MainWindow::menuUndo);
    connect(exitAction_,        &QAction::triggered, this, &MainWindow::menuExit);
    connect(instructionsAction_,&QAction::triggered, this, &MainWindow::menuInstructions);
    connect(aboutAction_,       &QAction::triggered, this, &MainWindow::menuAbout);

    // ═══════════════════════════════════════════════════════
    //  中央区域
    // ═══════════════════════════════════════════════════════
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *topLayout  = new QHBoxLayout;

    boardWidget = new BoardWidget(this);
    scoreLabel  = new QLabel("Score: 0", this);

    QFont font = scoreLabel->font();
    font.setPointSize(16);
    scoreLabel->setFont(font);

    restartButton = new QPushButton("Restart", this);

    topLayout->addWidget(scoreLabel);
    topLayout->addStretch();
    topLayout->addWidget(restartButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(boardWidget);

    central->setLayout(mainLayout);

    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartGame);

    // ═══════════════════════════════════════════════════════
    //  状态栏 —— 显示最高分
    // ═══════════════════════════════════════════════════════
    highScoreLabel = new QLabel(this);
    statusBar()->addPermanentWidget(highScoreLabel);

    resize(450, 570);
    setWindowTitle("2048 Qt — 元素版");

    game.init();
    boardWidget->setBoard(&game.getBoard());
    updateUI();
}

// ── 槽函数 ────────────────────────────────────────────

void MainWindow::restartGame() {
    game.init();
    updateUI();
}

void MainWindow::menuNewGame() {
    restartGame();
}

void MainWindow::menuUndo() {
    if (!game.canUndo()) return;
    game.undo();
    updateUI();
}

void MainWindow::menuExit() {
    QApplication::quit();
}

void MainWindow::menuInstructions() {
    QMessageBox box(this);
    box.setWindowTitle("游戏说明");
    box.setIcon(QMessageBox::Information);
    box.setText(
        "<h3>2048 — 元素合成版</h3>"
        "<p><b>目标：</b>合成尽可能重的元素！</p>"
        "<p><b>操作：</b></p>"
        "<ul>"
        "<li><b>方向键 / WASD</b> — 移动方块</li>"
        "<li><b>Ctrl+Z</b> — 撤销上一步</li>"
        "<li><b>Ctrl+N</b> — 新游戏</li>"
        "</ul>"
        "<p><b>规则：</b></p>"
        "<ul>"
        "<li>两个相同元素碰撞 → 合成下一个元素</li>"
        "<li>H + H → He，He + He → Li …</li>"
        "<li>每步随机生成 H（90%）或 He（10%）</li>"
        "<li>棋盘满且无法合并时游戏结束</li>"
        "</ul>"
        "<p>你能合成到哪个元素？试试看！</p>"
    );
    box.exec();
}

void MainWindow::menuAbout() {
    QMessageBox box(this);
    box.setWindowTitle("关于");
    box.setIcon(QMessageBox::Information);
    box.setText(
        "<h3>2048 Qt — 元素版</h3>"
        "<p>版本 1.0</p>"
        "<p>基于 Qt6 / C++17 开发</p>"
        "<p>用化学元素周期表重新演绎经典 2048 游戏。</p>"
    );
    box.exec();
}

// ── UI 刷新 ───────────────────────────────────────────

void MainWindow::updateUI() {
    boardWidget->setBoard(&game.getBoard());
    scoreLabel->setText(QString("Score: %1").arg(game.getScore()));
    undoAction_->setEnabled(game.canUndo());

    // 状态栏
    int hs = game.getHighScore();
    highScoreLabel->setText(
        hs > 0 ? QString("  最高分: %1  ").arg(hs) : "");
}

// ── 移动处理 ──────────────────────────────────────────

void MainWindow::handleMove(Direction dir) {
    if (game.isOver())
        return;
    bool moved = game.move(dir);
    if (moved)
        updateUI();
    if (game.isOver())
        showGameOver();
}

void MainWindow::showGameOver() {
    // 如果这次破了纪录，给点提示
    QString msg = QString("游戏结束！\n\n最终得分: %1").arg(game.getScore());
    if (game.getScore() >= game.getHighScore() && game.getScore() > 0) {
        msg += "\n\n🎉 新纪录！";
    }
    QMessageBox::information(this, "Game Over", msg);
}

// ── 键盘事件 ──────────────────────────────────────────

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        handleMove(Direction::Left);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        handleMove(Direction::Right);
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        handleMove(Direction::Up);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        handleMove(Direction::Down);
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}
