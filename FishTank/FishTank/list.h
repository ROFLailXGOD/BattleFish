#pragma once
#include"fishlist.h"
#include <fstream>

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
	int iWannaKillSmbd(FISHLIST *);
	void FeedFish();
	void sereal(FISHLIST *);
	FISHLIST* createFishlist(int, std::ifstream &);
	void dereal();
private:
	void GetFish(int);
};