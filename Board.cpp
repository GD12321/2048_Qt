#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

static const ElementInfo kElements[] = {
    {"", 0.0},
    {"H", 1.01}, {"He", 4.00}, {"Li", 6.94}, {"Be", 9.01},
    {"B", 10.81}, {"C", 12.01}, {"N", 14.01}, {"O", 16.00},
    {"F", 19.00}, {"Ne", 20.18},{"Na", 22.99}, {"Mg", 24.31},
    {"Al", 26.98}, {"Si", 28.09}, {"P", 30.97}, {"S", 32.07},
    {"Cl", 35.45},{"Ar", 39.95},{"K", 39.10},{"Ca", 40.08}
};

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
    int atomicNumber = (rand() % 10 == 0) ? 2 : 1;
    grid[emptyList[idx].first][emptyList[idx].second] = atomicNumber;
}

const int (*Board::getGrid() const)[4] {
    return grid;
}

int Board::getMaxAtomicNumber() const {
    int m = 0;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (grid[i][j] > m) m = grid[i][j];
    return m;
}

bool Board::slideAndMerge(int line[4], std::vector<int>* merged) {
    std::vector<int> temp;
    for (int i = 0; i < 4; ++i)
        if (line[i]) temp.push_back(line[i]);
    bool changed = temp.size() != 4;
    for (int i = 0; i + 1 < temp.size(); ++i) {
        if (temp[i] == temp[i + 1]) {
            temp[i] += 1; // 合成新元素
            if (merged) merged->push_back(temp[i]); // 记录新合成元素的原子序
            temp[i + 1] = 0;
            changed = true;
        }
    }
    std::vector<int> mergedLine;
    for (int n : temp) if (n) mergedLine.push_back(n);
    while (mergedLine.size() < 4) mergedLine.push_back(0);
    for (int i = 0; i < 4; ++i) {
        if (line[i] != mergedLine[i]) changed = true;
        line[i] = mergedLine[i];
    }
    return changed;
}

bool Board::move(Direction dir, std::vector<int>* merged) {
    bool changed = false;
    if (dir == Direction::Left) {
        for (int i = 0; i < 4; ++i) {
            int line[4];
            for (int j = 0; j < 4; ++j) line[j] = grid[i][j];
            if (slideAndMerge(line, merged)) changed = true;
            for (int j = 0; j < 4; ++j) grid[i][j] = line[j];
        }
    }
    else if (dir == Direction::Right) {
        for (int i = 0; i < 4; ++i) {
            int line[4];
            for (int j = 0; j < 4; ++j) line[j] = grid[i][3 - j];
            if (slideAndMerge(line, merged)) changed = true;
            for (int j = 0; j < 4; ++j) grid[i][3 - j] = line[j];
        }
    }
    else if (dir == Direction::Up) {
        for (int j = 0; j < 4; ++j) {
            int line[4];
            for (int i = 0; i < 4; ++i) line[i] = grid[i][j];
            if (slideAndMerge(line, merged)) changed = true;
            for (int i = 0; i < 4; ++i) grid[i][j] = line[i];
        }
    }
    else if (dir == Direction::Down) {
        for (int j = 0; j < 4; ++j) {
            int line[4];
            for (int i = 0; i < 4; ++i) line[i] = grid[3 - i][j];
            if (slideAndMerge(line, merged)) changed = true;
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

const ElementInfo* Board::getElementTable(int &count) {
    count = sizeof(kElements)/sizeof(ElementInfo);
    return kElements;
}

const char* Board::elementSymbol(int atomicNumber) {
    int size = sizeof(kElements)/sizeof(ElementInfo);
    if (atomicNumber >= 0 && atomicNumber < size)
        return kElements[atomicNumber].symbol;
    return "";
}
double Board::elementMass(int atomicNumber) {
    int size = sizeof(kElements)/sizeof(ElementInfo);
    if (atomicNumber >= 0 && atomicNumber < size)
        return kElements[atomicNumber].mass;
    return 0.0;
}