#include "../include/marker_thread.h"
#include <iostream>
#include <iomanip>

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    if (!lpParam) {
        std::cerr << "MinMaxThread: Invalid parameter\n";
        return 1;
    }

    MinMaxData* data = static_cast<MinMaxData*>(lpParam);
    
    if (!data->array || data->size <= 0) {
        std::cerr << "MinMaxThread: Invalid array data\n";
        return 1;
    }

    std::cout << "\n[MinMaxThread] Thread min_max has started working.\n";

    data->minValue = data->array[0];
    data->maxValue = data->array[0];
    data->minIndex = 0;
    data->maxIndex = 0;

    for (int i = 1; i < data->size; ++i) {
        if (data->array[i] < data->minValue) {
            data->minValue = data->array[i];
            data->minIndex = i;
        }

        Sleep(7);
        
        if (data->array[i] > data->maxValue) {
            data->maxValue = data->array[i];
            data->maxIndex = i;
        }

        Sleep(7);
    }

    std::cout << "[MinMaxThread] Minimum element: " << data->minValue << " (index " << data->minIndex << ")\n";
    std::cout << "[MinMaxThread] Maximum element: " << data->maxValue << " (index " << data->maxIndex << ")\n";
    std::cout << "[MinMaxThread] Thread min_max has completed its work.\n";

    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    if (!lpParam) {
        std::cerr << "AverageThread: Invalid parameter\n";
        return 1;
    }

    AverageData* data = static_cast<AverageData*>(lpParam);
    
    if (!data->array || data->size <= 0) {
        std::cerr << "AverageThread: Invalid array data\n";
        return 1;
    }

    std::cout << "\n[AverageThread] Thread average has started working.\n";

    long long sum = 0;

    for (int i = 0; i < data->size; ++i) {
        sum += data->array[i];
        
        Sleep(12);
    }

    data->averageValue = static_cast<double>(sum) / data->size;

    std::cout << "[AverageThread] Arithmetic mean: " 
         << std::fixed << std::setprecision(2) << data->averageValue << '\n';
    std::cout << "[AverageThread] Thread average has completed its work.\n";

    return 0;
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