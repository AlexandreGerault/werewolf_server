#include "network/tcp_session.hpp"
#include "rooms/room_interface.hpp"

using namespace mge;

room_interface::room_interface(const std::string &name) : m_state(room_state::lobby), m_name(name),
                                                          m_config(name + ".config") {

}

/**
 * @brief add a connection to the room's clients set
 *
 * @param client
 */
void room_interface::join(std::shared_ptr<tcp_session> client) {
    m_clients.insert(client);
}

/**
 * @brief remove a connection from a room.
 *
 * @param client
 */
void room_interface::leave(std::shared_ptr<tcp_session> client) {
    m_clients.erase(client);
}
