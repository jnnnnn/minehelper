#pragma once

#include <vector>
#include <unordered_map>

extern const int GRID_SIZE;

typedef std::pair<short, short> CellCoord;
typedef std::pair<short, short> BitmapCoord;

namespace std {
template <> struct hash<CellCoord> {
  std::size_t operator()(const CellCoord &k) const {
    return std::hash<int>()((k.first << (sizeof(short) * 8)) + k.second);
  }
};
}

struct MineGrid {
  enum CellValue {
    Clear0 = 0,
    Clear1,
    Clear2,
    Clear3,
    Clear4,
    Clear5,
    Clear6,
    Clear7,
    Clear8,
    Mine,
    UnclickedMine,
    UnclickedClear,
    Unclicked,
    UnclickedBorder,
    Unknown,
  };
  void SetCell(int x, int y, CellValue v) {
    cells[x * 100 + y] = v;
    if (v != Unknown) {
      width = max(x, width);
      height = max(y, height);
    }
  }
  CellValue GetCell(int x, int y) const { return cells[x * 100 + y]; }

  CellValue cells[10000]; // 100x100. Normal screen is 60x50.
  int offsetx = -1;
  int offsety = -1;
  int width = 0;    // number of cells across which are filled in
  int height = 0;   // number of cells up which are filled in
  int widthpx = 0;  // original size of the window this grid was resolved from
  int heightpx = 0; // original size of the window this grid was resolved from
  void PrintGrid() const;
};

// a cheap-copy clone of a MineGrid, used to explore possible solutions.
struct ModifiedGrid {
  ModifiedGrid(const MineGrid &baseGrid) : baseGrid(baseGrid) {}
  const MineGrid &baseGrid;
  std::unordered_map<std::pair<short, short>, MineGrid::CellValue>
      modifiedCells;

  bool IsModified() { return modifiedCells.size() > 0; }

  void SetCell(int x, int y, MineGrid::CellValue v) {
    modifiedCells[{x, y}] = v;
  }

  MineGrid::CellValue GetCell(int x, int y) const {
    auto it = modifiedCells.find({x, y});
    if (it != modifiedCells.end())
      return it->second;
    else
      return baseGrid.GetCell(x, y);
  }
};
