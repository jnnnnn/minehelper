#include "stdafx.h"
#include "Outputter.h"
#include "Screen.h"

void Outputter::Output(MineGrid &grid) {
  shared_ptr<RawBitmap> overlay(new RawBitmap());
  overlay->Clear(grid.widthpx, grid.heightpx, 255);
  for (int x = 1; x < grid.width; x++) {
    for (int y = 1; y < grid.height; y++) {

      int xpx = x * GRID_SIZE + grid.offsetx + GRID_SIZE / 2;
      int ypx = y * GRID_SIZE + grid.offsety + GRID_SIZE / 2;

      if (grid.GetCell(x, y) == MineGrid::UnclickedClear) {
        overlay->DrawBox(xpx, ypx, 12, 0, 255, 0);
      } else if (grid.GetCell(x, y) == MineGrid::UnclickedMine) {
        overlay->DrawBox(xpx, ypx, 12, 255, 0, 0);
      } else if (grid.GetCell(x, y) == MineGrid::UnclickedBorder) {
        overlay->DrawBox(xpx, ypx, 12, 0, 0, 255);
      }
    }
  }

  HDC hdcWindow;
  HDC hdcMemDC = NULL;
  HBITMAP hbmWindow = NULL;

  hdcWindow = GetDC(hwndTarget);
  hdcMemDC = CreateCompatibleDC(hdcWindow);
  if (!hdcMemDC)
    goto done;

  hbmWindow =
      CreateCompatibleBitmap(hdcWindow, overlay->Width, overlay->Height);
  if (!hbmWindow)
    goto done;

  BITMAPINFOHEADER bi;
  bi.biBitCount = 32;
  bi.biClrImportant = 0;
  bi.biClrUsed = 0;
  bi.biCompression = BI_RGB;
  bi.biHeight = overlay->Height;
  bi.biPlanes = 1;
  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biSizeImage = 0;
  bi.biWidth = overlay->Width;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;

  SetDIBits(hdcWindow, hbmWindow, 0, overlay->Height, overlay->bits.get(),
            (BITMAPINFO *)&bi, DIB_RGB_COLORS);

  HGDIOBJ hOld = SelectObject(hdcMemDC, hbmWindow);
  BitBlt(hdcWindow, 0, 0, overlay->Width, overlay->Height, hdcMemDC, 0, 0,
         SRCAND);
  SelectObject(hdcMemDC, hOld);

done:
  DeleteObject(hbmWindow);
  DeleteObject(hdcMemDC);
  ReleaseDC(hwndTarget, hdcWindow);
}
