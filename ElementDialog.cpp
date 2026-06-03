#include "ElementDialog.h"
#include "ElementData.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QFrame>

ElementDialog::ElementDialog(int atomicNumber, QWidget *parent)
    : QDialog(parent)
{
    if (atomicNumber < 1 || atomicNumber >= kElementCount)
        atomicNumber = 1;

    const ElementDetail &el = kElementDetails[atomicNumber];
    bool light = (atomicNumber <= 2);

    setWindowTitle(QString("%1 — 元素详情").arg(QString::fromUtf8(el.chineseName)));
    setFixedSize(300, 340);
    setModal(true);

    // ── 全局字体 ──
    setStyleSheet("QDialog { background: #FAF8EF; font-family: 'Microsoft YaHei'; }");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ═══════════════════════════════════════════════
    //  顶部彩色条
    // ═══════════════════════════════════════════════
    auto *header = new QFrame(this);
    header->setFixedHeight(100);
    header->setStyleSheet(QString(
        "QFrame { background-color: %1; border-bottom: 2px solid #9C8D7B; }"
    ).arg(el.tileColor.name()));

    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(20, 12, 20, 12);

    // 符号（大号）
    auto *symbolLbl = new QLabel(QString::fromUtf8(el.symbol), header);
    symbolLbl->setFont(QFont("Microsoft YaHei", 40, QFont::Bold));
    symbolLbl->setStyleSheet(QString("color: %1; background: transparent;")
        .arg(light ? "#3C3A32" : "#FFFFFF"));

    // 右侧：原子序 + 中文名
    auto *rightCol = new QVBoxLayout;
    rightCol->setSpacing(2);

    auto *numLbl = new QLabel(QString::number(el.atomicNumber), header);
    numLbl->setFont(QFont("Microsoft YaHei", 20, QFont::Bold));
    numLbl->setStyleSheet(QString("color: %1; background: transparent;")
        .arg(light ? "#5C5650" : "#F0E8D8"));

    auto *cnLbl = new QLabel(QString::fromUtf8(el.chineseName), header);
    cnLbl->setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
    cnLbl->setStyleSheet(QString("color: %1; background: transparent;")
        .arg(light ? "#3C3A32" : "#FFFFFF"));

    auto *enLbl = new QLabel(QString::fromUtf8(el.englishName), header);
    enLbl->setFont(QFont("Microsoft YaHei", 10));
    enLbl->setStyleSheet(QString("color: %1; background: transparent;")
        .arg(light ? "#6E6860" : "#E8E0D0"));

    rightCol->addWidget(numLbl);
    rightCol->addWidget(cnLbl);
    rightCol->addWidget(enLbl);
    headerLayout->addWidget(symbolLbl);
    headerLayout->addSpacing(18);
    headerLayout->addLayout(rightCol);
    headerLayout->addStretch();
    root->addWidget(header);

    // ═══════════════════════════════════════════════
    //  详情字段
    // ═══════════════════════════════════════════════
    struct Field { const char *label; QString value; };
    Field fields[] = {
        {"英文名",  QString::fromUtf8(el.englishName)},
        {"原子量",  QString::number(el.mass, 'f', 2)},
        {"类别",    QString::fromUtf8(el.categoryName)},
        {"电子排布", QString::fromUtf8(el.electronConfig)},
    };

    auto *body = new QVBoxLayout;
    body->setContentsMargins(24, 16, 24, 12);
    body->setSpacing(10);

    for (const auto &f : fields) {
        auto *row = new QHBoxLayout;
        row->setSpacing(12);

        auto *keyLbl = new QLabel(QString::fromUtf8(f.label), this);
        keyLbl->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
        keyLbl->setStyleSheet("color: #776E65;");
        keyLbl->setFixedWidth(60);

        auto *valLbl = new QLabel(f.value, this);
        valLbl->setFont(QFont("Microsoft YaHei", 10));
        valLbl->setStyleSheet("color: #3C3A32;");
        valLbl->setWordWrap(true);

        row->addWidget(keyLbl);
        row->addWidget(valLbl, 1);
        body->addLayout(row);
    }
    root->addLayout(body);
    root->addStretch();

    // ═══════════════════════════════════════════════
    //  关闭按钮
    // ═══════════════════════════════════════════════
    auto *btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(24, 8, 24, 20);

    auto *closeBtn = new QPushButton("关闭", this);
    closeBtn->setFixedWidth(80);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #BBADA0;"
        "  color: #FFFFFF;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 8px 0px;"
        "  font-size: 11pt;"
        "  font-weight: bold;"
        "  font-family: 'Microsoft YaHei';"
        "}"
        "QPushButton:hover { background-color: #9C8D7B; }"
        "QPushButton:pressed { background-color: #8D7B69; }"
    );
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    root->addLayout(btnLayout);
}
