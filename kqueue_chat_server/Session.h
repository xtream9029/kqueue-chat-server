#pragma once
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

class Session {
public:
    Session(int client_socket);
    ~Session();
    void process(); 
    void setId(int sessionId);
    int getId();
	int getSocketId();
public:
    int id = 0;

private:
    int client_socket;
};
