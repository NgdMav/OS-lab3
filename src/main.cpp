#include "../include/marker_thread.h"
#include <iostream>
#include <limits>
#include <memory>

int main() {
    try {
        int size = 0;
        
        std::cout << "=== Lab 3: Threads Synchronization ===\n";
        std::cout << "\nEnter the array size:";
        std::cin >> size;

        if (!CheckArraySize(size)) {
            return 1;
        }

        std::unique_ptr<int[]> array(new int[size]);
        for(size_t i = 0; i < size; i++) {
            array[i] = 0;
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