#ifndef HEADER_PROCESS
#define HEADER_PROCESS

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdlib.h>
#include <TlHelp32.h>
#include <WinBase.h>

struct processData* suspendProcessByWindow(HWND hWnd);
void resumeProcess(struct processData* pData);
#endif // HEADER_PROCESS
