#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <vector>

Board::Board() { reset(); }

void Board::reset() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            grid[i][j] = 0;
    addRandomTile();
    addRandomTile();
}

void Board::addRandomTile() {
    std::vector<std::pair<int, int>> emptyList;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (grid[i][j] == 0)
                emptyList.emplace_back(i, j);
    if (emptyList.empty()) return;
    int idx = rand() % emptyList.size();
    int value = (rand() % 10 == 0) ? 4 : 2; // 10%概率4
    grid[emptyList[idx].first][emptyList[idx].second] = value;
}

const int (*Board::getGrid() const)[4] {
    return grid;
}

int Board::getMaxTile() const {
    int m = 0;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (grid[i][j] > m) m = grid[i][j];
    return m;
}

bool Board::slideAndMerge(int line[4]) {
    std::vector<int> temp;
    for (int i = 0; i < 4; ++i)
        if (line[i]) temp.push_back(line[i]);
    bool changed = temp.size() != 4;
    for (int i = 0; i + 1 < temp.size(); ++i) {
        if (temp[i] == temp[i + 1]) {
            temp[i] *= 2;
            temp[i + 1] = 0;
            changed = true;
        }
    }
    std::vector<int> merged;
    for (int n : temp) if (n) merged.push_back(n);
    while (merged.size() < 4) merged.push_back(0);
    for (int i = 0; i < 4; ++i) {
        if (line[i] != merged[i]) changed = true;
        line[i] = merged[i];
    }
    return changed;
}

bool Board::move(Direction dir) {
    bool changed = false;
    if (dir == Direction::Left) {
        for (int i = 0; i < 4; ++i) {
            int line[4];
            for (int j = 0; j < 4; ++j) line[j] = grid[i][j];
            if (slideAndMerge(line)) changed = true;
            for (int j = 0; j < 4; ++j) grid[i][j] = line[j];
        }
    }
    else if (dir == Direction::Right) {
        for (int i = 0; i < 4; ++i) {
            int line[4];
            for (int j = 0; j < 4; ++j) line[j] = grid[i][3 - j];
            if (slideAndMerge(line)) changed = true;
            for (int j = 0; j < 4; ++j) grid[i][3 - j] = line[j];
        }
    }
    else if (dir == Direction::Up) {
        for (int j = 0; j < 4; ++j) {
            int line[4];
            for (int i = 0; i < 4; ++i) line[i] = grid[i][j];
            if (slideAndMerge(line)) changed = true;
            for (int i = 0; i < 4; ++i) grid[i][j] = line[i];
        }
    }
    else if (dir == Direction::Down) {
        for (int j = 0; j < 4; ++j) {
            int line[4];
            for (int i = 0; i < 4; ++i) line[i] = grid[3 - i][j];
            if (slideAndMerge(line)) changed = true;
            for (int i = 0; i < 4; ++i) grid[3 - i][j] = line[i];
        }
    }
    return changed;
}

bool Board::canMove() const {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            if (grid[i][j] == 0) return true;
            if (i < 3 && grid[i][j] == grid[i + 1][j]) return true;
            if (j < 3 && grid[i][j] == grid[i][j + 1]) return true;
        }
    return false;
}