#pragma once
#include "frame.h"
//#include"defines.h"
// type FISH defines each species of fish
class FISH {
public:
	char  *FileName;
	int   Direction;
	int   nFrames;
	int   FrameDelay;
	FRAME Frame[4];
	int   FishWidth[4];
	int   FishHeight[4];
	short *Bitmap[4];
	int   MaxHunger;
};