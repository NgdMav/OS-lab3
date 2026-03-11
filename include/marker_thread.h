#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include <windows.h>

struct MinMaxData {
    int* array;
    int size;
    int minValue;
    int maxValue;
    int minIndex;
    int maxIndex;
};

struct AverageData {
    int* array;
    int size;
    double averageValue;
};

DWORD WINAPI MinMaxThread(LPVOID lpParam);
DWORD WINAPI AverageThread(LPVOID lpParam);

void PrintArray(const int* array, int size, const char* message);
bool CheckArraySize(int size);

#endif // THREAD_FUNCTIONS_H