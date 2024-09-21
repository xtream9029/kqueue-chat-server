#include "SessionManager.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

// 세션을 추가하는 메서드
bool SessionManager::Add(Session* session) {
    std::lock_guard<std::mutex> lock(mutex_); // 스레드 안전성 보장
    total_sessionId++;  // 새로운 세션 ID 생성
    session->setId(total_sessionId);  // 세션 ID 설정
    sessions[total_sessionId] = session;  // 세션을 맵에 추가
    std::cout << "Session added with ID: " << total_sessionId << std::endl;
    return true;
}

// 세션을 제거하는 메서드
bool SessionManager::Remove(int sessionId) {
    std::lock_guard<std::mutex> lock(mutex_); // 스레드 안전성 보장
    auto it = sessions.find(sessionId);  // 세션 ID를 맵에서 검색
    if (it != sessions.end()) {
        delete it->second;  // 세션 객체 삭제
        sessions.erase(it);  // 맵에서 세션 삭제
        std::cout << "Session removed with ID: " << sessionId << std::endl;
        return true;
    } else {
        std::cerr << "Session ID not found: " << sessionId << std::endl;
        return false;
    }
}

void SessionManager::Broadcast(std::vector<uint8_t> &segment) {
	int count=0;
	Session *arr[100] = {0, };
	{
		std::lock_guard<std::mutex> lock(mutex_); // 스레드 안전성 보장
		for (const auto& pair : sessions) {
			if (count >= 100)
				break;

			arr[count++] = pair.second;  // pair.second가 Session*을 가리킵니다.
		}
	}

	std::cout << "Session count" << count << '\n';

	for(int i=0;i<count;++i) {
		Session *session = arr[i];
		int sendBytes = send(session->getSocketId(), segment.data(), segment.size(), 0);
		std::cout<<"Server Broadcast Bytes = "<<sendBytes<<'\n';
	}
}



