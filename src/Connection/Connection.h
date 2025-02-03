#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <TBasePacket.h>

#include "../Security/Security.h"

#pragma comment(lib, "ws2_32.lib")
#define SERVER_BUFFER_SIZE 1024

namespace connection {
	bool init_connection();


	bool wait_for_security_ok();
	void set_security_ok();

	bool send_packet(TBasePacket& packet);


	void handle_base_packet(TBasePacket& packet);
}