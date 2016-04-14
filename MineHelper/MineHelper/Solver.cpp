#include "stdafx.h"
#include "Solver.h"
#include <functional>
#include <set>
#include <utility>
#include <deque>

// monkey solver
void Solver::SimpleSolve(MineGrid &grid) {
  for (int iterations = 0; iterations < 4; iterations++) {
    for (int x = 1; x < grid.width - 1; x++) {
      for (int y = 1; y < grid.height - 1; y++) {
        auto neighbours = SumNeighbours(grid, x, y);
        MineGrid::CellValue c = grid.GetCell(x, y);
        if (neighbours.unclickedUndetermined > 0 &&
            neighbours.unclickedUndetermined < 8 &&
            neighbours.cleared + neighbours.unclickedClear + neighbours.mines +
                    neighbours.unclickedMines +
                    neighbours.unclickedUndetermined ==
                8) {
          if (neighbours.mines + neighbours.unclickedMines == c) {
            ForEachNeighbour(x, y, [&](int xn, int yn) {
              if (grid.IsUnsolved(xn, yn))
                grid.SetCell(xn, yn, MineGrid::UnclickedClear);
            });
          } else if ((neighbours.unclickedUndetermined + neighbours.mines +
                      neighbours.unclickedMines) == c) {
            ForEachNeighbour(x, y, [&](int xn, int yn) {
              if (grid.IsUnsolved(xn, yn))
                grid.SetCell(xn, yn, MineGrid::UnclickedMine);
            });
          } else if (grid.IsUnsolved(x, y) && neighbours.cleared > 0) {
            grid.SetCell(x, y, MineGrid::UnclickedBorder);
          }
        }
      }
    }
  }
}

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

Solver::Neighbours Solver::SumNeighbours(MineGrid &grid, int x0, int y0) {
  Neighbours result;
  ForEachNeighbour(x0, y0, [&](int x, int y) {
    MineGrid::CellValue c = grid.GetCell(x, y);
    result.mines += c == MineGrid::Mine;
    result.unclickedMines += c == MineGrid::UnclickedMine;
    result.unclickedClear += c == MineGrid::UnclickedClear;
    result.cleared += c <= MineGrid::Clear8;
    result.unclickedUndetermined +=
        c == MineGrid::Unclicked || c == MineGrid::UnclickedBorder;
  });
  return result;
}

void Solver::Solve(MineGrid &grid) {
  // Get the easy stuff out of the way
  SimpleSolve(grid);

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

  for (auto &group : borderGroups) {
    for (auto &cellCoord : group) {

    }
  }
}
