#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
HANDLE pipeToAI;  //pipe handle
HANDLE pipeFromAI;//pipe handle
HANDLE process;   //process handle
DWORD processId;  //process identifier
static HANDLE startEvent, start2Event;
int width = 15;
char buf[800];
using namespace std;


#ifdef GOMOKU_API
#define GOMOKU_API __declspec(dellexport)
#else
#define GOMOKU_API __declspec(dllimport)
#endif 

class GOMOKU_API Gomoku_AI {
public:
	void init();
	int createProcess(bool pipe, STARTUPINFO& si);
	void startPipeAI();
	void sendCommand(const char* text, ...);
	int readCommand(char* buf, int n);
	pair<int, int> extractTwoInts(const char buf[100]);
	pair<int, int> getSteps(int x, int y);
};