#include "../Connection.h"


void connection::handle_keepalive_packet(TKeepAlivePacket& packet) {
    if (packet.user_id != get_good_keepalive_code()) {
        security::breach_detected();
    }
    else {
        connection::set_keepalive(true);
    }
}