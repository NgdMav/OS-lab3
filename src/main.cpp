#include "../include/thread_functions.h"
#include <iostream>
#include <limits>
#include <memory>

int main() {
    try {
        int size = 0;
        
        std::cout << "=== Lab 2: Creating Threads ===\n";
        std::cout << "\nEnter the array size:";
        std::cin >> size;

        if (!CheckArraySize(size)) {
            return 1;
        }

        std::unique_ptr<int[]> array(new int[size]);
        
        std::cout << "Enter " << size << " integers:\n";
        for (int i = 0; i < size; ++i) {
            std::cout << "Element [" << i << "]: ";
            std::cin >> array[i];
        }

        PrintArray(array.get(), size, "Start array:");

        MinMaxData minMaxData = {array.get(), size, 0, 0, 0, 0};
        AverageData avgData = {array.get(), size, 0.0};

        HANDLE hMinMaxThread = nullptr;
        HANDLE hAverageThread = nullptr;
        DWORD threadID = 0;

        try {
            // Создание потока min_max
            hMinMaxThread = CreateThread(
                nullptr,                    // атрибуты безопасности по умолчанию
                0,                          // размер стека по умолчанию
                MinMaxThread,               // функция потока
                &minMaxData,                // параметр функции
                0,                          // флаги создания
                &threadID                   // ID потока
            );

            if (hMinMaxThread == nullptr) {
                DWORD error = GetLastError();
                std::cerr << "Error creating min_max thread. Error code: " << error << '\n';
                return 1;
            }

            std::cout << "\n[Main] Thread min_max created (ID: " << threadID << ")\n";

            // Создание потока average
            hAverageThread = CreateThread(
                nullptr,                    // атрибуты безопасности по умолчанию
                0,                          // размер стека по умолчанию
                AverageThread,              // функция потока
                &avgData,                   // параметр функции
                0,                          // флаги создания
                &threadID                   // ID потока
            );

            if (hAverageThread == nullptr) {
                DWORD error = GetLastError();
                std::cerr << "Error creating average thread. Error code: " << error << '\n';
                CloseHandle(hMinMaxThread);
                return 1;
            }

            std::cout << "[Main] Thread average has been created. (ID: " << threadID << ")\n";
            std::cout << "\n[Main] Waiting for threads to complete...\n";

            WaitForSingleObject(hMinMaxThread, INFINITE);
            WaitForSingleObject(hAverageThread, INFINITE);

            std::cout << "\n[Main] All threads have completed\n";

            DWORD exitCode = 0;
            if (GetExitCodeThread(hMinMaxThread, &exitCode) && exitCode != 0) {
                std::cerr << "Warning: Thread min_max terminated with code " << exitCode << '\n';
            }
            if (GetExitCodeThread(hAverageThread, &exitCode) && exitCode != 0) {
                std::cerr << "Warning: Thread average terminated with code " << exitCode << '\n';
            }

            int avgRounded = static_cast<int>(avgData.averageValue);
            
            std::cout << "\n[Main] Replacing min and max elements with the average value (" 
                 << avgRounded << ")...\n";

            array[minMaxData.minIndex] = avgRounded;
            array[minMaxData.maxIndex] = avgRounded;

            PrintArray(array.get(), size, "Result array:");

            std::cout << "\n=== The program was completed successfully. ===\n";

        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what();
            
            if (hAverageThread) CloseHandle(hAverageThread);
            if (hMinMaxThread) CloseHandle(hMinMaxThread);

            system("pause");
            
            return 1;
        }

        CloseHandle(hAverageThread);
        CloseHandle(hMinMaxThread);

        system("pause");

        return 0;

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