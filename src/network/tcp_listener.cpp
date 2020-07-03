#include "network/tcp_listener.hpp"
#include <iostream>

using namespace ww;

tcp_listener::tcp_listener(boost::asio::io_context &context, boost::asio::ip::tcp::endpoint const &ep, std::weak_ptr<observer> obs)
        : tcp_listener_interface{context, ep, obs} {
    spdlog::debug("TCP server created");
}

/**
 * @brief start the server events loop.
 */
void tcp_listener::run() {
    spdlog::debug("Run TCP server on port {}", m_acceptor.local_endpoint().port());
    start_accept();
}

/**
 * @brief we start to listen for a new connection.
 */
void tcp_listener::start_accept() {
    spdlog::debug("Start listening for incoming TCP connection...");
    std::shared_ptr<tcp_session_interface> new_session = std::make_shared<tcp_session>(m_io_context);
    new_session->add_observer(m_command_observer);

    m_acceptor.async_accept(
            new_session->socket(),
            [this, new_session](boost::system::error_code const &error) {
                handle_accept(new_session, error);
            }
    );
}

/**
 * @brief add the new connection to the set then listen for a new connection again (to avoid the server to stop).
 *
 * @param new_connection
 * @param error
 */
void tcp_listener::handle_accept(std::shared_ptr<tcp_session_interface> new_session,
                                 boost::system::error_code const &error) {
    if (!error) {
        spdlog::info("New connection!");
        m_sessions.insert(new_session);
        spdlog::info("Start the session...");
        new_session->start();
        start_accept();
    } else {
        spdlog::error("[TCP handle accept] {}", error.message());
    }
}
