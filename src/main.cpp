#include "../include/marker_thread.h"
#include <iostream>
#include <limits>
#include <memory>

int main() {
    try {

        

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