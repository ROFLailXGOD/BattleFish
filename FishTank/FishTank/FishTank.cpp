/****************************************************************************\
*                                                                            *
*  FishHC.c                                                                  *
*                                                                            *
*  High color Fastgraph fish tank.                                           *
*                                                                            *
\****************************************************************************/

#include "FGWIN.H"
#include <io.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#define vbWidth  1280
#define vbHeight 720
#define vbDepth  16

#define RIGHT 0
#define LEFT  1

#define UP  -1
#define DOWN 1

// generate a random integer between 2 numbers
#define IRAND(min,max) ((rand()%((max)-(min)+1))+(min))

// location of each fish frame as stored in a PCX file
typedef struct _frame
{
	int x1; int x2; int y1; int y2;
} FRAME;

// type FISH defines each species of fish
typedef struct _fish
{
	char  *FileName;
	int   Direction;
	int   nFrames;
	int   FrameDelay;
	FRAME Frame[4];
	int   FishWidth[4];
	int   FishHeight[4];
	short *Bitmap[4];
} FISH;

FISH fish[6] =
{ {
		"Images/BlueDamsel.pcx", LEFT, 4, 4,
		79,232,218,281,
		270,423,219,281,
		457,610,219,281,
		73,226,334,396,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL
	},{
		"Images/BlueTang.pcx", RIGHT, 4, 4,
		492,635,200,275,
		325,468,200,275,
		165,308,200,275,
		1,144,199,274,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL
	},{
		"Images/Butterfly.pcx", RIGHT, 4, 4,
		244,397,191,286,
		41,194,191,286,
		41,194,337,433,
		244,397,338,433,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL
	},{
		"Images/Gudgeon.pcx", RIGHT, 4, 4,
		23,203,198,280,
		231,411,195,280,
		438,618,195,280,
		23,203,340,425,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL
	},{
		"Images/Killifish.pcx", RIGHT, 4,  4,
		133,285,153,205,
		350,502,153,205,
		136,288,273,324,
		350,502,272,324,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL
	},{
		"Images/SeaHorse.pcx", LEFT, 3, 4,
		388,435,205,293,
		295,342,205,293,
		199,246,205,293,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL
	} };
// EDITED by ROFLail_X_GOD

// forward declaration
struct _FishList;
typedef struct _FishList FISHLIST;

// Each fish is stored as a sprite in a linked list
typedef struct _FishList
{
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
} FISHLIST;

FISHLIST *Head;
FISHLIST *Tail;

int hBackground; // background virtual buffer
int hWork;       // workspace virtual buffer
				 //int hWheel;      // water wheel virtual buffer
				 //EDITED by HellWiz

				 // buffers required for water wheel flic file
				 //char WheelContext[20];
				 //EDITED by HellWiz
char *ImageBuf;

int Counter = 0;
int BubbleFrame = 0;

// function declarations
void AnimateFish(void);
void GetFish(int);
void InitFish(void);

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdParam, int iCmdShow)
{
	static char szAppName[] = "FGfishHC";
	HWND        hWnd;
	MSG         msg;
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wndclass);

	hWnd = CreateWindow(szAppName,  // window class name
		"Best Fish Tank by HTD SuperTeam", // window caption
		WS_POPUP,                // window style
		0,                       // initial x position
		0,                       // initial y position
		vbWidth,                 // initial x size
		vbHeight,                // initial y size
		NULL,                    // parent window handle
		NULL,                    // window menu handle
		hInstance,               // program instance handle
		NULL);                   // creation parameters
								 // EDITED by ROFLail_X_GOD

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	PlaySound("Sounds/BackGroundMusic.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	//ADDED by ROFLail_X_GOD

	// The message loop processes entries placed in the message queue.
	// When no message is ready, call AnimateFish() to perform one frame
	// of animation.

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		/*else
		AnimateFish();*/
		//EDITED by HellWiz
	}

	return msg.wParam;
}

/****************************************************************************\
*                                                                            *
*  WindowProc()                                                              *
*                                                                            *
\****************************************************************************/

HDC      hDC;
HPALETTE hPal;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	int  BuffSize;
	int  Button;
	FILE *Stream;
	register int i;

	switch (iMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		fg_setdc(hDC);
		hPal = fg_defpal();
		fg_realize(hPal);
		SetTimer(hWnd, 1, 16, NULL);
		ShowWindow(hWnd, SW_SHOWNORMAL);

		// set 640x480 display resolution
		if (fg_modetest(vbWidth, vbHeight, fg_colors()) != 0)
		{
			MessageBox(hWnd, "Cannot set desktop resolution.", "Error", MB_OK | MB_ICONSTOP);
			DestroyWindow(hWnd);
			return 0;
		}
		fg_modeset(vbWidth, vbHeight, fg_colors(), 1);

		// initialize virtual buffers
		fg_vbinit();
		fg_vbdepth(vbDepth);
		//hWheel = fg_vballoc(186,153);
		//EDITED by HellWiz
		hWork = fg_vballoc(vbWidth, vbHeight);
		hBackground = fg_vballoc(vbWidth, vbHeight);

		// load background image into the background virtual buffer
		fg_vbopen(hBackground);
		fg_showjpeg("Images/Aquarium.jpg", 0);

		// get all the fish and put them into a linked list
		fg_vbopen(hWork);
		for (i = 0; i < 6; i++)
			GetFish(i);
		InitFish();

		// load the water wheel flic file into the fg_imagebuf() buffer
		/*fg_flicopen("Wheel.flc",WheelContext);
		fg_flicdone(WheelContext);
		Stream = fopen("Wheel.flc","rb");
		BuffSize = filelength(fileno(Stream));
		ImageBuf = (char *)malloc(BuffSize);
		fread(ImageBuf,sizeof(char),BuffSize,Stream);
		fclose(Stream);
		fg_imagebuf(ImageBuf,BuffSize);*/
		//EDITED by HellWiz

		// load virtual palette with the water wheel flic file palette
		/*fg_vbopen(hWheel);
		fg_flicplay(WheelContext,1,5);
		fg_vbclose();*/
		//EDITED by HellWiz

		// things won't look too good with a 256-color display driver
		if (fg_colors() <= 8)
		{
			MessageBeep(0);
			Button = MessageBox(hWnd,
				"A high color or true color display is recommended. Continue?",
				"FishHC", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
			if (Button == IDNO)
			{
				DestroyWindow(hWnd);
				return 0;
			}
		}

		fg_vbopen(hBackground);
		return 0;

	case WM_TIMER:
		AnimateFish();
		return 0;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		case VK_F12:
			DestroyWindow(hWnd);
			break;
		}
		return 0;

	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		fg_vbpaste(0, vbWidth - 1, 0, vbHeight - 1, 0, vbHeight - 1);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_SETFOCUS:
		fg_realize(hPal);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		fg_vbclose();
		fg_vbfree(-1);
		fg_vbfin();
		DeleteObject(hPal);
		ReleaseDC(hWnd, hDC);
		fg_modeset(0, 0, 0, 0);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

/****************************************************************************\
*                                                                            *
*  AnimateFish()                                                             *
*                                                                            *
\****************************************************************************/

void AnimateFish(void)
{
	FISHLIST *Node;
	register int i;
	int nRows, y;
	int r, g, b;

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

	// display all the fish in the linked list
	for (Node = Head; Node != (FISHLIST *)NULL; Node = Node->Next)
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

/****************************************************************************\
*                                                                            *
*  GetFish()                                                                 *
*                                                                            *
\****************************************************************************/

void GetFish(int FishNum)
{
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

/****************************************************************************\
*                                                                            *
*  InitFish()                                                                *
*                                                                            *
\****************************************************************************/

void InitFish(void)
{
	FISHLIST *Node;
	int nNodes;
	register int i;

	// choose a random number of fish
	srand(fg_getclock());
	nNodes = IRAND(6, 12);

	// build a linked list
	for (i = 0; i < nNodes; i++)
	{
		Node = (FISHLIST *)malloc(sizeof(FISHLIST));
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
		if (Head == (FISHLIST *)NULL)
		{
			Head = Node;
			Tail = Head;
		}
		else
		{
			Tail->Next = Node;
			Tail = Node;
		}
	}
}
