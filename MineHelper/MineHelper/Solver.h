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
    BYTE unclicked = 0;
	BYTE notmines = 0;
  };

  static void Solve(MineGrid &grid);
protected:
	static void SimpleSolve(MineGrid & grid);
	static void ForEachNeighbour(int x, int y, std::function<void(int x, int y)> f);
	static Neighbours SumNeighbours(MineGrid &grid, int x0, int y0);
};
