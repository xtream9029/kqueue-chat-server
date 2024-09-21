#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads)
    : stop(false) {
    // numThreads만큼 스레드를 생성하여 workerThread 메서드를 실행
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this]() { this->workerThread(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join(); // 모든 스레드가 종료될 때까지 기다림
        }
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task); // 작업을 큐에 추가
    }

    condition.notify_one(); // 대기 중인 스레드 하나를 깨워서 작업을 처리하게 함
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() { return this->stop || !this->tasks.empty(); });

            if (this->stop && this->tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        task(); // 작업을 실행
    }
}

