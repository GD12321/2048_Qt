#include "PeriodicTable.h"
#include "ElementDialog.h"
#include "ElementData.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QApplication>

// ── 每个元素在周期表中的 (row, col) ──
// 18 列，4 行（周期 1-4）。未出现的坐标就是空位。
struct CellPos { int row; int col; };

static const CellPos kCellPositions[] = {
    {-1,-1},  // 0  unused
    {0, 0},   // 1  H
    {0, 17},  // 2  He
    {1, 0},   // 3  Li
    {1, 1},   // 4  Be
    {1, 12},  // 5  B
    {1, 13},  // 6  C
    {1, 14},  // 7  N
    {1, 15},  // 8  O
    {1, 16},  // 9  F
    {1, 17},  // 10 Ne
    {2, 0},   // 11 Na
    {2, 1},   // 12 Mg
    {2, 12},  // 13 Al
    {2, 13},  // 14 Si
    {2, 14},  // 15 P
    {2, 15},  // 16 S
    {2, 16},  // 17 Cl
    {2, 17},  // 18 Ar
    {3, 0},   // 19 K
    {3, 1},   // 20 Ca
};

static const int kCols = 18;
static const int kRows = 4;
static const int kCellSize = 52;

// ═══════════════════════════════════════════════════════════
PeriodicTable::PeriodicTable(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("元素周期表");
    setAttribute(Qt::WA_DeleteOnClose, false);
    setModal(false);

    cells.resize(kElementCount);  // index by atomic number

    loadDiscoveries();
    buildUI();
}

// ── 构建 18×4 网格 ──────────────────────────────────
void PeriodicTable::buildUI()
{
    QVBoxLayout *outer = new QVBoxLayout(this);
    outer->setContentsMargins(12, 12, 12, 12);

    auto *grid = new QGridLayout;
    grid->setSpacing(4);
    outer->addLayout(grid);

    // ── 列标号 (1-18) ──
    for (int c = 0; c < kCols; ++c) {
        auto *lbl = new QLabel(QString::number(c + 1), this);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setFont(QFont("Microsoft YaHei", 7));
        lbl->setStyleSheet("color: #999;");
        lbl->setFixedSize(kCellSize, 16);
        grid->addWidget(lbl, 0, c + 1);   // row 0 reserved for col headers
    }

    // ── 行标号 + 格子 ──
    for (int r = 0; r < kRows; ++r) {
        auto *rowLbl = new QLabel(QString::number(r + 1), this);
        rowLbl->setAlignment(Qt::AlignCenter);
        rowLbl->setFont(QFont("Microsoft YaHei", 7));
        rowLbl->setStyleSheet("color: #999;");
        rowLbl->setFixedSize(20, kCellSize);
        grid->addWidget(rowLbl, r + 1, 0);

        for (int c = 0; c < kCols; ++c) {
            // 占位：空单元格给一个透明固定尺寸
            auto *placeHolder = new QWidget(this);
            placeHolder->setFixedSize(kCellSize, kCellSize);
            placeHolder->setStyleSheet("background: transparent;");
            grid->addWidget(placeHolder, r + 1, c + 1);
        }
    }

    // ── 在正确位置放置 20 个元素按钮 ──
    for (int n = 1; n < kElementCount; ++n) {
        const CellPos &pos = kCellPositions[n];
        if (pos.row < 0) continue;

        auto *btn = new QPushButton(this);
        btn->setFixedSize(kCellSize, kCellSize);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFont(QFont("Microsoft YaHei", 14, QFont::Bold));
        btn->setStyleSheet(
            "QPushButton { border: none; border-radius: 6px; }"
        );

        // 点击 → 弹出详情
        int an = n;
        QObject::connect(btn, &QPushButton::clicked, this, [this, an]() {
            emit elementClicked(an);
        });

        cells[n] = btn;
        // 放到对应位置（grid row 偏移 +1 因为 row 0 是列标号）
        grid->addWidget(btn, pos.row + 1, pos.col + 1);
        updateCell(n);
    }

    // ── 底部图例 ──
    auto *legend = new QLabel(
        "灰色 = 未发现  |  彩色 = 已发现（点击查看详情）", this);
    legend->setFont(QFont("Microsoft YaHei", 8));
    legend->setStyleSheet("color: #999; margin-top: 8px;");
    legend->setAlignment(Qt::AlignCenter);
    outer->addWidget(legend);
}

// ── 更新单个格子外观 ──────────────────────────────────
void PeriodicTable::updateCell(int n)
{
    QPushButton *btn = cells[n];
    if (!btn) return;

    const ElementDetail &el = kElementDetails[n];
    bool discovered = discoveredSet.contains(n);

    if (discovered) {
        QColor c = el.tileColor;
        btn->setText(QString::fromUtf8(el.symbol));
        btn->setEnabled(true);
        // 浅色格深字，深色格白字
        bool light = (n <= 2);
        btn->setStyleSheet(QString(
            "QPushButton {"
            "  background-color: %1;"
            "  color: %2;"
            "  border: none;"
            "  border-radius: 6px;"
            "  font-size: 16pt;"
            "  font-weight: bold;"
            "  font-family: 'Microsoft YaHei';"
            "}"
            "QPushButton:hover {"
            "  border: 2px solid %3;"
            "}"
        ).arg(c.name())
         .arg(light ? "#3C3A32" : "#F9F6F2")
         .arg(light ? "#BBB0A0" : "#FFE080"));
    } else {
        btn->setText(QString::number(n));
        btn->setEnabled(false);
        btn->setStyleSheet(
            "QPushButton {"
            "  background-color: #CDC1B4;"
            "  color: #AAA090;"
            "  border: none;"
            "  border-radius: 6px;"
            "  font-size: 9pt;"
            "  font-family: 'Microsoft YaHei';"
            "}"
        );
    }
}

// ── 批量更新 ─────────────────────────────────────────
void PeriodicTable::addDiscoveries(const std::vector<int> &atomicNumbers)
{
    bool changed = false;
    for (int n : atomicNumbers) {
        if (n > 0 && n < kElementCount && !discoveredSet.contains(n)) {
            discoveredSet.insert(n);
            updateCell(n);
            changed = true;
        }
    }
    if (changed)
        saveDiscoveries();
}

// ── QSettings 持久化 ──────────────────────────────────
void PeriodicTable::loadDiscoveries()
{
    QSettings settings("MySoft", "2048");
    QStringList parts = settings.value("discoveries", "1,2").toString().split(",");
    for (const QString &s : parts) {
        bool ok;
        int n = s.toInt(&ok);
        if (ok && n > 0 && n < kElementCount)
            discoveredSet.insert(n);
    }
}

void PeriodicTable::saveDiscoveries()
{
    QStringList parts;
    for (int n : discoveredSet)
        parts.append(QString::number(n));
    QSettings settings("MySoft", "2048");
    settings.setValue("discoveries", parts.join(","));
}

// ── 外部按原子序取格子指针 ────────────────────────────
QPushButton* PeriodicTable::cellFor(int atomicNumber) const
{
    if (atomicNumber > 0 && atomicNumber < cells.size())
        return cells[atomicNumber];
    return nullptr;
}
