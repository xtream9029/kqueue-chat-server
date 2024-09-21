#pragma once
#include "Session.h"
#include "SessionManager.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/event.h>

class Listener {
public:
    Listener(int port);
    ~Listener();
    bool start();

private:
    void listenForClients();  // 클라이언트 연결 수신

    int port;
    int listen_fd;
    int kq;
    struct sockaddr_in serv_addr;
    std::atomic<bool> running;  // 서버 실행 여부를 관리하는 플래그
};

