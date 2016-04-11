#pragma once
#include <regex>
#include <iostream>

// Scrapes a bitmap from the screen.

extern HWND hwndTarget;

using namespace std;

class Screen {
public:
  Screen();
  ~Screen();

  // http://stackoverflow.com/questions/9677580/visual-basic-rbg-capture-from-screen/10123008#10123008
  // http://stackoverflow.com/questions/2659932/how-to-read-the-screen-pixels
  // http://stackoverflow.com/questions/11283015/screen-capture-specific-window
  void GetScreenBitmap();

  int CaptureAnImage(HWND hWnd);
};
