#pragma once
#include "IPacket.h"
#include "../Session.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>

class S_CHAT : public IPacket {
public:
    std::wstring chat;

    uint16_t Id() const override {
        return static_cast<uint16_t>(CHAT_PACKET::S_CHAT);
    }

    void Read(const std::vector<uint8_t>& segment) override {
        uint16_t offset = sizeof(uint16_t) * 2;
        const uint8_t* span = segment.data();

        uint16_t chatLen = 0;
        std::memcpy(&chatLen, &span[offset], sizeof(uint16_t));
        offset += sizeof(uint16_t);
        chat = std::wstring(reinterpret_cast<const wchar_t*>(&span[offset]), chatLen / sizeof(wchar_t));
        offset += chatLen;
    }

    std::vector<uint8_t> Write(Session& session) override {
        std::vector<uint8_t> buffer(4096);
        uint16_t offset = 0;
        uint16_t sizeOffset = offset;

        offset += sizeof(uint16_t);

        uint16_t packetId = Id();
        std::memcpy(&buffer[offset], &packetId, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        uint16_t chatLen = static_cast<uint16_t>(chat.size() * sizeof(wchar_t));
        std::memcpy(&buffer[offset], &chatLen, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        std::memcpy(&buffer[offset], chat.data(), chatLen);
        offset += chatLen;

        std::memcpy(&buffer[sizeOffset], &offset, sizeof(uint16_t));

        buffer.resize(offset);
        return buffer;
    }
};

