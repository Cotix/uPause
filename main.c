#include "process.h"
#include "data.h"
#include <stdlib.h>

struct node* pausedProcesses[10];
struct node* pausedProcessesEnd[10];


int getGroupNumber()
{
    int i;
    for(i = 0x30; i != 0x30+10; ++i){
        if(GetKeyState(i) < 0)
            return i-0x30;
    }
    return -1;
}

int isInGroup(struct processData* pData)
{
    int group;
    for(group = 0; group != 10; ++group){
        struct node* ptr;
        for(ptr = pausedProcesses[group]; ptr != pausedProcessesEnd[group]; ptr = ptr->next){
            if(ptr->data->pid == pData->pid){
                return 1;
            }
        }
    }
    return 0;
}

void addGroup(int group, struct processData* pData)
{
    if(isInGroup(pData)){
        free(pData);
        return;
    }
    pausedProcessesEnd[group]->next = malloc(sizeof(struct node));
    pausedProcessesEnd[group]->data = pData;
    pausedProcessesEnd[group] = pausedProcessesEnd[group]->next;
}

void resumeGroup(int group)
{
    struct node* ptr;
    for(ptr = pausedProcesses[group]; ptr != pausedProcessesEnd[group]; ptr = ptr->next){
        resumeProcess(ptr->data);
    }
}

void pauseGroup(int group)
{
    struct node* ptr;
    for(ptr = pausedProcesses[group]; ptr != pausedProcessesEnd[group]; ptr = ptr->next){
        pauseProcess(ptr->data);
    }
}

void addGroupPaused(int group, struct processData* pData)
{
    pauseProcess(pData);
    addGroup(group, pData);
}

void initGroups()
{
    int i;
    for(i = 0; i != 10; ++i){
        pausedProcesses[i] = malloc(sizeof(struct node));
        pausedProcesses[i]->next = 0;
        pausedProcessesEnd[i] = pausedProcesses[i];
    }
}

int main()
{
    initGroups();
    while(1){
        Sleep(50);
        int group = getGroupNumber();
        if(group == -1){
            continue;
        }
        if(GetKeyState(VK_SHIFT) < 0){
            struct processData *pData = pDataByWindow();
            if(GetKeyState(VK_LCONTROL) < 0 || GetKeyState(VK_RCONTROL) < 0){
                addGroupPaused(group, pData);
                continue;
            }
            addGroup(group,pData);
            continue;
        }
        if(GetKeyState(VK_LCONTROL) < 0 || GetKeyState(VK_RCONTROL) < 0){
            if(GetKeyState(VK_SHIFT) < 0){
                continue;
            }
            pauseGroup(group);
            continue;
        }
        if(GetKeyState(VK_LMENU) < 0){
            resumeGroup(group);
            continue;
        }
    }
}
