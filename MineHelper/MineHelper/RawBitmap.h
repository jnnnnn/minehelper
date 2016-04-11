#pragma once
#include <algorithm>

struct RawBitmap {

  int Width = 0;
  int Height = 0;

  std::unique_ptr<BYTE[]> bits;

  inline int PosB(int x, int y) const { return bits[4 * ((y * Width) + x)]; }
  inline int PosG(int x, int y) const {
    return bits[4 * ((y * Width) + x) + 1];
  }
  inline int PosR(int x, int y) const {
    return bits[4 * ((y * Width) + x) + 2];
  }
  inline int PosA(int x, int y) const {
    return bits[4 * ((y * Width) + x) + 3];
  }
  inline int PosGray(int x, int y) const {
    return (PosB(x, y) + PosG(x, y) + PosR(x, y)) / 3;
  }

  inline void SetB(int x, int y, BYTE v) { bits[4 * ((y * Width) + x)] = v; }
  inline void SetG(int x, int y, BYTE v) { bits[4 * ((y * Width) + x) + 1] = v; }
  inline void SetR(int x, int y, BYTE v) { bits[4 * ((y * Width) + x) + 2] = v; }
  inline void SetA(int x, int y, BYTE v) { bits[4 * ((y * Width) + x) + 3] = v; }
  inline void SetGray(int x, int y, BYTE v) {
    SetB(x, y, v);
    SetG(x, y, v);
    SetR(x, y, v);
    SetA(x, y, 255);
  }

  // see if two bitmaps are the same
  bool Matches(int x0, int x1, int y0, int y1, const RawBitmap &other,
               int otherx0, int othery0) {
    int xrange = min(Width - x0, x1 - x0);
    xrange = min(xrange, other.Width - otherx0);
    int yrange = min(Height - y0, y1 - y0);
    yrange = min(yrange, other.Height - othery0);

    for (int x = 0; x < xrange; x++) {
      for (int y = 0; y < yrange; y++) {
        if (PosA(x0 + x, y0 + y) == 0 ||
            other.PosA(otherx0 + x, othery0 + y) == 0)
          continue;
        if (PosGray(x0 + x, y0 + y) != other.PosGray(otherx0 + x, othery0 + y))
          return false;
      }
    }
    return true;
  }
};