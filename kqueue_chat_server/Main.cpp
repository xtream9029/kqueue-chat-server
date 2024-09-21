#include "Listener.h"
#include "ThreadPool.h"
#include <iostream>

int main() {
    int port = 8080; // 서버가 수신 대기할 포트 번호
    size_t numThreads = 4; // 스레드 풀의 스레드 수
	ThreadPool::initializeThreadPool(numThreads);
    try {
        Listener listener(port);

        if (listener.start()) {
            std::cout << "Server started and listening on port " << port << "..." << std::endl;
        } else {
            std::cerr << "Failed to start the server." << std::endl;
            return 1; // 서버 시작 실패 시 오류 코드 반환
        }

        // 메인 스레드는 여기서 추가적인 작업을 수행할 수 있습니다.
        // 예: 서버가 종료될 때까지 대기하는 루프
        while (true) {
            // 서버가 실행 중인 동안 메인 스레드는 다른 작업을 수행하거나 대기할 수 있습니다.
			std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception& ex) {
        std::cerr << "Exception occurred: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

