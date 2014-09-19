#ifndef HEADER_PROCESS
#define HEADER_PROCESS

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdlib.h>
#include <TlHelp32.h>
#include <WinBase.h>

struct processData* suspendProcessByWindow();
void resumeProcess(struct processData* pData);
void pauseProcess(struct processData* pData);
void suspendProcess(DWORD ProcessId, int Suspend);
#endif // HEADER_PROCESS
