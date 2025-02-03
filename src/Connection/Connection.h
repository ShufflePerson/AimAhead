#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <thread>

#include <TBasePacket.h>
#include <TKeepAlivePacket.h>
#include <TCrashPacket.h>
#include "../Security/Security.h"


#pragma comment(lib, "ws2_32.lib")
#define SERVER_BUFFER_SIZE 1024

namespace connection {
	bool init_connection();

	void keepalive_loop();

	bool wait_for_security_ok();
	void set_security_ok();
	void set_keepalive(bool val);
	bool get_keepalive();

	bool send_base_packet(TBasePacket& packet);
	bool send_keepalive_packet(TKeepAlivePacket& packet);
	bool send_crash_packet(ECrashReason reason);

	void handle_base_packet(TBasePacket& packet);
	void handle_keepalive_packet(TKeepAlivePacket& packet);
}