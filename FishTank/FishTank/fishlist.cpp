#include"fishlist.h"
#include"fish.h"
#include<math.h>
#include<algorithm>
extern FISH fish[6];
bool FISHLIST::Distance(FISHLIST* sacrifice)
{
	int killersWidth = fish[this->FishNum].FishWidth[0];
	double dist = sqrt(((this->x + killersWidth / 2) - (sacrifice->x + fish[sacrifice->FishNum].FishWidth[0] / 2))*
		((this->x + killersWidth / 2) - (sacrifice->x + fish[sacrifice->FishNum].FishWidth[0] / 2)) + 
		((this->y + fish[5].FishHeight[0] / 2) - (sacrifice->y + fish[sacrifice->FishNum].FishHeight[0] / 2))*
		((this->y + fish[5].FishHeight[0] / 2) - (sacrifice->y + fish[sacrifice->FishNum].FishHeight[0] / 2)));
	return (dist <= std::min(fish[5].FishHeight[0],fish[sacrifice->FishNum].FishHeight[0]));
}