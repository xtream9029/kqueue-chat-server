#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

	static ThreadPool& getThreadPool() {
		if (!threadPool) {
			throw std::runtime_error("ThreadPool is not initialized.");
		}
		return *threadPool;
	}

    void enqueue(std::function<void()> task);
	static void initializeThreadPool(size_t numThreads);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    static std::unique_ptr<ThreadPool> threadPool;  // 스레드 풀 객체를 포인터로 선언
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

    void workerThread();
};
