#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "share/C_CHAT.h"
#include "share/S_CHAT.h"
#include "PacketManager.h"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // 서버 주소 설정 (127.0.0.1은 로컬 서버를 의미)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // 서버에 연결 시도
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

	Session* session = new Session(sock);
	session->start();

	std::cout<<"Please Enter Your Name:";
	std::wstring name;
	std::wstring msg;
	std::wcin>>name;
    
	C_CHAT c_chat;
    char buffer[4096*4];

	while (true) {
		std::wcin>>msg;
		c_chat.name = name;
		c_chat.chat = msg;

		std::vector<uint8_t> serializedData = c_chat.Write(*session);
		send(sock, serializedData.data(), serializedData.size(), 0);
	}

    close(sock);
	delete session;

    return 0;
}

