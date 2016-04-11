#pragma once
#include "MineGrid.h"
#include "RawBitmap.h"

// Receives a bitmap and outputs a grid of what the squares contain.
class Recognizer
{
public:
	Recognizer();
	shared_ptr<MineGrid> Recognize(shared_ptr<RawBitmap> bitmap);

protected:
	shared_ptr<RawBitmap> GridCorner;
};

