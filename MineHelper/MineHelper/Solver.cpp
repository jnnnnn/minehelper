#include "stdafx.h"
#include "Solver.h"
#include <functional>

void Solver::Solve(MineGrid &grid) {

  auto ForEachNeighbour = [&](int x0, int y0,
                              std::function<void(int x, int y)> f) {
    for (int y = y0 - 1; y <= y0 + 1; y++) {
      for (int x = x0 - 1; x <= x0 + 1; x++) {
        if (x == x0 && y == y0)
          continue;
        f(x, y);
      }
    }
  };

  auto SumNeighbours = [&](int x0, int y0) -> Neighbours {
    Neighbours result;
    ForEachNeighbour(x0, y0, [&](int x, int y) {
      MineGrid::Cell c = grid.GetCell(x, y);
      result.mines += c == MineGrid::Mine || c == MineGrid::UnclickedMine;
      result.cleared += c <= MineGrid::Clear8 || c == MineGrid::UnclickedClear;
      result.unknown += c == MineGrid::Unclicked;
    });
    return result;
  };

  for (int iterations = 0; iterations < 4; iterations++) {
    for (int x = 1; x < grid.width - 1; x++) {
      for (int y = 1; y < grid.height - 1; y++) {
        auto neighbours = SumNeighbours(x, y);
        MineGrid::Cell c = grid.GetCell(x, y);
        if (neighbours.unknown > 0) {
          if (neighbours.mines == c) {
            ForEachNeighbour(x, y, [&](int xn, int yn) {
              if (grid.GetCell(xn, yn) == MineGrid::Unclicked)
                grid.SetCell(xn, yn, MineGrid::UnclickedClear);
            });
          } else if ((neighbours.unknown + neighbours.mines) == c) {
            ForEachNeighbour(x, y, [&](int xn, int yn) {
              if (grid.GetCell(xn, yn) == MineGrid::Unclicked)
                grid.SetCell(xn, yn, MineGrid::UnclickedMine);
            });
          }
        }
      }
    }
  }
}
