#ifndef HEADER_DATA
#define HEADER_DATA

#include <stdlib.h>

struct processData{
    DWORD pid;
    short windowAmount;
    HWND* windowHandles;
    char paused;
};

#endif
