#pragma once
class FISHLIST{
public:
	FISHLIST *Next;
	int FishNum;
	int x;
	int y;
	int xMin;
	int xMax;
	int xInc;
	int yMin;
	int yMax;
	int yInc;
	int Frame;
	int xDir;
	int yDir;
	int FishDir;
	int FrameCounter;
	int CurHunger;
	short *HungerBar[2];
	bool Distance(FISHLIST *);
	~FISHLIST();
};