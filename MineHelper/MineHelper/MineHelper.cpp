// MineHelper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Screen.h"
#include "Recognizer.h"
#include "Solver.h"
#include "Outputter.h"

#include <stdio.h>

#include <chrono>
#include <thread>

int main() {
  while (1) {
    Screen screen;
    std::shared_ptr<RawBitmap> bitmap = screen.GetScreenBitmap();
    // bitmap->SaveBitmap("test.bmp");
    Recognizer recognizer;
    auto grid = recognizer.Recognize(bitmap);
    Solver solver;
    solver.Solve(*grid);
    //grid->PrintGrid();
	Outputter outputter;
	outputter.Output(*grid);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
