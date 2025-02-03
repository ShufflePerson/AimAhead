#include "Connection.h"
#define FAKE_USER_ID 54538182

SOCKET connectSocket = INVALID_SOCKET;
bool security_ok = false;
bool keepalive_ok = false;

bool connection::wait_for_security_ok() {
    int iterations = 0;
    while (!security_ok) {
        if (iterations >= 10) {
            return false;
        }
        iterations++;
        Sleep(100);
    }
    return true;
}

bool connection::get_keepalive() {
    return keepalive_ok;
}

void connection::set_security_ok() {
    security_ok = true;
    keepalive_ok = true;
}

void connection::set_keepalive(bool val) {
    keepalive_ok = val;
}

bool connection::init_connection() {
    WSADATA wsaData;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    int iResult;
    char sendbuf[SERVER_BUFFER_SIZE];
    char recvbuf[SERVER_BUFFER_SIZE];
    int recvbuflen = SERVER_BUFFER_SIZE;


    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(XorStr("localhost"), XorStr("12345"), &hints, &result);
    if (iResult != 0) {
        WSACleanup();
        return false;
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            WSACleanup();
            return false;
        }

        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result); 

    if (connectSocket == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    bool init_packet_sent = false;

    do {
        if (!init_packet_sent) {
            TBasePacket packet;
            packet.messageType = base_packet_get_message_type();
            //TODO set an actual UserID
            packet.user_id = FAKE_USER_ID;
            send_base_packet(packet);
            init_packet_sent = true;
        }

        ZeroMemory(recvbuf, recvbuflen);
        iResult = recv(connectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            if (iResult >= sizeof(uint32_t)) {
                uint32_t messageTypeNetOrder = *(uint32_t*)recvbuf;
                uint32_t messageType = ntoh32(messageTypeNetOrder);

                if (messageType == base_packet_get_message_type()) {
                    TBasePacket packet;
                    packet.deserialize(recvbuf, iResult, packet);
                    connection::handle_base_packet(packet);
                }

                if (messageType == keepalive_packet_get_type()) {
                    TKeepAlivePacket packet;
                    packet.deserialize(recvbuf, iResult, packet);
                    connection::handle_keepalive_packet(packet);
                }
            }
        }
        else {
            set_keepalive(false);
        }
    } while (true);

    iResult = shutdown(connectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        set_keepalive(false);
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }
    set_keepalive(false);

    closesocket(connectSocket);
    WSACleanup();



	return true;
}

bool connection::send_base_packet(TBasePacket& packet) {
    std::vector<char> buffer = packet.serialize(packet);
    int iResult = send(connectSocket, buffer.data(), buffer.size(), 0);

    if (iResult == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool connection::send_keepalive_packet(TKeepAlivePacket& packet) {
    std::vector<char> buffer = packet.serialize(packet);
    int iResult = send(connectSocket, buffer.data(), buffer.size(), 0);

    if (iResult == SOCKET_ERROR) {
        return false;
    }
    return true;
}

void connection::keepalive_loop() {
    while (true) {
        TKeepAlivePacket packet;
        packet.messageType = keepalive_packet_get_type();
        packet.user_id = FAKE_USER_ID;
        set_keepalive(send_keepalive_packet(packet));
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}