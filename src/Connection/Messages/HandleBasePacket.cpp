#include "../Connection.h"


void connection::handle_base_packet(TBasePacket& packet) {
    if (packet.user_id != get_login_passed_code()) {
        security::breach_detected();
    }
    else {
        connection::set_security_ok();
    }
}