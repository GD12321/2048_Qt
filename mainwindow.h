#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include "boardwidget.h"
#include "Game2048.h"

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
    void restartGame();
private:
    BoardWidget *boardWidget;
    QLabel *scoreLabel;
    QPushButton *restartButton;
    Game2048 game;

    // ── 菜单栏 ──
    QMenuBar *menuBar_;
    QMenu    *gameMenu_;
    QAction  *newGameAction_;
    QAction  *undoAction_;
    QAction  *exitAction_;

    QMenu    *helpMenu_;
    QAction  *instructionsAction_;
    QAction  *aboutAction_;

    // ── 状态栏 ──
    QLabel   *highScoreLabel;

    void updateUI();
    void handleMove(Direction dir);
    void showGameOver();
};