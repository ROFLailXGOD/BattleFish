#pragma once
#include"fishlist.h"
#include <fstream>
//#include"defines.h"

class LIST {
public:
	FISHLIST* head;
	FISHLIST* tail;
	FISHLIST* createFish(int);
	void init(int,int);
	~LIST();
	void AnimateFish(int,int);
	void killFish(int);
	void killFish(FISHLIST *);
	void addFish(int);
	void iWannaKillSmbd(FISHLIST *);
	void FeedFish();
	void sereal(FISHLIST *);
	FISHLIST* createFishlist(int, std::ifstream &);
	void dereal();
private:
	void GetFish(int);
	//int Counter = 0;
	
};