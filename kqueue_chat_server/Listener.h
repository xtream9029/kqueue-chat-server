#pragma once
#include "Session.h"
#include "SessionManager.h"
#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/event.h>

class Listener {
public:
    Listener(int port, size_t numThreads);
    ~Listener();
    bool start();

	static ThreadPool& getThreadPool() {
		if (!threadPool) {
			throw std::runtime_error("ThreadPool is not initialized.");
		}
		return *threadPool;
	}

private:
    void listenForClients();  // 클라이언트 연결 수신
	void initializeThreadPool(size_t numThreads);

    int port;
    int listen_fd;
    int kq;
    struct sockaddr_in serv_addr;
    static std::unique_ptr<ThreadPool> threadPool;  // 스레드 풀 객체를 포인터로 선언
    std::atomic<bool> running;  // 서버 실행 여부를 관리하는 플래그
};

