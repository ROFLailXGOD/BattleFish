#pragma once
#include"fishlist.h"
//#include"defines.h"

class LIST {
public:
	FISHLIST* head;
	FISHLIST* tail;
	void init(int,int);
	~LIST();
	void AnimateFish(int,int);
	void FeedFish();
private:
	void GetFish(int);
	//int Counter = 0;
	
};