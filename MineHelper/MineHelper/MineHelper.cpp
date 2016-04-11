// MineHelper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Screen.h"
#include "Recognizer.h"

#include <stdio.h>

#include <chrono>
#include <thread>

int main() {
	while (1) {
		Screen s;
		std::shared_ptr<RawBitmap> bitmap = s.GetScreenBitmap();
		//bitmap->SaveBitmap("test.bmp");
		Recognizer r;
		auto grid = r.Recognize(bitmap);
		grid->PrintGrid();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
