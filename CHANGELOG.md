# Changelog

## [Unreleased] — 2026-06-03

### Added — 元素周期表 (Periodic Table)
- **周期表窗口**: 18列×4行真周期表布局，非模态窗口，可通过 查看→元素周期表 (Ctrl+T) 打开
- **元素发现系统**: 合成新元素时自动发现并高亮，H/He 预设为已发现
- **元素详情弹窗**: 点击已发现元素弹出模态卡片（中文名/英文名/原子量/类别/电子排布）
- **持久化**: 发现进度通过 QSettings 保存，关闭应用后不丢失
- **共享数据层** (`ElementData.h`): 21条元素数据（符号/中英文名/类别/电子排布/颜色），Board 和 BoardWidget 统一引用

### Changed — GUI 统一优化
- **菜单栏**: 背景统一为 `#BBADA0`，白字加粗，底部 2px 深色边框
- **下拉菜单**: 边框加粗至 2px，与菜单栏色系统一
- **分数卡片**: 圆角 8→12px，新增柔和阴影和边框
- **棋盘**: 新增 2px 深色边框和阴影
- **按钮**: 添加 Unicode 图标前缀 (↩ 撤销, ⟳ 重新开始)
- **按钮样式作用域**: QPushButton 样式限制在右侧面板，不影响 QMessageBox 对话框按钮

### Files
| 新建 | 修改 |
|------|------|
| `ElementData.h` | `Board.h`, `Board.cpp` |
| `PeriodicTable.h`, `.cpp` | `Game2048.h`, `Game2048.cpp` |
| `ElementDialog.h`, `.cpp` | `mainwindow.h`, `mainwindow.cpp` |
| | `boardwidget.cpp` |
| | `CMakeLists.txt` |
