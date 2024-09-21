#include "Session.h"
#include "PacketManager.h"
#include "share/C_CHAT.h"
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
    char buffer[4096*4];
    while (true) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0); 
        if (bytes_received > 0) {
            std::vector<uint8_t> buf(buffer, buffer + bytes_received);
            int processLen = PacketManager::getInstance().ProcessPacket(buf, bytes_received, this);
            std::cout << "Process Len = " << processLen << '\n';
        } else if (bytes_received == 0) {
            // 클라이언트가 연결을 닫은 경우
            std::cout << "Client disconnected.\n";
        } else {
            // 오류 처리 (예: recv 에러)
            std::cerr << "Error receiving data.\n";
        }
        // CPU 사용률을 줄이기 위해 대기 시간을 추가
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Session::start() {
    worker_thread = std::thread(&Session::process, this);  // 스레드 시작
}

void Session::setId(int sessionId) {
    id = sessionId;
    std::cout << "Session ID set to: " << id << std::endl;
}

