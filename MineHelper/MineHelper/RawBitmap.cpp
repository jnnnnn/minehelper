#include "stdafx.h"
#include "RawBitmap.h"

bool RawBitmap::Matches(int x0, int x1, int y0, int y1, const RawBitmap &other,
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

void RawBitmap::SaveBitmap(const string filename) const {
  SavePartialBitmap(filename, 0, Width, 0, Height);
}

void RawBitmap::SavePartialBitmap(const string filename, int x0, int x1, int y0,
                                  int y1) const {
  unsigned int headers[13];
  FILE *outfile;
  int extrabytes;
  int paddedsize;
  int x;
  int y;
  int n;
  int width = x1 - x0;
  int height = y1 - y0;

  // How many bytes of padding to add to each horizontal line - the size of
  // which must be a multiple of 4 bytes.
  extrabytes = 4 - ((width * 3) % 4);
  if (extrabytes == 4)
    extrabytes = 0;

  paddedsize = ((width * 3) + extrabytes) * height;

  // Headers...
  // Note that the "BM" identifier in bytes 0 and 1 is NOT included in these
  // "headers".

  headers[0] = paddedsize + 54; // bfSize (whole file size)
  headers[1] = 0;               // bfReserved (both)
  headers[2] = 54;              // bfOffbits
  headers[3] = 40;              // biSize
  headers[4] = width;           // biWidth
  headers[5] = height;          // biHeight

  // Would have biPlanes and biBitCount in position 6, but they're shorts.
  // It's easier to write them out separately (see below) than pretend
  // they're a single int, especially with endian issues...

  headers[7] = 0;          // biCompression
  headers[8] = paddedsize; // biSizeImage
  headers[9] = 0;          // biXPelsPerMeter
  headers[10] = 0;         // biYPelsPerMeter
  headers[11] = 0;         // biClrUsed
  headers[12] = 0;         // biClrImportant

  outfile = fopen(filename.c_str(), "wb");

  //
  // Headers begin...
  // When printing ints and shorts, we write out 1 character at a time to
  // avoid endian issues.
  //

  fprintf(outfile, "BM");

  for (n = 0; n <= 5; n++) {
    fprintf(outfile, "%c", headers[n] & 0x000000FF);
    fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
    fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
    fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
  }

  // These next 4 characters are for the biPlanes and biBitCount fields.
  fprintf(outfile, "%c", 1);
  fprintf(outfile, "%c", 0);
  fprintf(outfile, "%c", 24);
  fprintf(outfile, "%c", 0);

  for (n = 7; n <= 12; n++) {
    fprintf(outfile, "%c", headers[n] & 0x000000FF);
    fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
    fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
    fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
  }

  // Headers done, now write the data...
  // BMP image format is written from bottom to top...
  for (y = y0; y < y1; y++) {
    for (x = x0; x < x1; x++) {
      // Also, it's written in (b,g,r) format...
      fprintf(outfile, "%c", PosB(x, y));
      fprintf(outfile, "%c", PosG(x, y));
      fprintf(outfile, "%c", PosR(x, y));
    }
    // See above - BMP lines must be of lengths divisible by 4.
    if (extrabytes) {
      for (n = 1; n <= extrabytes; n++) {
        fprintf(outfile, "%c", 0);
      }
    }
  }

  fclose(outfile);
  return;
}