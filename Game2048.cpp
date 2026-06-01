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
}

bool Game2048::move(Direction dir) {
    std::vector<int> mergedList;
    bool changed = board.move(dir, &mergedList);

    if (changed) {
        saveUndoState();
        board.addRandomTile();
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

void Game2048::saveUndoState() {
    const int (*g)[4] = board.getGrid();
    std::memcpy(undoGrid, g, sizeof(undoGrid));
    undoScore = score;
    hasUndoState = true;
}

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
