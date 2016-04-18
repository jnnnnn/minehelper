#pragma once
#include "MineGrid.h"
#include <functional>

// Receives a grid from the recognizer and works out which squares would be good
// to click.
class Solver {
public:
  struct Neighbours {
    BYTE mines = 0;
    BYTE cleared = 0;
    BYTE unclickedUndetermined = 0;
    BYTE unclickedMines = 0;
    BYTE unclickedClear = 0;
  };

  enum SolveResult { NoChanges = 0, Valid = 1, Invalid = 2 };

  static void ForEachNeighbour(int x, int y,
                               std::function<void(int x, int y)> f);
  static void Solve(MineGrid &grid);

  template <typename TGrid>
  static Neighbours Solver::SumNeighbours(TGrid &grid, int x0, int y0) {
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

  // monkey solver
  template <typename TGrid>
  static SolveResult SimpleSolve(TGrid &grid, int x0, int x1, int y0, int y1);
};

template <typename TGrid>
static Solver::SolveResult Solver::SimpleSolve(TGrid &grid, int x0, int x1,
                                               int y0, int y1) {
  SolveResult result = NoChanges;
  for (int x = x0; x <= x1; x++) {
    for (int y = y0; y <= y1; y++) {
      auto neighbours = SumNeighbours(grid, x, y);
      MineGrid::CellValue c = grid.GetCell(x, y);
      unsigned char possible_mines = neighbours.mines +
                                     neighbours.unclickedMines +
                                     neighbours.unclickedUndetermined;
      bool bFullInfo =
          8 == neighbours.cleared + neighbours.unclickedClear + possible_mines;
      if (bFullInfo && c <= MineGrid::Clear8) {
        bool bNotEnoughMines = possible_mines < c;
        bool bTooManyMines = c < neighbours.mines + neighbours.unclickedMines;
        if (bNotEnoughMines || bTooManyMines)
          return Invalid;
      }
      if (neighbours.unclickedUndetermined > 0 &&
          neighbours.unclickedUndetermined < 8 && bFullInfo) {
        if (neighbours.mines + neighbours.unclickedMines == c) {
          ForEachNeighbour(x, y, [&](int xn, int yn) {
            MineGrid::CellValue cn = grid.GetCell(xn, yn);
            if (cn == MineGrid::Unclicked || cn == MineGrid::UnclickedBorder) {
              result = Valid;
              grid.SetCell(xn, yn, MineGrid::UnclickedClear);
            }
          });
        } else if ((possible_mines) == c) {
          ForEachNeighbour(x, y, [&](int xn, int yn) {
            MineGrid::CellValue cn = grid.GetCell(xn, yn);
            if (cn == MineGrid::Unclicked || cn == MineGrid::UnclickedBorder) {
              result = Valid;
              grid.SetCell(xn, yn, MineGrid::UnclickedMine);
            }
          });
        } else {
          if ((c == MineGrid::Unclicked || c == MineGrid::UnclickedBorder) &&
              neighbours.cleared > 0) {
            grid.SetCell(x, y, MineGrid::UnclickedBorder);
          }
        }
      }
    }
  }
  return result;
}