#include "mainwindow.h"
#include "ElementDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFont>
#include <QShortcut>
#include <QApplication>
#include <QGraphicsDropShadowEffect>

// ═══════════════════════════════════════════════════════
//  构造
// ═══════════════════════════════════════════════════════

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ── 菜单栏 ──────────────────────────────────────
    menuBar_ = new QMenuBar(this);

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

    viewMenu_ = new QMenu("查看(&V)", this);

    periodicTableAction_ = new QAction("元素周期表(&P)", this);
    periodicTableAction_->setShortcut(QKeySequence("Ctrl+T"));
    viewMenu_->addAction(periodicTableAction_);

    menuBar_->addMenu(viewMenu_);

    helpMenu_ = new QMenu("帮助(&H)", this);

    instructionsAction_ = new QAction("游戏说明(&I)", this);
    helpMenu_->addAction(instructionsAction_);

    aboutAction_ = new QAction("关于(&A)", this);
    helpMenu_->addAction(aboutAction_);

    menuBar_->addMenu(helpMenu_);

    setMenuBar(menuBar_);

    // ── 信号连接 ────────────────────────────────────
    connect(newGameAction_,     &QAction::triggered, this, &MainWindow::menuNewGame);
    connect(undoAction_,        &QAction::triggered, this, &MainWindow::menuUndo);
    connect(exitAction_,        &QAction::triggered, this, &MainWindow::menuExit);
    connect(instructionsAction_,&QAction::triggered, this, &MainWindow::menuInstructions);
    connect(aboutAction_,       &QAction::triggered, this, &MainWindow::menuAbout);
    connect(periodicTableAction_,&QAction::triggered, this, &MainWindow::menuPeriodicTable);

    // ═══════════════════════════════════════════════════════
    //  中央区域 — 左右布局
    // ═══════════════════════════════════════════════════════
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    // ── 左侧：棋盘 ──
    boardWidget = new BoardWidget(this);
    boardWidget->setFixedSize(420, 420);
    boardWidget->setObjectName("boardFrame");
    {
        auto *boardShadow = new QGraphicsDropShadowEffect(boardWidget);
        boardShadow->setBlurRadius(16);
        boardShadow->setOffset(0, 4);
        boardShadow->setColor(QColor(0, 0, 0, 35));
        boardWidget->setGraphicsEffect(boardShadow);
    }
    mainLayout->addWidget(boardWidget);

    // ── 右侧：信息面板 ──
    rightPanel = new QWidget(this);
    rightPanel->setObjectName("rightPanel");
    rightPanel->setFixedWidth(140);
    QVBoxLayout *panelLayout = new QVBoxLayout;
    panelLayout->setContentsMargins(0, 4, 0, 4);
    panelLayout->setSpacing(12);

    // 分数卡片
    scoreCard = createScoreCard("分 数", scoreValueLabel);
    panelLayout->addWidget(scoreCard);

    // 最高分卡片
    highScoreCard = createScoreCard("最高分", highScoreValueLabel);
    panelLayout->addWidget(highScoreCard);

    panelLayout->addStretch();

    // 撤销按钮
    undoButton = new QPushButton("↩ 撤销", this);
    undoButton->setCursor(Qt::PointingHandCursor);
    undoButton->setEnabled(false);
    panelLayout->addWidget(undoButton);

    // 重新开始按钮
    restartButton = new QPushButton("⟳ 重新开始", this);
    restartButton->setCursor(Qt::PointingHandCursor);
    panelLayout->addWidget(restartButton);

    rightPanel->setLayout(panelLayout);
    mainLayout->addWidget(rightPanel);

    central->setLayout(mainLayout);

    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartGame);
    connect(undoButton,    &QPushButton::clicked, this, &MainWindow::menuUndo);

    // ═══════════════════════════════════════════════════════
    //  窗口设置
    // ═══════════════════════════════════════════════════════
    setFixedSize(600, 480);
    setWindowTitle("2048 Qt — 元素版");

    // 样式
    setupStyle();

    game.init();
    boardWidget->setBoard(&game.getBoard());
    updateUI();
}

// ═══════════════════════════════════════════════════════
//  创建分数卡片
// ═══════════════════════════════════════════════════════

QFrame* MainWindow::createScoreCard(const QString &title, QLabel *&valueLabel)
{
    QFrame *card = new QFrame(this);
    card->setObjectName("scoreCard");
    card->setFixedHeight(80);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(2);

    QLabel *titleLbl = new QLabel(title, card);
    titleLbl->setObjectName("scoreTitle");
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setFont(QFont("Microsoft YaHei", 10));

    valueLabel = new QLabel("0", card);
    valueLabel->setObjectName("scoreValue");
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setFont(QFont("Microsoft YaHei", 18, QFont::Bold));

    layout->addWidget(titleLbl);
    layout->addWidget(valueLabel);
    card->setLayout(layout);

    auto *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(12);
    shadow->setOffset(0, 2);
    shadow->setColor(QColor(0, 0, 0, 30));
    card->setGraphicsEffect(shadow);

    return card;
}

// ═══════════════════════════════════════════════════════
//  全局样式
// ═══════════════════════════════════════════════════════

void MainWindow::setupStyle()
{
    setStyleSheet(R"(
        /* 窗口背景 */
        QMainWindow {
            background-color: #FAF8EF;
        }
        QWidget {
            font-family: "Microsoft YaHei";
        }

        /* 棋盘框架 */
        #boardFrame {
            border: 2px solid #9C8D7B;
            border-radius: 6px;
        }

        /* 分数卡片 */
        #scoreCard {
            background-color: #BBADA0;
            border: 1px solid #C8BBA8;
            border-radius: 12px;
        }
        #scoreTitle {
            color: #EEE4DA;
            font-size: 10pt;
            font-weight: bold;
        }
        #scoreValue {
            color: #FFFFFF;
            font-size: 18pt;
            font-weight: bold;
        }

        /* 按钮 — 仅右侧面板 */
        #rightPanel QPushButton {
            background-color: #BBADA0;
            color: #FFFFFF;
            border: none;
            border-radius: 8px;
            padding: 10px 0px;
            font-size: 11pt;
            font-weight: bold;
            font-family: "Microsoft YaHei";
        }
        #rightPanel QPushButton:hover {
            background-color: #9C8D7B;
        }
        #rightPanel QPushButton:pressed {
            background-color: #8D7B69;
        }
        #rightPanel QPushButton:disabled {
            background-color: #D6CDC4;
            color: #C0B6AC;
        }

        /* 菜单栏 */
        QMenuBar {
            background-color: #BBADA0;
            color: #FFFFFF;
            font-size: 10pt;
            font-weight: bold;
            font-family: "Microsoft YaHei";
            border-bottom: 2px solid #9C8D7B;
        }
        QMenuBar::item:selected {
            background-color: #9C8D7B;
            border-radius: 4px;
        }
        QMenu {
            background-color: #FAF8EF;
            color: #776E65;
            border: 2px solid #BBADA0;
            font-family: "Microsoft YaHei";
        }
        QMenu::item:selected {
            background-color: #EDE0C8;
            border-radius: 4px;
        }
    )");
}

// ═══════════════════════════════════════════════════════
//  槽函数
// ═══════════════════════════════════════════════════════

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

void MainWindow::menuPeriodicTable() {
    if (!periodicTable) {
        periodicTable = new PeriodicTable(this);
        // 点击已发现元素 → 弹出详情
        connect(periodicTable, &PeriodicTable::elementClicked,
                this, [this](int an) {
            auto *dlg = new ElementDialog(an, periodicTable);
            dlg->exec();
        });
    }
    periodicTable->show();
    periodicTable->raise();
    periodicTable->activateWindow();
}

void MainWindow::menuExit() {
    QApplication::quit();
}

void MainWindow::menuInstructions() {
    QMessageBox box(this);
    box.setWindowTitle("游戏说明");
    box.setIcon(QMessageBox::Information);
    box.setFont(QFont("Microsoft YaHei", 9));
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
        "<p style='text-align:center;'><b>合成链：</b><br>"
        "H → He → Li → Be → B → C → N → O → F → Ne<br>"
        "Na → Mg → Al → Si → P → S → Cl → Ar → K → Ca</p>"
        "<p>你能合成到哪个元素？试试看！</p>"
    );
    box.exec();
}

void MainWindow::menuAbout() {
    QMessageBox box(this);
    box.setWindowTitle("关于");
    box.setIcon(QMessageBox::Information);
    box.setFont(QFont("Microsoft YaHei", 9));
    box.setText(
        "<h3>2048 Qt — 元素版</h3>"
        "<p>版本 1.0</p>"
        "<p>基于 Qt6 / C++17 开发</p>"
        "<p>用化学元素周期表重新演绎经典 2048 游戏。</p>"
    );
    box.exec();
}

// ═══════════════════════════════════════════════════════
//  UI 刷新
// ═══════════════════════════════════════════════════════

void MainWindow::updateUI() {
    boardWidget->setBoard(&game.getBoard());
    scoreValueLabel->setText(QString::number(game.getScore()));
    highScoreValueLabel->setText(QString::number(game.getHighScore()));
    undoAction_->setEnabled(game.canUndo());
    undoButton->setEnabled(game.canUndo());
}

// ═══════════════════════════════════════════════════════
//  移动处理
// ═══════════════════════════════════════════════════════

void MainWindow::handleMove(Direction dir) {
    if (game.isOver())
        return;
    bool moved = game.move(dir);
    if (moved) {
        updateUI();
        // 通知周期表新发现的元素
        if (periodicTable)
            periodicTable->addDiscoveries(game.getLastMerged());
    }
    if (game.isOver())
        showGameOver();
}

void MainWindow::showGameOver() {
    QString msg = QString("游戏结束！\n\n最终得分: %1").arg(game.getScore());
    if (game.getScore() >= game.getHighScore() && game.getScore() > 0) {
        msg += "\n\n🎉 新纪录！";
    }
    QMessageBox box(this);
    box.setWindowTitle("游戏结束");
    box.setIcon(QMessageBox::Information);
    box.setFont(QFont("Microsoft YaHei", 9));
    box.setText(msg);
    box.exec();
}

// ═══════════════════════════════════════════════════════
//  键盘事件
// ═══════════════════════════════════════════════════════

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
