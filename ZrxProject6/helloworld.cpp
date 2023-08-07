#include "pch.h"
//#include"dllGomoku.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
using namespace std;

HANDLE pipeToAI;  //pipe handle
HANDLE pipeFromAI;//pipe handle
HANDLE process;   //process handle
DWORD processId;  //process identifier
static HANDLE startEvent, start2Event;
void sendCommand(const char* text, ...);
int readCommand(char* buf, int n);
void startPipeAI();
void init();
pair<int, int> extractTwoInts(const char buf[100]);
pair<int, int> getSteps(int x, int y);

int width = 15;
char buf[800];



/*int main() {

	init();
	while (1) {
		int x, y;
		cin >> x >> y;
		pair<int, int> result = getSteps(x, y);
		cout << result.first << " " << result.second << endl;
	}


}*/

void init() {
	startPipeAI();
	sendCommand("START %d", width);
	sendCommand("INFO timeout_turn %d", 5000);
	readCommand(buf, sizeof(buf));
}

pair<int, int> getSteps(int x, int y) {
	sendCommand("TURN %d,%d", x, y);
	readCommand(buf, sizeof(buf));
	pair<int, int> result = extractTwoInts(buf);
	return result;
}

int createProcess(bool pipe, STARTUPINFO& si)
{
	wchar_t exe[] = L"pbrain-test.exe";
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	si.dwFlags |= STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
	PROCESS_INFORMATION pi;

	if (!CreateProcess(0, exe, 0, 0, pipe ? TRUE : FALSE,
		0, 0, NULL, &si, &pi)) {
		std::cout << "Failed to create process." << std::endl;
		return 1;
	}

	CloseHandle(pi.hThread);
	process = pi.hProcess;
	processId = pi.dwProcessId;
	return 0;
}

void startPipeAI()
{
	HANDLE h, hChildStdOut, hChildStdIn, hChildStdErr;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&h, &hChildStdOut, &sa, 0))
		std::cout << "CreatePipe failed" << std::endl;
	if (!DuplicateHandle(GetCurrentProcess(), h,
		GetCurrentProcess(), &pipeFromAI,
		0, FALSE, DUPLICATE_SAME_ACCESS))
		std::cout << "DuplicateHandle failed" << std::endl;

	CloseHandle(h);

	if (!CreatePipe(&hChildStdIn, &h, &sa, 20 * 20 * 10))
		std::cout << "CreatePipe failed" << std::endl;
	if (!DuplicateHandle(GetCurrentProcess(), h,
		GetCurrentProcess(), &pipeToAI,
		0, FALSE, DUPLICATE_SAME_ACCESS))
		std::cout << "DuplicateHandle failed" << std::endl;

	CloseHandle(h);

	if (!DuplicateHandle(GetCurrentProcess(), hChildStdOut,
		GetCurrentProcess(), &hChildStdErr,
		0, TRUE, DUPLICATE_SAME_ACCESS))
		std::cout << "DuplicateHandle failed" << std::endl;

	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hChildStdIn;
	si.hStdOutput = hChildStdOut;
	si.hStdError = hChildStdErr;

	createProcess(true, si);

	CloseHandle(hChildStdIn);
	CloseHandle(hChildStdOut);
	CloseHandle(hChildStdErr);
}

void sendCommand(const char* text, ...)
{
	if (!pipeToAI) return;
	char buffer[256];
	va_list v;
	va_start(v, text);
	int i = vsprintf_s(buffer, text, v);
	buffer[i++] = '\r';
	buffer[i++] = '\n';
	buffer[i] = 0;
	DWORD w;
	WriteFile(pipeToAI, buffer, i, &w, 0);
	va_end(v);
}

int readCommand(char* buf, int n)
{
	char* p;
	do {
		for (p = buf;;) {
			DWORD r;
			if (!ReadFile(pipeFromAI, p, 1, &r, 0) || !r) {
				*p = 0;
				return 1;
			}
			if (*p == '\r' || *p == '\n') break;
			if (p < buf + n - 3) p++;
		}
		*p = 0;
	} while (p == buf);
	while (buf[0] == 'D') {
		readCommand(buf, sizeof(buf));
	}
	return 0;
}

pair<int, int> extractTwoInts(const char buf[100]) {
	std::string str(buf);
	std::istringstream iss(str);
	std::string token;
	std::pair<int, int> result;

	std::getline(iss, token, ',');
	result.first = std::stoi(token);

	std::getline(iss, token, ',');
	result.second = std::stoi(token);

	return result;
}




void helloworld()
{
	acutPrintf(_T("\nHello world!"));
	//Gomoku_AI ai;
	//ai.init();
	//std::pair<int, int> res = ai.getSteps(1, 6);
	init();
	std::pair<int, int> res = getSteps(1, 6);
	acutPrintf(_T("n %d, %d"), res.first, res.second);
}


int showhello(struct resbuf *rb)
{
	ads_printf(_T("hello"));
	ads_retvoid();
	return RTNORM;
}

int showhellocmd(struct resbuf *rb)
{
	ads_printf(_T("hello"));
	ads_retvoid();
	return RTNORM;
}
