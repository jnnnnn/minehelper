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
    BYTE unknown = 0;
  };

  static void Solve(MineGrid &grid);
};
