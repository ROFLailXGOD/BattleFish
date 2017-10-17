#pragma once
#include "fish.h"
#include "fishlist.h"
#include "list.h"

/*#include "FGWIN.H"
#include <io.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "frame.h"
#include "fish.h"
#include "fishlist.h"
#include "list.h"*/

#define vbWidth  1280
#define vbHeight 720
#define vbDepth  16

#define RIGHT 0
#define LEFT  1

#define UP  -1
#define DOWN 1

// generate a random integer between 2 numbers
#define IRAND(min,max) ((rand()%((max)-(min)+1))+(min))

static FISH fish[6] =
{ {
		"Images/BlueDamsel.pcx", LEFT, 4, 4,
		79,232,218,281,
		270,423,219,281,
		457,610,219,281,
		73,226,334,396,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		10000
	},{
		"Images/BlueTang.pcx", RIGHT, 4, 4,
		492,635,200,275,
		325,468,200,275,
		165,308,200,275,
		1,144,199,274,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		10000
	},{
		"Images/Butterfly.pcx", RIGHT, 4, 4,
		244,397,191,286,
		41,194,191,286,
		41,194,337,433,
		244,397,338,433,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		1000
	},{
		"Images/Gudgeon.pcx", RIGHT, 4, 4,
		23,203,198,280,
		231,411,195,280,
		438,618,195,280,
		23,203,340,425,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		10000
	},{
		"Images/Killifish.pcx", RIGHT, 4,  4,
		133,285,153,205,
		350,502,153,205,
		136,288,273,324,
		350,502,272,324,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		10000
	},{
		"Images/SeaHorse.pcx", LEFT, 3, 4,
		388,435,205,293,
		295,342,205,293,
		199,246,205,293,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		20000
	} };

static char *ImageBuf;
static short *HungerBarBuf;
static short *TextBuffer;
static short *HungerBar = (short *)malloc(fg_imagesiz(100, 10));
static short *Text = (short *)malloc(fg_imagesiz(1280, 64));
static int i1Pressed = 0; //Fix this fucking thing pls

static int Counter = 0;
static int BubbleFrame = 0;

/*FISHLIST *Head;
FISHLIST *Tail;*/

//static int hBackground; // background virtual buffer
//static int hWork;       // workspace virtual buffer

static HDC      hDC;
static HPALETTE hPal;

static LIST mainlist;

// function declarations
/*void AnimateFish(void);
void GetFish(int);
void InitFish(void);*/

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);