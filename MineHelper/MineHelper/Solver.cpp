#include "stdafx.h"
#include "Solver.h"
#include <functional>
#include <set>
#include <utility>
#include <deque>

void Solver::ForEachNeighbour(int x, int y,
                              std::function<void(int x, int y)> f) {
  f(x - 1, y - 1);
  f(x - 1, y - 0);
  f(x - 1, y + 1);
  f(x - 0, y - 1);
  //	f(x - 0, y - 0);
  f(x - 0, y + 1);
  f(x + 1, y - 1);
  f(x + 1, y - 0);
  f(x + 1, y + 1);
}

void Solver::Solve(MineGrid &grid) {
  // Get the easy stuff out of the way
  for (int i = 0; i < 5; i++) {
    SolveResult r = SimpleSolve(grid, 1, grid.width - 2, 1, grid.height - 2);
    if (r != Solver::Valid)
      break;
  }

  // attempt brute force on the unsolved cells
  typedef std::set<CellCoord> BorderGroup;
  BorderGroup borderCells;
  for (int x = 1; x < grid.width - 1; x++)
    for (int y = 1; y < grid.height - 1; y++)
      if (grid.GetCell(x, y) == MineGrid::UnclickedBorder)
        borderCells.insert(CellCoord(x, y));

  // separate unsolved cells into groups
  std::vector<BorderGroup> borderGroups;
  const size_t MAX_GROUP_SIZE = 8;
  while (borderCells.size() > 0) {
    std::deque<CellCoord> stack;
    BorderGroup group;
    auto firstCell = *borderCells.begin();
    borderCells.erase(firstCell);
    stack.push_back(firstCell);
    while (stack.size()) {
      // next node is oldest because breadth-first search groups lines better
      CellCoord currentCell = stack[0];
      stack.pop_front();
      group.insert(currentCell);

      auto matcher = [&](int x, int y) {
        auto neighbour = CellCoord(x, y);
        if (grid.GetCell(x, y) == MineGrid::UnclickedBorder &&
            borderCells.find(neighbour) != borderCells.end() &&
            group.size() + stack.size() < MAX_GROUP_SIZE) {
          borderCells.erase(neighbour);
          stack.push_back(neighbour);
        };
      };

      ForEachNeighbour(currentCell.first, currentCell.second, matcher);
    }
    borderGroups.push_back(group);
  }

  //return;

  for (auto &group : borderGroups) {
    for (auto &cellCoord : group) {
      // add a mine, extrapolate 2 times, check validity
      ModifiedGrid g(grid);
      g.SetCell(cellCoord.first, cellCoord.second, MineGrid::UnclickedMine);

      for (int i = 0; i < 2; i++) {
        SolveResult r =
            SimpleSolve(g, max(1, cellCoord.first - 1 - 2 * i),
                        min(grid.width - 2, cellCoord.first + 1 + 2 * i),
                        max(1, cellCoord.second - 1 - 2 * i),
                        min(grid.height - 2, cellCoord.second + 1 + 2 * i));
        if (r == Solver::Invalid)
          grid.SetCell(cellCoord.first, cellCoord.second,
                       MineGrid::UnclickedClear);
        if (r == Solver::NoChanges)
          break;
      }
      // add a clear space, extrapolate 3 times, checking validity
      ModifiedGrid g2(grid);
      g2.SetCell(cellCoord.first, cellCoord.second, MineGrid::UnclickedClear);

      for (int i = 0; i < 2; i++) {
        SolveResult r =
            SimpleSolve(g2, max(1, cellCoord.first - 1 - 2 * i),
                        min(grid.width - 2, cellCoord.first + 1 + 2 * i),
                        max(1, cellCoord.second - 1 - 2 * i),
                        min(grid.height - 2, cellCoord.second + 1 + 2 * i));
        if (r == Solver::Invalid)
          grid.SetCell(cellCoord.first, cellCoord.second,
                       MineGrid::UnclickedMine);
        if (r == Solver::NoChanges)
          break;
      }
    }
  }
}
