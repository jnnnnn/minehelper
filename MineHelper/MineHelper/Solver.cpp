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
      result.mines += grid.GetCell(x, y) == MineGrid::Mine;
      result.cleared += grid.GetCell(x, y) <= MineGrid::Clear8;
      result.unknown += grid.GetCell(x, y) == MineGrid::Unclicked;
    });
    return result;
  };

  for (int x = 1; x < grid.width - 1; x++) {
    for (int y = 1; y < grid.height - 1; y++) {
      auto neighbours = SumNeighbours(x, y);
      MineGrid::Cell c = grid.GetCell(x, y);
      if (neighbours.unknown > 0) {
        if (neighbours.mines == c) {
          grid.SetCell(x, y, MineGrid::ClickGreen);
        } else if ((neighbours.unknown + neighbours.mines) == c) {
          grid.SetCell(x, y, MineGrid::ClickRed);
        }
      }
    }
  }
}
