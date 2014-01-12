#include "process.h"
#include "data.h"

int main()
{
    Sleep(2000);
    printf("Suspending process...\n");
    struct processData* pData = suspendProcessByWindow(GetForegroundWindow());
    Sleep(2000);
    printf("Resuming process...\n");
    resumeProcess(pData);
    free(pData);
    return 0;
}
