#pragma once

#include <vector>

extern const int GRID_SIZE;

struct MineGrid {
  enum Cell {
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

  void SetCell(int x, int y, Cell v) {
    cells[x * 100 + y] = v;
    if (v != Unknown) {
      width = max(x, width);
      height = max(y, height);
    }
  }
  Cell GetCell(int x, int y) const { return cells[x * 100 + y]; }
  bool IsUnsolved(int x, int y) const {
    Cell c = GetCell(x, y);
    return c == Unclicked || c == UnclickedBorder;
  }
  Cell cells[10000]; // 100x100. Normal screen is 60x50.
  int offsetx = -1;
  int offsety = -1;
  int width = 0;    // number of cells across which are filled in
  int height = 0;   // number of cells up which are filled in
  int widthpx = 0;  // original size of the window this grid was resolved from
  int heightpx = 0; // original size of the window this grid was resolved from
  void PrintGrid() const;
};
