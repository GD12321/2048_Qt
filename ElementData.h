#pragma once

#include <QColor>

// ── 元素类别 ──
enum class ElementCategory {
    Empty,
    Nonmetal,         // 非金属
    NobleGas,         // 贵气体（稀有气体）
    AlkaliMetal,      // 碱金属
    AlkalineEarth,    // 碱土金属
    Metalloid,        // 类金属
    Halogen,          // 卤素
    PostTransition    // 后过渡金属
};

// ── 扩展元素信息 ──
struct ElementDetail {
    int           atomicNumber;
    const char   *symbol;
    const char   *chineseName;
    const char   *englishName;
    double        mass;
    ElementCategory category;
    const char   *categoryName;       // 中文类别名
    const char   *electronConfig;
    QColor        tileColor;          // 棋盘 / 周期表共用颜色
};

// ═══════════════════════════════════════════════════════════════
//  元素数据表（21 项: 0=空, 1=H, 2=He … 20=Ca）
//  颜色与 boardwidget.cpp 中的公式一致
// ═══════════════════════════════════════════════════════════════
static const ElementDetail kElementDetails[] = {
    // 0 – 空
    {0, "",   "",    "",       0.0,  ElementCategory::Empty,
     "",      "",    QColor(205, 193, 180)},

    // 1 – H  氢
    {1, "H",  "氢", "Hydrogen",   1.01,  ElementCategory::Nonmetal,
     "非金属",     "1s¹",              QColor(238, 228, 218)},

    // 2 – He 氦
    {2, "He", "氦", "Helium",     4.00,  ElementCategory::NobleGas,
     "贵气体",     "1s²",              QColor(237, 224, 200)},

    // 3 – Li 锂
    {3, "Li", "锂", "Lithium",    6.94,  ElementCategory::AlkaliMetal,
     "碱金属",     "[He]2s¹",         QColor(242, 177, 121)},

    // 4 – Be 铍
    {4, "Be", "铍", "Beryllium",  9.01,  ElementCategory::AlkalineEarth,
     "碱土金属",   "[He]2s²",         QColor(245, 149, 99)},

    // 5 – B  硼
    {5, "B",  "硼", "Boron",      10.81, ElementCategory::Metalloid,
     "类金属",     "[He]2s²2p¹",      QColor(246, 124, 95)},

    // 6 – C  碳
    {6, "C",  "碳", "Carbon",     12.01, ElementCategory::Nonmetal,
     "非金属",     "[He]2s²2p²",      QColor(246, 94, 59)},

    // 7 – N  氮  (公式: 202,152,99)
    {7, "N",  "氮", "Nitrogen",   14.01, ElementCategory::Nonmetal,
     "非金属",     "[He]2s²2p³",      QColor(202, 152, 99)},

    // 8 – O  氧  (公式: 197,146,96)
    {8, "O",  "氧", "Oxygen",     16.00, ElementCategory::Nonmetal,
     "非金属",     "[He]2s²2p⁴",      QColor(197, 146, 96)},

    // 9 – F  氟  (公式: 192,140,93)
    {9, "F",  "氟", "Fluorine",   19.00, ElementCategory::Halogen,
     "卤素",       "[He]2s²2p⁵",      QColor(192, 140, 93)},

    // 10 – Ne 氖 (公式: 187,134,90)
    {10, "Ne","氖", "Neon",       20.18, ElementCategory::NobleGas,
     "贵气体",     "[He]2s²2p⁶",      QColor(187, 134, 90)},

    // 11 – Na 钠 (公式: 182,128,87)
    {11, "Na","钠", "Sodium",     22.99, ElementCategory::AlkaliMetal,
     "碱金属",     "[Ne]3s¹",         QColor(182, 128, 87)},

    // 12 – Mg 镁 (公式: 177,122,84)
    {12, "Mg","镁", "Magnesium",  24.31, ElementCategory::AlkalineEarth,
     "碱土金属",   "[Ne]3s²",         QColor(177, 122, 84)},

    // 13 – Al 铝 (公式: 172,116,81)
    {13, "Al","铝", "Aluminum",   26.98, ElementCategory::PostTransition,
     "后过渡金属", "[Ne]3s²3p¹",      QColor(172, 116, 81)},

    // 14 – Si 硅 (公式: 167,110,78)
    {14, "Si","硅", "Silicon",    28.09, ElementCategory::Metalloid,
     "类金属",     "[Ne]3s²3p²",      QColor(167, 110, 78)},

    // 15 – P  磷 (公式: 162,104,75)
    {15, "P", "磷", "Phosphorus", 30.97, ElementCategory::Nonmetal,
     "非金属",     "[Ne]3s²3p³",      QColor(162, 104, 75)},

    // 16 – S  硫 (公式: 157,98,72)
    {16, "S", "硫", "Sulfur",     32.07, ElementCategory::Nonmetal,
     "非金属",     "[Ne]3s²3p⁴",      QColor(157, 98, 72)},

    // 17 – Cl 氯 (公式: 152,92,69)
    {17, "Cl","氯", "Chlorine",   35.45, ElementCategory::Halogen,
     "卤素",       "[Ne]3s²3p⁵",      QColor(152, 92, 69)},

    // 18 – Ar 氩 (公式: 147,86,66)
    {18, "Ar","氩", "Argon",      39.95, ElementCategory::NobleGas,
     "贵气体",     "[Ne]3s²3p⁶",      QColor(147, 86, 66)},

    // 19 – K  钾 (公式: 142,80,63)
    {19, "K", "钾", "Potassium",  39.10, ElementCategory::AlkaliMetal,
     "碱金属",     "[Ar]4s¹",         QColor(142, 80, 63)},

    // 20 – Ca 钙 (公式: 137,74,60)
    {20, "Ca","钙", "Calcium",    40.08, ElementCategory::AlkalineEarth,
     "碱土金属",   "[Ar]4s²",         QColor(137, 74, 60)},
};

constexpr int kElementCount = sizeof(kElementDetails) / sizeof(ElementDetail);
