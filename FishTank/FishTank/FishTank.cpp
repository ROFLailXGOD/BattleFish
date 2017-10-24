#include "FGWIN.H"
#include <io.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "frame.h"
#include "fish.h"
#include "fishlist.h"
#include "list.h"

int hBackground; // background virtual buffer
int hWork;       // workspace virtual buffer
HDC      hDC;
HPALETTE hPal;
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

FISH fish[6] =
{ {
		"Images/BlueDamsel.pcx", LEFT, 4, 4,
		79,232,218,281,
		270,423,219,281,
		457,610,219,281,
		73,226,334,396,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		11000
	},{
		"Images/BlueTang.pcx", RIGHT, 4, 4,
		492,635,200,275,
		325,468,200,275,
		165,308,200,275,
		1,144,199,274,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		12500
	},{
		"Images/Butterfly.pcx", RIGHT, 4, 4,
		244,397,191,286,
		41,194,191,286,
		41,194,337,433,
		244,397,338,433,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		12000
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
		9000
	},{
		"Images/SeaHorse.pcx", LEFT, 3, 4,
		388,435,205,293,
		295,342,205,293,
		199,246,205,293,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		NULL,NULL,NULL,NULL,
		7000
	} };

int Counter = 0;
int KillCount = 0;
int BubbleFrame = 0;
LIST mainlist;

int i2Pressed; //Add Fish
int i3Pressed; //Remove Fish
int i4Pressed = 1; //Show/Hide Health Bar
int i8Pressed = 1; //Mute/Unmute
int i9Pressed = 1; //Show/Hide HM

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

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	PlaySound("Sounds/BackGroundMusic.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

	while (TRUE){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT)
				break;
			else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	int  BuffSize;
	int  Button;
	FILE *Stream;
	register int i;

	switch (iMsg){
	case WM_CREATE:
		hDC = GetDC(hWnd);
		fg_setdc(hDC);
		hPal = fg_defpal();
		fg_realize(hPal);
		SetTimer(hWnd, 1, 16, NULL);
		ShowWindow(hWnd, SW_SHOWNORMAL);

		// set display resolution
		if (fg_modetest(vbWidth, vbHeight, fg_colors()) != 0){
			MessageBox(hWnd, "Cannot set desktop resolution.", "Error", MB_OK | MB_ICONSTOP);
			DestroyWindow(hWnd);
			return 0;
		}
		fg_modeset(vbWidth, vbHeight, fg_colors(), 1);

		// initialize virtual buffers
		fg_vbinit();
		fg_vbdepth(vbDepth);
		hWork = fg_vballoc(vbWidth, vbHeight);
		hBackground = fg_vballoc(vbWidth, vbHeight);

		// load background image into the background virtual buffer
		fg_vbopen(hBackground);
		fg_showjpeg("Images/Aquarium.jpg", 0);

		// get all the fish and put them into a linked list
		fg_vbopen(hWork);
		mainlist.init(hBackground, hWork);

		// things won't look too good with a 256-color display driver
		if (fg_colors() <= 8){
			MessageBeep(0);
			Button = MessageBox(hWnd,
				"A high color or true color display is recommended. Continue?",
				"FishHC", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
			if (Button == IDNO){
				DestroyWindow(hWnd);
				return 0;
			}
		}

		fg_vbopen(hBackground);
		return 0;

	case WM_TIMER:
		mainlist.AnimateFish(hBackground, hWork);
		return 0;

	case WM_KEYDOWN:
		switch (wParam){
		case 48: // 0
			i2Pressed = 0;
			i3Pressed = 0;
			break;
		case 49: // 1
			if (i2Pressed){
				mainlist.addFish(0);
			} // Add Blue Damsel
			else if (i3Pressed){
				mainlist.killFish(0);
			} // Remove Blue Damsel
			else
				mainlist.FeedFish();
			break;
		case 50: // 2
			if (i2Pressed){
				mainlist.addFish(1);
			} // Add Blue Tang
			else if (i3Pressed){
				mainlist.killFish(1);
			} // Remove Blue Tang
			else
				++i2Pressed;
			break;
		case 51: // 3
			if (i2Pressed){
				mainlist.addFish(2);
			} // Add Butterfly
			else if (i3Pressed){
				mainlist.killFish(2);
			} // Remove Butterfly
			else
				++i3Pressed;
			break;
		case 52: // 4
			if (i2Pressed){
				mainlist.addFish(3);
			} // Add Gudgeon
			else if (i3Pressed){
				mainlist.killFish(3);
			} // Remove Gudgeon
			else
				if (i4Pressed)
					--i4Pressed;
				else
					++i4Pressed;
			break;
		case 53: // 5
			if (i2Pressed){
				mainlist.addFish(4);
			} // Add Killifish
			else if (i3Pressed){
				mainlist.killFish(4);
			} // Remove Killifish
			break;
		case 54: // 6
			if (i2Pressed){
				mainlist.addFish(5);
			} // Add Sea Horse
			else if (i3Pressed){
				mainlist.killFish(5);
			} // Remove Sea Horse
			break;
		case 56: // 8
			if (i8Pressed){
				PlaySound(NULL, NULL, NULL);
				--i8Pressed;
			}
			else{
				PlaySound("Sounds/BackGroundMusic.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
				++i8Pressed;
			}
			break;
		case 57: // 9
			if (i9Pressed)
				--i9Pressed;
			else
				++i9Pressed;
			break;
		case VK_F5: 
			// Quick Save
			mainlist.sereal(mainlist.head);
			break;
		case VK_F9:
			//Quick Load
			mainlist.dereal();
			break;
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
