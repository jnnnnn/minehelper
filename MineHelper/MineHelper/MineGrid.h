#pragma once

#include <vector>

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
    Unclicked,
    Unknown,
    ClickGreen,
    ClickRed
  };

  void SetCell(int x, int y, Cell v) {
    cells[x * 100 + y] = v;
    if (v != Unknown) {
      width = max(x, width);
      height = max(y, height);
    }
  }
  Cell GetCell(int x, int y) const { return cells[x * 100 + y]; }

  Cell cells[10000]; // 100x100. Normal screen is 60x50.
  int offsetx = -1;
  int offsety = -1;
  int width = 0;  // number of cells across which are filled in
  int height = 0; // number of cells up which are filled in

  void PrintGrid() const;
};
