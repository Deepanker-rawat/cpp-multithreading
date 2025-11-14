/*
 Oversubscription occurs when more threads are created than there are available hardware execution
 contexts (CPU cores or hardware threads).
 */
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

void work(int id) {
    std::cout << "Thread " << id << " starting\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread " << id << " done\n";
}

int main() {
    const int thread_count = 20;  // Oversubscription on a 4-core system
    std::vector<std::thread> threads;

    for (int i = 0; i < thread_count; ++i)
        threads.emplace_back(work, i);

    for (auto& t : threads)
        t.join();

    return 0;
}

