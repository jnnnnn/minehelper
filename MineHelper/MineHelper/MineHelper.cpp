// MineHelper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Screen.h"
#include "Recognizer.h"

#include <stdio.h>

int main() {
  Screen s;
  std::shared_ptr<RawBitmap> bitmap = s.GetScreenBitmap();
  bitmap->SaveBitmap("test.bmp");
  Recognizer r;
  auto grid = r.Recognize(bitmap);

  std::string str;
  cin >> str;
}
