#pragma once
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>

class Session {
public:
    Session(int client_socket);
    ~Session();
    void process(); 
    void setId(int sessionId);
	void start();
public:
    int id = 0;
    std::thread worker_thread;

private:
    int client_socket;
};
