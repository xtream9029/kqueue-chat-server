#include "PacketManager.h"
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

        uint16_t packetId = 0;
        std::memcpy(&packetId, &data[offset + 2], sizeof(uint16_t));

        std::vector<uint8_t> packet(data.begin() + offset, data.begin() + offset + packetSize);
        offset += packetSize;

		std::cout<<"packetId = "<<packetId<<'\n';
        switch (packetId) {
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

        processLen += packetSize;
    }

    return processLen;
}

void PacketManager::HandleSChat(const S_CHAT& sChatPacket, Session &session) {
	std::wstring chat = sChatPacket.chat;
	std::wcout << chat << std::endl;
}

