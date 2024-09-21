#pragma once
#include <vector>
#include <cstdint>
#include "Session.h"
#include "share/C_CHAT.h"  // Protobuf로 생성된 C_CHAT 헤더 파일
#include "share/S_CHAT.h"  // Protobuf로 생성된 S_CHAT 헤더 파일

class PacketManager {
public:
    static PacketManager& getInstance();
    int ProcessPacket(const std::vector<uint8_t>& data, int length, Session *session);

private:
    PacketManager();
    ~PacketManager();

    void HandleCChat(const C_CHAT& cChatPacket, Session &session);
    void HandleSChat(const S_CHAT& sChatPacket, Session &session);
};

