#pragma once

#include <QDialog>
#include <QPushButton>
#include <QSet>
#include <QSettings>
#include <vector>

class PeriodicTable : public QDialog
{
    Q_OBJECT
public:
    explicit PeriodicTable(QWidget *parent = nullptr);

    void addDiscoveries(const std::vector<int> &atomicNumbers);

signals:
    void elementClicked(int atomicNumber);

private:
    void loadDiscoveries();
    void saveDiscoveries();
    void updateCell(int atomicNumber);
    void buildUI();
    QPushButton* cellFor(int atomicNumber) const;

    QSet<int>          discoveredSet;
    QVector<QPushButton*> cells;   // index by atomicNumber (0 unused)
};

