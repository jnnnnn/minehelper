#pragma once

// In this structure's coordinate system, (0,0) is bottom left. So increasing y
// goes up the screen and increasing x goes across to the right.
struct RawBitmap {

  int Width = 0;
  int Height = 0;

  std::unique_ptr<BYTE[]> bits;

  void Clear(int width, int height, BYTE gray = 0) {
    Width = width;
    Height = height;
    bits.reset(new BYTE[4 * width * height]);
    memset(bits.get(), gray, sizeof(BYTE) * 4 * width * height);
  }

  int PosB(int x, int y) const { return bits[4 * ((y * Width) + x)]; }
  int PosG(int x, int y) const { return bits[4 * ((y * Width) + x) + 1]; }
  int PosR(int x, int y) const { return bits[4 * ((y * Width) + x) + 2]; }
  int PosA(int x, int y) const { return bits[4 * ((y * Width) + x) + 3]; }
  int PosGray(int x, int y) const {
    return (PosB(x, y) + PosG(x, y) + PosR(x, y)) / 3;
  }

  void DrawBox(int cx, int cy, int radius, int r, int g, int b) {
    for (int x = cx - radius; x < cx + radius; x++) {
      SetPx(x, cy - radius, r, g, b);
      SetPx(x, cy + radius, r, g, b);
    }
    for (int y = cy - radius; y < cy + radius; y++) {
      SetPx(cx - radius, y, r, g, b);
      SetPx(cx + radius, y, r, g, b);
    }
  }

  void DrawSolid(int cx, int cy, int radius, int r, int g, int b) {
    for (int x = cx - radius; x < cx + radius; x++) {
      for (int y = cy - radius; y < cy + radius; y++) {
        SetPx(x, y, r, g, b);
      }
    }
  }

  void SetPx(int x, int y, int r, int g, int b) {
    SetB(x, y, b);
    SetG(x, y, g);
    SetR(x, y, r);
    SetA(x, y, 255);
  }

  void SetB(int x, int y, BYTE v) { bits[4 * ((y * Width) + x)] = v; }
  void SetG(int x, int y, BYTE v) { bits[4 * ((y * Width) + x) + 1] = v; }
  void SetR(int x, int y, BYTE v) { bits[4 * ((y * Width) + x) + 2] = v; }
  void SetA(int x, int y, BYTE v) { bits[4 * ((y * Width) + x) + 3] = v; }
  void SetGray(int x, int y, BYTE v) { SetPx(x, y, v, v, v); }

  // see if two bitmaps are the same
  bool Matches(int x0, int x1, int y0, int y1, const RawBitmap &other,
               int otherx0, int othery0);

  void SaveBitmap(string filename) const;
  void SavePartialBitmap(string filename, int x0, int x1, int y0, int y1) const;
};