#ifndef MARKER_THREAD_H
#define MARKER_THREAD_H

#include <windows.h>
#include <cstdint>

struct MarkerData {
    int32_t* array;
    int32_t arraySize;
    int32_t markerID;
};

DWORD WINAPI MarkerThread(LPVOID lpParam);

void PrintArray(const int* array, int size, const char* message);
bool CheckArraySize(int size);

#endif // MARKER_THREAD_H