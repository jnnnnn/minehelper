#pragma once

#include <vector>

struct MineGrid
{
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
    Unknown,
    Mine,
	ClickGreen,
	ClickRed
  };

  Cell cells[10000]; // 100x100
};

