#ifndef MARKER_THREAD_H
#define MARKER_THREAD_H

#include <windows.h>
#include <cstdint>

struct MarkerData {
    int32_t* array;
    int32_t arraySize;
    int32_t markerID;

    HANDLE startEvent;

    CRITICAL_SECTION* cs;
};

DWORD WINAPI MarkerThread(LPVOID lpParam);

void PrintArray(const int32_t* array, int32_t size, const char* message);
bool CheckSize(int32_t size);

#endif // MARKER_THREAD_H