#include "Game2048.h"

Game2048::Game2048() {
    init();
}

void Game2048::init() {
    board.reset();
    score = 0;
}

bool Game2048::move(Direction dir) {
    bool changed = board.move(dir);
    if (changed) {
        board.addRandomTile();
        // 重新统计分数（所有非0格求和，实际可根据实现而来）
        int curScore = 0;
        auto g = board.getGrid();
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (g[i][j]) curScore += g[i][j];
        score = curScore;
    }
    return changed;
}

bool Game2048::isOver() const {
    return !board.canMove();
}

int Game2048::getScore() const {
    return score;
}

Board& Game2048::getBoard() {
    return board;
}