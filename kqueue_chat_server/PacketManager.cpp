#include "PacketManager.h"
#include "SessionManager.h"
#include "RedisHelper.h"
#include <iostream>
#include <cstring>

PacketManager& PacketManager::getInstance() {
    static PacketManager instance;
    return instance;
}

PacketManager::PacketManager() {}
PacketManager::~PacketManager() {}

int PacketManager::ProcessPacket(const std::vector<uint8_t>& data, int length, Session *session) {
    int processLen = 0;
    int offset = 0;

    int totalLen = length;

    while (true) {
        int headerSize = 4;
        if (totalLen - offset < headerSize) break;

        uint16_t packetSize = 0;
        std::memcpy(&packetSize, &data[offset], sizeof(uint16_t));
        if (totalLen - offset < packetSize) break;
		std::cout<<"PacketSize = "<<packetSize<<'\n';

        uint16_t packetId = 0;
        std::memcpy(&packetId, &data[offset+2], sizeof(uint16_t));
		std::cout<<"PacketId = "<<packetId<<'\n';

        std::vector<uint8_t> packet(data.begin() + offset, data.begin() + offset + packetSize);
        offset += packetSize;

        switch (packetId) {
            case 1: {
                C_CHAT cChatPacket;
				cChatPacket.Read(packet);
				HandleCChat(cChatPacket, *session);
                break;
            }
            case 2: {
                S_CHAT sChatPacket;
				sChatPacket.Read(packet);
				HandleSChat(sChatPacket, *session);
                break;
            }
			default:
				std::cout<<"! Process Packet error"<<packetId<<'\n';
				abort();
        }

		std::cout<<"PacketManager Process TestLog"<<packetId<<'\n';
        processLen += packetSize;
    }

    return processLen;
}

void PacketManager::HandleCChat(const C_CHAT& cChatPacket, Session &session) {
	std::wstring name = cChatPacket.name;
	std::wstring chat = cChatPacket.chat;
	std::wcout << L"C_CHAT TEST Name[" << name << L"] Chat[" << chat << L"]" << std::endl;

	// Redis에 name과 chat을 저장하기 위한 호출
	RedisHelper& redis = RedisHelper::getInstance();
	std::string nameStr(name.begin(), name.end());  // wstring을 string으로 변환
	std::string chatStr(chat.begin(), chat.end());  // wstring을 string으로 변환

	if (redis.set(nameStr, chatStr)) {
		std::wcout << L"Stored in Redis: Name[" << name << L"], Chat[" << chat << L"]" << std::endl;
	} else {
		std::wcout << L"Failed to store in Redis" << std::endl;
	}

	S_CHAT sChatPacket;
	std::wstring delimiter = L":";
	sChatPacket.chat = name + delimiter + chat;
	std::wcout << L"S_CHAT Response Chat[" << sChatPacket.chat << L"]" << '\n';
	std::vector<uint8_t> serializedData = sChatPacket.Write(session);
	SessionManager::getInstance().Broadcast(serializedData);
}

void PacketManager::HandleSChat(const S_CHAT& sChatPacket, Session &session) {
	std::wstring chat = sChatPacket.chat;
	std::wcout << chat << std::endl;
}

