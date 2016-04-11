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

  void SetCell(int x, int y, Cell v) { cells[x * 100 + y] = v; }
  Cell GetCell(int x, int y) const { return cells[x * 100 + y]; }

  Cell cells[10000]; // 100x100
  int offsetx = -1;
  int offsety = -1;

  void PrintGrid() {
    cout << "\r\n";
    for (int y = 40; y >= 0; y--) {
      for (int x = 0; x < 40; x++) {
        Cell cell = GetCell(x, y);
        if (cell < 9)
          cout << GetCell(x, y);
        else if (cell == Mine)
          cout << "!";
        else if (cell == Unclicked)
          cout << ".";
        else
          cout << "?";
      }
      cout << "\r\n";
    }
  }
};
