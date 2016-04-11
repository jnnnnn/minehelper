#pragma once
#include <regex>
#include <iostream>
#include "RawBitmap.h"

// Scrapes a bitmap from the screen.

extern HWND hwndTarget;

class Screen {
public:
  shared_ptr<RawBitmap> GetScreenBitmap();
};
