#pragma once
#include "IPacket.h"
#include "../Session.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <iostream>

class C_CHAT : public IPacket {
public:
    std::wstring name;
    std::wstring chat;

    uint16_t Id() const override {
        return static_cast<uint16_t>(CHAT_PACKET::C_CHAT);
    }

    void Read(const std::vector<uint8_t>& segment) override {
        uint16_t offset = sizeof(uint16_t) * 2;
        const uint8_t* span = segment.data();

        uint16_t nameLen = 0;
        std::memcpy(&nameLen, &span[offset], sizeof(uint16_t));
        offset += sizeof(uint16_t);
        name = std::wstring(reinterpret_cast<const wchar_t*>(&span[offset]), nameLen / sizeof(wchar_t));
        offset += nameLen;

        uint16_t chatLen = 0;
        std::memcpy(&chatLen, &span[offset], sizeof(uint16_t));
        offset += sizeof(uint16_t);
        chat = std::wstring(reinterpret_cast<const wchar_t*>(&span[offset]), chatLen / sizeof(wchar_t));
        offset += chatLen;
    }

    std::vector<uint8_t> Write(Session& session) override {
		uint16_t packetSize = sizeof(uint16_t) // 패킷 전체 크기
			+ sizeof(uint16_t) // 패킷 ID
			+ sizeof(uint16_t) // 이름 길이
			+ static_cast<uint16_t>(name.size() * sizeof(wchar_t)) // 이름 데이터
			+ sizeof(uint16_t) // 채팅 길이
			+ static_cast<uint16_t>(chat.size() * sizeof(wchar_t)); // 채팅 데이터

		// 필요한 크기만큼 버퍼 할당
		std::vector<uint8_t> buffer(packetSize);
        uint16_t offset = 0;
        uint16_t sizeOffset = offset;

        offset += sizeof(uint16_t);

        uint16_t packetId = Id();
        std::memcpy(&buffer[offset], &packetId, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        uint16_t nameLen = static_cast<uint16_t>(name.size() * sizeof(wchar_t));
        std::memcpy(&buffer[offset], &nameLen, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        std::memcpy(&buffer[offset], name.data(), nameLen);
        offset += nameLen;

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

