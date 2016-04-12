#include "stdafx.h"
#include "MineGrid.h"

const int GRID_SIZE = 32;

void MineGrid::PrintGrid() const {
  cout << "\r\n";
  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      Cell cell = GetCell(x, y);
      if (cell < 9)
        cout << GetCell(x, y);
      else if (cell == Mine)
        cout << "!";
      else if (cell == Unclicked)
        cout << ".";
      else if (cell == ClickGreen)
        cout << "L";
      else if (cell == ClickRed)
        cout << "R";
      else
        cout << "?";
    }
    cout << "\r\n";
  }
}
