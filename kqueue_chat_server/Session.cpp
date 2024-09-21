#include "Session.h"
#include "share/C_CHAT.h"
#include "SessionManager.h"
#include "PacketManager.h"
#include "ThreadPool.h"
#include <cstring>

Session::Session(int client_socket)
    : client_socket(client_socket) {
    // 세션 초기화 코드가 필요하다면 여기서 추가
    std::cout << "Session created with socket: " << client_socket << std::endl;
}

Session::~Session() {
    // 소켓을 닫고 세션을 정리
    if (client_socket >= 0) {
        close(client_socket);
        std::cout << "Session with socket " << client_socket << " closed." << std::endl;
    }
}

void Session::process() {
	// 패킷 수신을 위한 버퍼 설정
    char buffer[4096*4];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0); 
	if (bytes_received > 0) {
		std::vector<uint8_t> buf(buffer, buffer + bytes_received);
		int processLen = PacketManager::getInstance().ProcessPacket(buf, bytes_received, this);
		std::cout << "Process Len = " << processLen << '\n';
		ThreadPool::getThreadPool().enqueue([this]() {this->process();});
	} else  {
		// 클라이언트가 연결을 닫은 경우
		SessionManager::getInstance().Remove(id);
		std::cout << "Client disconnected.\n";
	} 
}

void Session::setId(int sessionId) {
    id = sessionId;
    std::cout << "Session ID set to: " << id << std::endl;
}

int Session::getId() {
	return id;	
}
	
int Session::getSocketId() {
	return client_socket;	
}


