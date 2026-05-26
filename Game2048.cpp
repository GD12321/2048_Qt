#include "Game2048.h"
#include <vector>

Game2048::Game2048() {
    init();
}

void Game2048::init() {
    board.reset();
    score = 0;
}

bool Game2048::move(Direction dir) {
    // 新增：本回合合成产生的新块加总
    std::vector<int> mergedList;
    // 必须在Board中增加“记录本回合新合成块的原子序号”功能
    bool changed = board.move(dir, &mergedList);

    if (changed) {
        board.addRandomTile();
        for (int n: mergedList) {
            score += n; // 每合成一个元素，分数加新元素的原子序
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

Board& Game2048::getBoard() {
    return board;
}