#include "Game2048.h"
#include <vector>
#include <cstring>

Game2048::Game2048() {
    loadHighScore();
    init();
}

void Game2048::init() {
    board.reset();
    score = 0;
    hasUndoState = false;
    lastMerged.clear();
}

bool Game2048::move(Direction dir) {
    std::vector<int> mergedList;

    // 先暂存移动前的状态，只有移动确实改变了棋盘时才提升为撤销点
    int preMoveGrid[4][4];
    std::memcpy(preMoveGrid, board.getGrid(), sizeof(preMoveGrid));
    int preMoveScore = score;

    bool changed = board.move(dir, &mergedList);

    if (changed) {
        // 移动成功 — 将移动前的状态保存为撤销点
        std::memcpy(undoGrid, preMoveGrid, sizeof(undoGrid));
        undoScore = preMoveScore;
        hasUndoState = true;

        board.addRandomTile();
        lastMerged = mergedList;   // 供周期表查询
        for (int n: mergedList) {
            score += n;
        }
        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }
    }
    return changed;
}

bool Game2048::isOver() const {
    return !board.canMove();
}

int Game2048::getScore() const {
    return score;
}

int Game2048::getHighScore() const {
    return highScore;
}

Board& Game2048::getBoard() {
    return board;
}

// -- Undo --

bool Game2048::canUndo() const {
    return hasUndoState;
}

void Game2048::undo() {
    if (!hasUndoState) return;
    board.setGrid(undoGrid);
    score = undoScore;
    hasUndoState = false;
}

// -- High Score Persistence --

void Game2048::loadHighScore() {
    QSettings settings("MySoft", "2048");
    highScore = settings.value("highScore", 0).toInt();
}

void Game2048::saveHighScore() {
    QSettings settings("MySoft", "2048");
    settings.setValue("highScore", highScore);
}
