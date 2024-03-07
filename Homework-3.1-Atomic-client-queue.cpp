#include <iostream>
#include <algorithm>
#include <execution>
#include <thread>
#include <chrono>
#include <atomic>

const unsigned int MAX_CLIENTS = 5;

void client(std::atomic<int>& queue) {
    for (; queue.load(std::memory_order_acquire) < MAX_CLIENTS; ++queue) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "New client ..." << "\tQueue: " << (queue.load(std::memory_order_relaxed) + 1) << std::endl;
    }
}

void q_operator(std::atomic<int>& queue) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    for (; queue.load(std::memory_order_acquire) > 0; --queue) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "Client served ..." << "\tQueue: " << (queue.load(std::memory_order_relaxed) - 1) << std::endl;
    }
    std::cout << "No clients left" << std::endl;
}

int main()
{
    std::atomic<int> queue(0);

    std::thread client_tr(client, std::ref(queue));
    std::thread q_operator_tr(q_operator, std::ref(queue));

    client_tr.join();
    q_operator_tr.join();

    return 0;
}