#include "Listener.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <fcntl.h>

std::unique_ptr<ThreadPool> Listener::threadPool = nullptr;

void Listener::initializeThreadPool(size_t numThreads) {
	if (!threadPool) {
		threadPool = std::make_unique<ThreadPool>(numThreads);  // 동적 생성
	}
}

Listener::Listener(int port, size_t numThreads)
    : port(port), listen_fd(-1), kq(-1), running(false) {

    initializeThreadPool(numThreads);

    // 소켓 생성
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        throw std::runtime_error("Failed to create socket");
    }

    // 소켓 옵션 설정
    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        throw std::runtime_error("Failed to set socket options");
    }

    // 소켓을 논블로킹 모드로 설정
    int flags = fcntl(listen_fd, F_GETFL, 0);
	fcntl(listen_fd, F_SETFL, flags & ~O_NONBLOCK);
    //fcntl(listen_fd, F_SETFL, flags | O_NONBLOCK);

    // 서버 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // 소켓 바인딩
    if (bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        throw std::runtime_error("Failed to bind socket");
    }

    // 소켓 리스닝
    if (listen(listen_fd, 10) < 0) {
        perror("listen");
        throw std::runtime_error("Failed to listen on socket");
    }

    // kqueue 생성
    kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        throw std::runtime_error("Failed to create kqueue");
    }

    // kqueue에 listen_fd 등록 (새 연결 수신 대기)
    struct kevent change;
    EV_SET(&change, listen_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        throw std::runtime_error("Failed to register kqueue event");
    }
}

Listener::~Listener() {
    running = false;
    close(listen_fd);
    close(kq);
}

bool Listener::start() {
    running = true;

    // 클라이언트 연결 수신을 위한 스레드 시작
    std::thread(&Listener::listenForClients, this).detach();

    return true;
}

void Listener::listenForClients() {
    struct kevent event;
    while (running) {
        int nev = kevent(kq, NULL, 0, &event, 1, NULL);
        if (nev == -1) {
            perror("kevent");
            break;
        } else if (nev > 0) {
            if (event.filter == EVFILT_READ) {
                int client_fd = accept(listen_fd, NULL, NULL);
                if (client_fd < 0) {
                    perror("accept");
                } else {
                    std::cout << "Accepted new client connection" << std::endl;
                    Session* session = new Session(client_fd);
                    SessionManager::getInstance().Add(session);
                    threadPool->enqueue([session]() {
                        session->process();
                    });
                }
            }
        }
    }
}

