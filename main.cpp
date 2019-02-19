// XE-first.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include <iostream>
#include"Common\\global.h"
#include"Chapter6\\CHillApp.h"
#include"Chapter6\\CBoxApp.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prev_instance,
	PSTR cmd_line, s32 show_cmd)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	xe::CHillApp app(hinstance);
	if (!app.VInit())
		return false;
	return app.VRun();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
