#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include "boardwidget.h"
#include "Game2048.h"
#include "PeriodicTable.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void menuNewGame();
    void menuUndo();
    void menuExit();
    void menuInstructions();
    void menuAbout();
    void menuPeriodicTable();
    void restartGame();
private:
    BoardWidget *boardWidget;
    Game2048 game;

    // ── 右侧面板 ──
    QWidget   *rightPanel;
    QFrame    *scoreCard;
    QLabel    *scoreTitleLabel;
    QLabel    *scoreValueLabel;
    QFrame    *highScoreCard;
    QLabel    *highScoreTitleLabel;
    QLabel    *highScoreValueLabel;
    QPushButton *undoButton;
    QPushButton *restartButton;

    // ── 周期表 ──
    PeriodicTable *periodicTable = nullptr;

    // ── 菜单栏 ──
    QMenuBar *menuBar_;
    QMenu    *gameMenu_;
    QAction  *newGameAction_;
    QAction  *undoAction_;
    QAction  *exitAction_;

    QMenu    *viewMenu_;
    QAction  *periodicTableAction_;

    QMenu    *helpMenu_;
    QAction  *instructionsAction_;
    QAction  *aboutAction_;

    void updateUI();
    void handleMove(Direction dir);
    void showGameOver();

    // ── 样式辅助 ──
    void setupStyle();
    QFrame* createScoreCard(const QString &title, QLabel *&valueLabel);
};