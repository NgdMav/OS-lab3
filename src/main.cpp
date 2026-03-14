#include "../include/marker_thread.h"
#include <iostream>
#include <vector>

int main() {
    try {
        int32_t size = 0;
        
        std::cout << "=== Lab 3: Threads Synchronization ===\n";
        std::cout << "\nEnter the array size:";
        std::cin >> size;

        if (!CheckSize(size)) {
            return 1;
        }

        std::unique_ptr<int32_t[]> array(new int32_t[size]);
        for(size_t i = 0; i < size; i++) {
            array[i] = 0;
        }

        std::cout << "Array has been created";

        int32_t markerCount = 0;
        std::cout << "\nEnter the count of markers:";
        std::cin >> markerCount;
        if (!CheckSize(markerCount)) {
            return 1;
        }

        CRITICAL_SECTION cs;
        InitializeCriticalSection(&cs);

        std::vector<HANDLE> markerThreads(markerCount);
        std::vector<MarkerData> markerData(markerCount);

        std::cout << "\nPreparing all markers\n";

        for (size_t i = 0; i < markerCount; ++i)
        {
            HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            if (!startEvent) {
                std::cerr << "Error creating event " << i << "\n";
                for (size_t j = 0; j < i; ++j)
                {
                    CloseHandle(markerThreads[j]);
                    CloseHandle(markerData[j].startEvent);
                }
                DeleteCriticalSection(&cs);
                return 1;
                
            }

            markerData[i].array = array.get();
            markerData[i].arraySize = size;
            markerData[i].markerID = i;

            markerData[i].startEvent = startEvent;

            markerData[i].cs = &cs;

            DWORD threadId;
            markerThreads[i] = CreateThread(NULL, 0, MarkerThread, &markerData[i], 0, &threadId);
 
            if (markerThreads[i] == NULL) {
                DWORD error = GetLastError();
                std::cerr << "Error creating thread " << i << ". Code: " << error << "\n";
                DeleteCriticalSection(&cs);
                return 1;
            }
 
            std::cout << "Thread marker " << i << " was created (ID: " << threadId << ")\n";
        }
        

    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation error: " << e.what();
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what();
        return 1;
    } catch (...) {
        std::cerr << "Unknown error";
        return 1;
    }
}