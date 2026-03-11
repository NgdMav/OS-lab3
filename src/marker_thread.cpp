#include "../include/marker_thread.h"
#include <iostream>
#include <iomanip>

DWORD WINAPI MarkerThread(LPVOID lpParam) {
    if (!lpParam) {
        std::cerr << "MarkerThread: Invalid parameter\n";
        return 1;
    }

    MarkerData* data = static_cast<MarkerData*>(lpParam);

    if (!data->array || data->arraySize <= 0) {
        std::cerr << "MarkerThread(" << data->markerID << "): Invalid array data\n";
        return 1;
    }
}

void PrintArray(const int* array, int size, const char* message) {
    if (!array || size <= 0) {
        std::cerr << "PrintArray: Invalid parameters\n";
        return;
    }

    std::cout << "\n" << message << "\n[";
    for (int i = 0; i < size; ++i) {
        std::cout << array[i];
        if (i < size - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ]\n";
}

bool CheckArraySize(int size) {
    if (size <= 0) {
        std::cerr << "Error: array size must be a positive number\n";
        return false;
    }
    
    if (size > 10000) {
        std::cerr << "Warning: Array too large (max 10000)\n";
        return false;
    }
    
    return true;
}