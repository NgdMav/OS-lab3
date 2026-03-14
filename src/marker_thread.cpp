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

    WaitForSingleObject(data->startEvent, INFINITE);

    std::cout << "[Marker#" << data->markerID <<"]Started\n";

    srand(data->markerID);

    data->markedCount = 0;
    data->blockedIndex = -1;

    while (true) {
        int32_t randomValue = rand();
        int32_t index = randomValue % data->arraySize;
 
        EnterCriticalSection(data->cs);
        if (data->array[index] == 0) {
            LeaveCriticalSection(data->cs);

            Sleep(5);

            EnterCriticalSection(data->cs);
            data->array[index] = data->markerID;
            data->markedCount++;
            LeaveCriticalSection(data->cs);

            Sleep(5);
            
            continue;
        } 
        else {
            data->blockedIndex = index;
            LeaveCriticalSection(data->cs);
            std::cout << "\n[Marker#" << data->markerID << "] Cannot continue:\n";
            std::cout << "  Marked elements: " << data->markedCount << "\n";
            std::cout << "  Blocked on: " << data->blockedIndex << " index (marked by " << data->array[index] << ")\n";
            
            SetEvent(data->cannotContinueEvent);
            
            HANDLE events[] = {data->continueEvent, data->terminateEvent};
            DWORD waitResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);
            
            if (waitResult == WAIT_OBJECT_0) {
                std::cout << "[Marker#" << data->markerID << "] Continue to work\n";
                ResetEvent(data->cannotContinueEvent);
                ResetEvent(data->continueEvent);
                continue;
            }
            else if (waitResult == WAIT_OBJECT_0 + 1) {
                break;
            }
            else {
                std::cerr << "[Marker#" << data->markerID << "] Error waiting events\n";
                return 1;
            }
        }
    }
}

void PrintArray(const int32_t* array, int32_t size, const char* message) {
    if (!array || size <= 0) {
        std::cerr << "PrintArray: Invalid parameters\n";
        return;
    }

    std::cout << "\n" << message << "\n[";
    for (size_t i = 0; i < size; ++i) {
        std::cout << array[i];
        if (i < size - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ]\n";
}

bool CheckSize(int32_t size) {
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