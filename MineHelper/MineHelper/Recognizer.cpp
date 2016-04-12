#include "stdafx.h"
#include "Recognizer.h"

Recognizer::Recognizer() {
  GridCorner.reset(new RawBitmap());
  GridCorner->Width = 4;
  GridCorner->Height = 4;
  GridCorner->bits.reset(new BYTE[4 * 4 * 4]);
  GridCorner->SetA(0, 0, 0);
  GridCorner->SetGray(0, 1, 0);
  GridCorner->SetGray(0, 2, 0);
  GridCorner->SetA(0, 3, 0);
  GridCorner->SetGray(1, 0, 0);
  GridCorner->SetGray(1, 1, 255);
  GridCorner->SetGray(1, 2, 255);
  GridCorner->SetGray(1, 3, 0);
  GridCorner->SetGray(2, 0, 0);
  GridCorner->SetGray(2, 1, 255);
  GridCorner->SetGray(2, 2, 255);
  GridCorner->SetGray(2, 3, 0);
  GridCorner->SetA(3, 0, 0);
  GridCorner->SetGray(3, 1, 0);
  GridCorner->SetGray(3, 2, 0);
  GridCorner->SetA(3, 3, 0);
}

shared_ptr<MineGrid> Recognizer::Recognize(shared_ptr<RawBitmap> bitmap) {

  auto result = shared_ptr<MineGrid>(new MineGrid());

  result->heightpx = bitmap->Height;
  result->widthpx = bitmap->Width;

  // start in the middle
  int x0 = bitmap->Width / 2;
  int y0 = bitmap->Height / 2;

  for (int x = x0; x < x0 + GRID_SIZE; x++) {
    for (int y = y0; y < y0 + GRID_SIZE; y++) {
      int o = GRID_SIZE * 0;
      if (!bitmap->Matches(x + o, x + o + GridCorner->Width, y + o,
                           y + o + GridCorner->Height, *GridCorner, 0, 0))
        continue;
      o = GRID_SIZE * 1;
      if (!bitmap->Matches(x + o, x + o + GridCorner->Width, y + o,
                           y + o + GridCorner->Height, *GridCorner, 0, 0))
        continue;
      o = GRID_SIZE * 2;
      if (!bitmap->Matches(x + o, x + o + GridCorner->Width, y + o,
                           y + o + GridCorner->Height, *GridCorner, 0, 0))
        continue;
      o = GRID_SIZE * 3;
      if (!bitmap->Matches(x + o, x + o + GridCorner->Width, y + o,
                           y + o + GridCorner->Height, *GridCorner, 0, 0))
        continue;

      // offset is difference between 0, 0 and bottom-left pixel of a square's
      // border.
      result->offsetx = (x + 1) % GRID_SIZE;
      result->offsety = (y + 1) % GRID_SIZE;
      break;
    }
  }

  cout << result->offsetx << ", " << result->offsety << "\r\n";
  
  if (result->offsetx == -1)
	  return result;

  // match each cell
  int width = (bitmap->Width - result->offsetx - 17) / GRID_SIZE;
  int height = (bitmap->Height - result->offsety - 15) / GRID_SIZE;
  for (int cellx = 0; cellx < width; cellx++) {
    for (int celly = 0; celly < height; celly++) {
      int gray = bitmap->PosGray(cellx * GRID_SIZE + result->offsetx + 17,
                                 celly * GRID_SIZE + result->offsety + 17);
      /*
          bitmap->SavePartialBitmap("cell x.bmp",
                                cellx * GRID_SIZE + result->offsetx,
                                (cellx + 1) * GRID_SIZE + result->offsetx,
                                celly * GRID_SIZE + result->offsety,
                                (celly + 1) * GRID_SIZE + result->offsety);
      */
      MineGrid::Cell cell;
      switch (gray) {
      case 219:
        cell = MineGrid::Unclicked;
        break;
      case 97:
	  case 0:
        cell = MineGrid::Mine;
        break;
      case 255:
        cell = MineGrid::Clear0;
        break;
      case 204:
        cell = MineGrid::Clear1;
        break;
      case 170:
        cell = MineGrid::Clear2;
        break;
      case 136:
        cell = MineGrid::Clear3;
        break;
      case 102:
        cell = MineGrid::Clear4;
        break;
      case 68:
        cell = MineGrid::Clear5;
        break;
      default:
        cell = MineGrid::Unknown;
      }
      result->SetCell(cellx, celly, cell);
    }
  }
  return result;
}
