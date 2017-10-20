#pragma once
#include"fishlist.h"
//#include"defines.h"

class LIST {
public:
	FISHLIST* head;
	FISHLIST* tail;
	FISHLIST * createFish(int);
	void init(int,int);
	~LIST();
	void AnimateFish(int,int);
	void killFish(int);
	void killFish(FISHLIST *);
	void addFish(int);
	void iWannaKillSmbd(FISHLIST *);
	void FeedFish();
private:
	void GetFish(int);
	//int Counter = 0;
	
};