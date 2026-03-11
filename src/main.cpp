#include "../include/marker_thread.h"
#include <iostream>
#include <limits>
#include <memory>

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