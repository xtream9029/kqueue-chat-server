#pragma once
#include "Session.h"
#include <iostream>
#include <unordered_map>
#include <mutex>

class SessionManager {
public:
    static SessionManager& getInstance() {
        static SessionManager instance;  // 첫 호출 시에만 생성되고 이후에는 동일한 인스턴스를 반환
        return instance;
    }
    bool Add(Session *session);
    bool Remove(int sessionId);
	void Broadcast(std::vector<uint8_t> &segment);
private:
    int total_sessionId = 0;
    std::unordered_map<int, Session*> sessions;
    std::mutex mutex_;    
};
