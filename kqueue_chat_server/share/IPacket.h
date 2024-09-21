#pragma once
#include <vector>

class Session;  // 전방 선언

enum class CHAT_PACKET : uint16_t {
    C_CHAT = 1,
    S_CHAT = 2,
};

class IPacket {
public:
    virtual ~IPacket() {}
    virtual uint16_t Id() const = 0;
    virtual std::vector<uint8_t> Write(Session& session) = 0;
    virtual void Read(const std::vector<uint8_t>& segment) = 0;
};

