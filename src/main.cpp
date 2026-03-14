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

        std::cout << "\nPreparing all markers...\n";

        for (size_t i = 0; i < markerCount; ++i)
        {
            HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            HANDLE continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            HANDLE terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            HANDLE cannotContinueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            if (!startEvent || !continueEvent || !terminateEvent || !cannotContinueEvent) {
                std::cerr << "Error creating events for " << i << "\n";
                for (size_t j = 0; j < i; ++j)
                {
                    CloseHandle(markerThreads[j]);
                    CloseHandle(markerData[j].startEvent);
                    CloseHandle(markerData[j].continueEvent);
                    CloseHandle(markerData[j].terminateEvent);
                    CloseHandle(markerData[j].cannotContinueEvent);
                }
                if (startEvent) {
                    CloseHandle(startEvent);
                }
                if (continueEvent) {
                    CloseHandle(continueEvent);
                }
                if (terminateEvent) {
                    CloseHandle(terminateEvent);
                }
                if (cannotContinueEvent) {
                    CloseHandle(cannotContinueEvent);
                }
                DeleteCriticalSection(&cs);
                return 1;
            }

            markerData[i].array = array.get();
            markerData[i].arraySize = size;
            markerData[i].markerID = i;

            markerData[i].startEvent = startEvent;
            markerData[i].continueEvent = continueEvent;
            markerData[i].terminateEvent = terminateEvent;
            markerData[i].cannotContinueEvent = cannotContinueEvent;

            markerData[i].cs = &cs;

            DWORD threadId;
            markerThreads[i] = CreateThread(NULL, 0, MarkerThread, &markerData[i], 0, &threadId);
 
            if (markerThreads[i] == NULL) {
                DWORD error = GetLastError();
                std::cerr << "Error creating thread " << i << ". Code: " << error << "\n";
                for (size_t j = 0; j < i; ++j)
                {
                    CloseHandle(markerThreads[j]);
                    CloseHandle(markerData[j].startEvent);
                    CloseHandle(markerData[j].continueEvent);
                    CloseHandle(markerData[j].terminateEvent);
                    CloseHandle(markerData[j].cannotContinueEvent);
                }
                CloseHandle(startEvent);
                CloseHandle(continueEvent);
                CloseHandle(terminateEvent);
                CloseHandle(cannotContinueEvent);

                DeleteCriticalSection(&cs);

                return 1;
            }
 
            std::cout << "Thread marker " << i << " was created (ID: " << threadId << ")\n";
        }

        std::cout << "\nStarting all markers...\n";
        for (size_t i = 0; i < markerCount; ++i)
        {
            SetEvent(markerData[i].startEvent);
        }
        Sleep(100);
        
        int32_t activeTreads = markerCount;

        while (activeTreads > 0)
        {
            std::cout << "\n--- Waiting for all threads to block ---\n";

            std::vector<HANDLE> activeEvents;
            for (int i = 0; i < markerCount; i++) {
                if (markerThreads[i] != NULL) {
                    activeEvents.push_back(markerData[i].cannotContinueEvent);
                }
            }

            if (activeEvents.empty()) {
                break;
            }

            WaitForMultipleObjects(
                static_cast<DWORD>(activeEvents.size()),
                activeEvents.data(), 
                TRUE,
                INFINITE
            );

            std::cout << "All active threads are blocked\n";
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