#include "process.h"
#include "data.h"

BOOL CALLBACK EnumWindowsProc(HWND hWnd, struct processData * pData)
{
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    if(pid == pData->pid){
        if(pData->windowAmount >= 255 || !IsWindowVisible(hWnd)){
            return 1;
        }
        ShowWindow(hWnd,SW_HIDE);
        pData->windowHandles[pData->windowAmount] = hWnd;
        pData->windowAmount++;
    }
    return 1;
}

BOOL CALLBACK EnumWindowsProcNoPause(HWND hWnd, struct processData * pData)
{
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    if(pid == pData->pid){
        if(pData->windowAmount >= 255 || !IsWindowVisible(hWnd)){
            return 1;
        }
        pData->windowHandles[pData->windowAmount] = hWnd;
        pData->windowAmount++;
    }
    return 1;
}

struct processData* suspendProcessByWindow()
{
    struct processData* pData = malloc(sizeof(struct processData));
    GetWindowThreadProcessId(GetForegroundWindow(), &pData->pid);
    pData->windowAmount = 0;
    pData->windowHandles = malloc(sizeof(HWND)*256);
    EnumWindows(EnumWindowsProc,pData);
    pData->windowHandles = realloc(pData->windowHandles,pData->windowAmount*sizeof(HWND));
    suspendProcess(pData->pid, 1);
    pData->paused = 1;
    return pData;
}

struct processData* pDataByWindow()
{
    struct processData* pData = malloc(sizeof(struct processData));
    GetWindowThreadProcessId(GetForegroundWindow(), &pData->pid);
    pData->windowAmount = 0;
    pData->windowHandles = malloc(sizeof(HWND)*256);
    EnumWindows(EnumWindowsProcNoPause,pData);
    pData->windowHandles = realloc(pData->windowHandles,pData->windowAmount*sizeof(HWND));
    pData->paused = 0;
    return pData;
}

void pauseProcess(struct processData* pData)
{
    if(pData->paused == 1){
        return;
    }
    int i;
    for(i = 0; i != pData->windowAmount; ++i){
        ShowWindow(pData->windowHandles[i],SW_HIDE);
    }
    suspendProcess(pData->pid,1);
    pData->paused = 1;
    return;
}
void resumeProcess(struct processData* pData)
{
    suspendProcess(pData->pid,0);
    int i;
    for(i = 0; i != pData->windowAmount; ++i){
        ShowWindow(pData->windowHandles[i],SW_SHOW);
    }
    pData->paused = 0;
    return;
}

void suspendProcess(DWORD ProcessId, int Suspend)
{
    HANDLE snHandle = NULL;
    THREADENTRY32 te32 = {0};

    snHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snHandle == INVALID_HANDLE_VALUE) return;

    te32.dwSize = sizeof(THREADENTRY32);
    if (Thread32First(snHandle, &te32)){
        do{
            if (te32.th32OwnerProcessID == ProcessId){
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                if (!Suspend){
                    ResumeThread(hThread);
                }else{
                    SuspendThread(hThread);
                }
            CloseHandle(hThread);
            }
        }
        while (Thread32Next(snHandle, &te32));
    }
    CloseHandle (snHandle);
}

