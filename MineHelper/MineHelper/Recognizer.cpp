#include "stdafx.h"
#include "Recognizer.h"

Recognizer::Recognizer() {
  GridIntersect.reset(new RawBitmap());
  GridIntersect->Width = 4;
  GridIntersect->Height = 4;
  GridIntersect->bits.reset(new BYTE[4 * 4 * 4]);
  GridIntersect->SetA(0, 0, 0);
  GridIntersect->SetGray(0, 1, 0);
  GridIntersect->SetGray(0, 2, 0);
  GridIntersect->SetA(0, 3, 0);
  GridIntersect->SetGray(1, 0, 0);
  GridIntersect->SetGray(1, 1, 255);
  GridIntersect->SetGray(1, 2, 255);
  GridIntersect->SetGray(1, 3, 0);
  GridIntersect->SetGray(2, 0, 0);
  GridIntersect->SetGray(2, 1, 255);
  GridIntersect->SetGray(2, 2, 255);
  GridIntersect->SetGray(2, 3, 0);
  GridIntersect->SetA(3, 0, 0);
  GridIntersect->SetGray(3, 1, 0);
  GridIntersect->SetGray(3, 2, 0);
  GridIntersect->SetA(3, 3, 0);
}

shared_ptr<MineGrid> Recognizer::Recognize(shared_ptr<RawBitmap> bitmap) {
  auto result = shared_ptr<MineGrid>(new MineGrid());

  // find the grid....
  int offsetx = -1, offsety = -1;

  // start in the middle
  int x0 = bitmap->Width / 2;
  int y0 = bitmap->Height / 2;

  const int GRID_SIZE = 32;

  for (int x = x0; x < x0 + GRID_SIZE; x++) {
    for (int y = y0; y < y0 + GRID_SIZE; y++) {
      if (bitmap->Matches(x, x + GridIntersect->Width, y,
                          y + GridIntersect->Height, *GridIntersect, 0, 0)) {
		  offsetx = x % GRID_SIZE;
		  offsety = y % GRID_SIZE;
			break;
      }
    }
  }

  cout << offsetx << offsety;

  // match each cell
  return nullptr;
}
