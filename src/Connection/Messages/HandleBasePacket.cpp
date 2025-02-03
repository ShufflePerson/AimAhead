#include "../Connection.h"


void connection::handle_base_packet(TBasePacket& packet) {
    if (packet.user_id != get_login_passed_code()) {
        connection::send_crash_packet(ECRASHREASON_SERVER_SAID_SO);
        security::breach_detected();
    }
    else {
        connection::set_security_ok();
        std::thread keepalive_thread(connection::keepalive_loop);
        keepalive_thread.detach();
    }
}