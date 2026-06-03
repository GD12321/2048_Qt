# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

```bash
# Configure (from repo root, using Qt6 MinGW on Windows)
"D:\Qt\Tools\CMake_64\bin\cmake.exe" -B build -G "MinGW Makefiles" \
  -DCMAKE_PREFIX_PATH="D:\Qt\6.11.0\mingw_64" \
  -DCMAKE_CXX_COMPILER="D:\Qt\Tools\mingw1310_64\bin\g++.exe" \
  -DCMAKE_MAKE_PROGRAM="D:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe"

# Build
cmake --build build

# Run (Qt DLLs must be on PATH)
export PATH="D:\Qt\6.11.0\mingw_64\bin;D:\Qt\Tools\mingw1310_64\bin;$PATH"
./build/2048.exe
```

Qt6 Widgets, C++17. No tests or linter configured.

## Architecture

Five-layer design:

| Layer | File | Role |
|---|---|---|
| Shared data | `ElementData.h` | Static `kElementDetails[21]` (0=empty, 1=H … 20=Ca). Each entry: symbol, Chinese/English name, mass, category, electron config, tile color. Single source of truth — Board, BoardWidget, PeriodicTable, and ElementDialog all reference this table. |
| Data | `Board.h/.cpp` | 4×4 grid of atomic numbers. Pure logic: `slideAndMerge()` per row/column with merged-element tracking (`mergedList`), random tile spawn (H 90%, He 10%), `canMove()` check. `elementSymbol()` and `elementMass()` delegate to `ElementData.h`. |
| Game state | `Game2048.h/.cpp` | Wraps `Board` with score, single-level undo (`memcpy` snapshot before each move), high-score persistence via `QSettings("MySoft", "2048")`. Exposes `lastMerged` (newly created elements this move) for the periodic table. |
| UI | `mainwindow.h/.cpp` | Left-right layout: board widget (fixed 420×420) + right panel (140px). Right panel: two score cards (`QFrame`), undo/restart buttons. Menu bar: 游戏 / 查看 / 帮助. Keyboard: arrows+WASD → `Direction`; Ctrl+Z → undo; Ctrl+T → periodic table. Window fixed 600×480. |
| Rendering | `boardwidget.h/.cpp` | `QPainter` widget. Tile colors from `kElementDetails[].tileColor`. Rounded rect (`borderRadius=8`) with margin `cellSide/16`. Element symbol bold (large), mass below (small). Light tiles (H, He) get dark text; darker tiles get white text. |
| Periodic table | `PeriodicTable.h/.cpp` | Non-modal QDialog, 18-column × 4-row authentic layout. Tracks discovered elements via `QSet<int>` persisted to `QSettings`. Undiscovered cells show atomic number in gray; discovered cells light up in element color. Click → opens `ElementDialog`. |
| Element popup | `ElementDialog.h/.cpp` | Modal QDialog. Colored header bar (symbol + atomic number + Chinese name), detail fields (English name, mass, category, electron config), "关闭" button. |

**Entry point:** `main.cpp` — seeds `rand()`, creates `QApplication`, shows `MainWindow`.

**Data flow:** Key press → `MainWindow::handleMove(Direction)` → `Game2048::move()` → `Board::move()` → `Board::slideAndMerge(mergedList)` → `MainWindow::updateUI()` → `BoardWidget::paintEvent()`, plus `periodicTable->addDiscoveries(game.getLastMerged())`.

## Styling

All styling lives in `MainWindow::setupStyle()` via Qt stylesheet (`setStyleSheet`). Key patterns:

- **Background**: `#FAF8EF` (warm off-white, classic 2048 feel)
- **Menu bar**: `#BBADA0` background, white bold text, `border-bottom: 2px solid #9C8D7B`. Item hover: `#9C8D7B`.
- **Dropdown menus**: `#FAF8EF` background, `2px solid #BBADA0` border
- **Score cards**: `#scoreCard` with `#BBADA0` bg, `border: 1px solid #C8BBA8`, `border-radius: 12px`, plus `QGraphicsDropShadowEffect`
- **Board**: `#boardFrame` with `border: 2px solid #9C8D7B`, `border-radius: 6px`, plus `QGraphicsDropShadowEffect`
- **Buttons**: Scoped to `#rightPanel QPushButton` (avoids leaking into QMessageBox). `#BBADA0` bg → `#9C8D7B` hover → `#8D7B69` press. Disabled: `#D6CDC4`. Undo/restart use Unicode icon prefixes (`↩ 撤销`, `⟳ 重新开始`).
- **Font**: Microsoft YaHei throughout — 10pt card titles, 18pt bold card values, 11pt bold buttons

## Element theme

Grid cells are chemical elements, not numbers. Two identical elements merge into the next (H + H → He, He + He → Li). New tiles: H (90%) or He (10%). The shared element table (`kElementDetails[]` in `ElementData.h`) defines 21 entries (0=empty, 1=H through 20=Ca); the grid stores the atomic number index.

**Element categories:** Nonmetal (非金属), NobleGas (贵气体), AlkaliMetal (碱金属), AlkalineEarth (碱土金属), Metalloid (类金属), Halogen (卤素), PostTransition (后过渡金属).

**Discovery:** Merged elements are tracked in `Game2048::lastMerged` and passed to `PeriodicTable::addDiscoveries()`. H and He are pre-discovered. Persisted via QSettings key `"discoveries"`.

## Undo model

Single-level: `Game2048::saveUndoState()` copies the grid via `memcpy` + saves `score` before each successful move. `undo()` restores via `Board::setGrid()`. Undo state is cleared after restoring (no redo).

## Agent skills

### Issue tracker

GitHub Issues on `GD12321/2048_Qt` (via `gh` CLI). See `docs/agents/issue-tracker.md`.

### Triage labels

Uses the five canonical labels: `needs-triage`, `needs-info`, `ready-for-agent`, `ready-for-human`, `wontfix`. See `docs/agents/triage-labels.md`.

### Domain docs

Single-context — one `CONTEXT.md` + `docs/adr/` at the repo root. See `docs/agents/domain.md`.
