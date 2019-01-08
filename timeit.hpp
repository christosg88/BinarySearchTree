#ifndef TIMEIT_TIMEIT_HPP
#define TIMEIT_TIMEIT_HPP

#include <iostream>
#include <string>
#include <chrono>

namespace ti {
    class ti {
    private:
        bool running;
        std::string message;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_point, stop_point;

        void printStart() {
            std::cout << "[T] Timing [" << message << "]\n";
        }

        void printStop() {
            std::cout << "[T] Timing [" << message << "] completed in "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(stop_point - start_point).count()
                      << " ms" << std::endl;
        }

    public:
        ti() : running(true), message(""), start_point(std::chrono::high_resolution_clock::now()) {
            printStart();
        }

        ti(std::string message) :
                running(true), message(std::move(message)), start_point(std::chrono::high_resolution_clock::now()) {
            printStart();
        }

        ~ti() {
            if (running) {
                running = false;
                stop_point = std::chrono::high_resolution_clock::now();
                printStop();
            }
        }

        void reset() {
            running = true;
            start_point = std::chrono::high_resolution_clock::now();
            printStart();
        }

        void finish() {
            running = false;
            stop_point = std::chrono::high_resolution_clock::now();
            printStop();
        }
    };
}

#endif
