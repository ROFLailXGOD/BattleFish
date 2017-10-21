#include "list.h"
#include"fish.h"
#include "FGWIN.H"
#include "defines.h"
#include "frame.h"
//#include"defines.h"

short *HMBuffer; // HM = HelpMenu
short *HM; // HM = HelpMenu
extern int Counter;
extern int BubbleFrame;
extern FISH fish[6];
extern int KillCount;

FISHLIST* LIST::createFish(int numberFish)
{
	FISHLIST* Node;
	int Hunger;
	Node = (FISHLIST *)malloc(sizeof(FISHLIST));
	Node->xDir = IRAND(RIGHT, LEFT);
	Node->yDir = IRAND(UP, DOWN);
	Node->FishDir = IRAND(0, 1);
	if (numberFish == 6)
	{
		Node->FishNum = IRAND(0, 5);
	}
	else if ((numberFish >= 0) && (numberFish < 6))
	{
		Node->FishNum = numberFish;
	}

	Node->Next = (FISHLIST *)NULL;
	Node->xMin = IRAND(-300, -100);
	// EDITED by ROFLail_X_GOD
	Node->xMax = IRAND(1380, 1580);
	// EDITED by ROFLail_X_GOD
	if (Node->xDir == RIGHT)
		Node->x = Node->xMin;
	else
		Node->x = Node->xMax;
	Node->xInc = IRAND(1, 4);
	Node->yMin = IRAND(60, 400);
	Node->yMax = IRAND(Node->yMin, 479);
	Node->y = IRAND(Node->yMin, Node->yMax);
	Node->yInc = IRAND(1, 4);
	Node->Frame = IRAND(1, 2);
	Node->FrameCounter = IRAND(0, 2);
	Node->HungerBar[0] = (short *)malloc(fg_imagesiz(100, 10));
	Node->HungerBar[1] = (short *)malloc(fg_imagesiz(100, 10));
	Hunger = 0.75 * fish[Node->FishNum].MaxHunger;
	Node->CurHunger = IRAND(Hunger, fish[Node->FishNum].MaxHunger);
	// copy hungerbar image to buffer
	fg_showpcx("Images/HungerBar.pcx", 0);
	fg_move(0, 9);
	fg_getdcb(Node->HungerBar[0], 100, 10);
	return Node;
}

void LIST::init(int hBackground, int hWork){
	register int i;
	FISHLIST *Node;
	int nNodes;

	for (i = 0; i < 6; i++)
		GetFish(i);
	head = (FISHLIST *)NULL;
	tail = (FISHLIST *)NULL;

	// choose a random number of fish
	srand(fg_getclock());
	nNodes = IRAND(6, 12);

	// HM init
	HMBuffer = (short *)malloc(fg_imagesiz(1280, 64));
	HM = (short *)malloc(fg_imagesiz(1280, 64)); // HM = HelpMenu
	fg_showpcx("Images/Text.pcx", 0);
	fg_move(0, 63);
	fg_getdcb(HMBuffer, 1280, 64);

	// build a linked list
	for (i = 0; i < nNodes; i++)
	{
		/*Node = (FISHLIST *)malloc(sizeof(FISHLIST));
		Node->xDir = IRAND(RIGHT, LEFT);
		Node->yDir = IRAND(UP, DOWN);
		Node->FishDir = IRAND(0, 1);
		Node->FishNum = IRAND(0, 5);
		Node->Next = (FISHLIST *)NULL;
		Node->xMin = IRAND(-300, -100);
		// EDITED by ROFLail_X_GOD
		Node->xMax = IRAND(1380, 1580);
		// EDITED by ROFLail_X_GOD
		if (Node->xDir == RIGHT)
			Node->x = Node->xMin;
		else
			Node->x = Node->xMax;
		Node->xInc = IRAND(1, 4);
		Node->yMin = IRAND(60, 400);
		Node->yMax = IRAND(Node->yMin, 479);
		Node->y = IRAND(Node->yMin, Node->yMax);
		Node->yInc = IRAND(1, 4);
		Node->Frame = IRAND(1, 2);
		Node->FrameCounter = IRAND(0, 2);
		Node->HungerBar[0] = (short *)malloc(fg_imagesiz(100, 10));
		Node->HungerBar[1] = (short *)malloc(fg_imagesiz(100, 10));
		Hunger = 0.75 * fish[Node->FishNum].MaxHunger;
		Node->CurHunger = IRAND(Hunger, fish[Node->FishNum].MaxHunger);*/
		Node = createFish(6);
		if (head == (FISHLIST *)NULL)
		{
			head = Node;
			tail = head;
		}
		else
		{
			tail->Next = Node;
			tail = Node;
		}

		// copy hungerbar image to buffer
		/*fg_showpcx("Images/HungerBar.pcx", 0);
		fg_move(0, 9);
		fg_getdcb(Node->HungerBar[0], 100, 10);*/

	}
}

LIST::~LIST(){
	FISHLIST *cur = head;
	FISHLIST *tmp;
	while (cur != (FISHLIST *)NULL) {
		tmp = cur->Next;
		free(cur);
		cur = tmp;
	}
}

void LIST::FeedFish()
{
	FISHLIST *Node;
	for (Node = head; Node != (FISHLIST *)NULL; Node = Node->Next)
		if (fish[Node->FishNum].MaxHunger - Node->CurHunger > 999)
			Node->CurHunger += 1000;
		else
		{
			this->killFish(Node);
		} // YOU KILLED HIM!
}

void LIST::GetFish(int FishNum){
	register int i;

	// display the fish PCX file the workspace virtual buffer
	fg_showpcx(fish[FishNum].FileName, 0);

	// grab the bitmaps for all the frames
	for (i = 0; i < fish[FishNum].nFrames; i++)
	{
		fish[FishNum].FishWidth[i] =
			fish[FishNum].Frame[i].x2 - fish[FishNum].Frame[i].x1 + 1;
		fish[FishNum].FishHeight[i] =
			fish[FishNum].Frame[i].y2 - fish[FishNum].Frame[i].y1 + 1;

		fish[FishNum].Bitmap[i] =
			(short *)malloc(fg_imagesiz(fish[FishNum].FishWidth[i], fish[FishNum].FishHeight[i]));

		fg_move(fish[FishNum].Frame[i].x1, fish[FishNum].Frame[i].y2);
		fg_getdcb(fish[FishNum].Bitmap[i],
			fish[FishNum].FishWidth[i], fish[FishNum].FishHeight[i]);
	}
}

void LIST::AnimateFish(int hBackground, int hWork)
{
	FISHLIST *Node;
	register int i;
	int nRows, y;
	int r, g, b;

	extern int i2Pressed;
	extern int i3Pressed;
	extern int i4Pressed;
	extern int i9Pressed;

	short Bubble[14 * 14];
	static char BubbleMask[14 * 14] = {
		0,0,0,0,0,1,1,1,1,0,0,0,0,0,
		0,0,0,1,1,1,1,1,1,1,1,0,0,0,
		0,0,1,1,1,1,1,1,1,1,1,1,0,0,
		0,0,1,1,1,1,1,1,1,1,1,1,0,0,
		0,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,1,1,1,1,1,1,1,1,1,1,1,1,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,0,1,1,1,1,1,1,1,1,1,1,0,0,
		0,0,1,1,1,1,1,1,1,1,1,1,0,0,
		0,0,0,1,1,1,1,1,1,1,1,0,0,0,
		0,0,0,0,0,1,1,1,1,0,0,0,0,0 };

	// copy background to workspace
	fg_vbcopy(0, vbWidth - 1, 0, vbHeight - 1, 0, vbHeight - 1, hBackground, hWork);

	// display next frame of water wheel flic file
	/*fg_vbopen(hWheel);
	if (fg_flicplay(WheelContext,1,5) == 0)
	{
	fg_flicskip(WheelContext,-1);
	fg_flicskip(WheelContext,1);
	fg_flicplay(WheelContext,1,5);
	BubbleFrame = 0;
	}
	fg_vbcopy(0,185,0,152,vbWidth-186,vbHeight-1,hWheel,hWork);*/
	//EDITED by HellWiz

	// create and display a translucent bubble moving up
	fg_vbopen(hWork);
	BubbleFrame++;
	y = (33 - BubbleFrame) * 11;
	fg_move(IRAND(457, 459), y);
	nRows = min(y, 14);
	if (nRows > 0)
	{
		fg_getdcb(Bubble, 14, nRows);
		for (i = 0; i < 14 * 14; i++)
		{
			if (BubbleMask[i])
			{
				fg_unmaprgb(Bubble[i], &r, &g, &b);
				Bubble[i] = fg_maprgb(r + 30, g + 30, b + 30);
			}
		}
		fg_clipdcb(Bubble, 14, nRows);
	}

	// display help menu (move it somewhere from there)
	fg_cutdcb(HMBuffer, HM, 0, 32 * (i2Pressed || i3Pressed), 1280, 1280 * i9Pressed, 32);
	fg_move(0, 710);
	fg_clipdcb(HM, 1280 * i9Pressed, 32);


	// display all the fish in the linked list
	for (Node = head; Node != (FISHLIST *)NULL; Node = Node->Next)
	{
		// blit the fish, may reverse while blitting
		fg_move(Node->x, Node->y);
		if (Node->xDir == fish[Node->FishNum].Direction)
		{
			fg_clipdcb(fish[Node->FishNum].Bitmap[Node->Frame],
				fish[Node->FishNum].FishWidth[Node->Frame],
				fish[Node->FishNum].FishHeight[Node->Frame]);
		}
		else
		{
			fg_flipdcb(fish[Node->FishNum].Bitmap[Node->Frame],
				fish[Node->FishNum].FishWidth[Node->Frame],
				fish[Node->FishNum].FishHeight[Node->Frame]);
		}

		// increment the horizontal position
		if (Node->xDir == RIGHT)
		{
			Node->x += Node->xInc;
			if (Node->x > Node->xMax)
				Node->xDir = LEFT;
		}
		else
		{
			Node->x -= Node->xInc;
			if (Node->x < Node->xMin)
				Node->xDir = RIGHT;
		}

		// adjust vertical position
		Node->y += Node->yDir;

		// keep vertical motion within tolerance area
		if (Node->y >= Node->yMax)
			Node->yDir = UP;
		else if (Node->y <= Node->yMin)
			Node->yDir = DOWN;

		// add a random element to speed and vertical motion
		Counter++;
		if (Counter % Node->y == 0)
		{
			Node->yDir = IRAND(-1, 5);
			if (Node->yDir > 1)
				Node->yDir = 0;
			Node->xInc = IRAND(1, 4);
			Node->yMin = IRAND(60, 400);
			Node->yMax = IRAND(Node->yMin, vbHeight - 1);
		}

		// add hungerbar above every fish and animate it
		if (Node->CurHunger > 0)
		{
			fg_cutdcb(Node->HungerBar[0], Node->HungerBar[1], 100, 0, 0, 100 * i4Pressed * Node->CurHunger / fish[Node->FishNum].MaxHunger, 10);
			fg_move(Node->x + fish[Node->FishNum].FishWidth[0] / 2 - 50, Node->y - fish[Node->FishNum].FishHeight[0] - 10);
			fg_clipdcb(Node->HungerBar[1], 100 * i4Pressed * Node->CurHunger / fish[Node->FishNum].MaxHunger, 10);
			--(Node->CurHunger);
		}
		else
		{
			this->killFish(Node);
		}; //if(Node->CurHunger <= 0){метод для удаления рыбы}

		++KillCount;
		KillCount %= 1;
		if (KillCount == 0) {
			if ((Node->FishNum == 5) && (Node->CurHunger < fish[Node->FishNum].MaxHunger / 2)) {
				iWannaKillSmbd(Node);
			}
		}

		// increment fish frame (swish tail slowly)
		Node->FrameCounter++;
		if (Node->FrameCounter >= fish[Node->FishNum].FrameDelay)
		{
			if (Node->FishDir == RIGHT)
			{
				Node->Frame++;
				if (Node->Frame >= fish[Node->FishNum].nFrames - 1)
					Node->FishDir = LEFT;
			}
			else
			{
				Node->Frame--;
				if (Node->Frame <= 1)
					Node->FishDir = RIGHT;
			}
			Node->FrameCounter = 0;
		}
	}

	// display the frame we just constructed
	fg_vbpaste(0, vbWidth - 1, 0, vbHeight - 1, 0, vbHeight - 1);
}

void LIST::killFish(int numberFish)
{
	numberFish--;
	FISHLIST* Test1, *Test2;
	Test1 = head;
	Test2 = (FISHLIST *)NULL;
	while (Test1 != (FISHLIST *)NULL)
	{
		if (Test1->FishNum == numberFish)
		{
			if (Test2 == (FISHLIST *)NULL)
			{
				head = head->Next;
				break;
			}
			else
			{
				Test2->Next = Test1->Next;
				break;
			}
		}
		Test2 = Test1;
		Test1 = Test1->Next;
	}
}

void LIST::killFish(FISHLIST* Test)
{
	if (Test == head)
	{
		head = head->Next;
	}
	else
	{
		FISHLIST* Test1 = head;
		while (Test1 != (FISHLIST *)NULL)
		{
			if (Test1->Next == Test)
			{
				Test1->Next = Test->Next;
				break;
			}
			Test1 = Test1->Next;
		}
	}
}

void LIST::addFish(int numberFish)
{
	FISHLIST* newFish = createFish(numberFish);
	if (head == (FISHLIST *)NULL)
	{
		head = newFish;;
		tail = head;
	}
	else
	{
		tail->Next = newFish;
		tail = newFish;
	}
}

void LIST::iWannaKillSmbd(FISHLIST* killer)
{
	FISHLIST* it = head;
	while (it != (FISHLIST*)NULL)
	{
		if ((killer != it) && (killer->Distance(it)))
		{
			if ((it->FishNum == 5) && (it->CurHunger < killer->CurHunger))
			{
				this->killFish(killer);
				it->CurHunger += 2000;
				break;
			}
			else
			{
				this->killFish(it);
				killer->CurHunger += 2000;
				break;
			}
		}
		it = it->Next;
	}
}

