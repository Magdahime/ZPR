#include "WebsocketServer.h"

using websocketpp::connection_hdl;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

WebsocketServer::WebsocketServer()
{
    server_.init_asio();
    server_.set_open_handler(bind(&WebsocketServer::onOpen, this, ::_1));
    server_.set_close_handler(bind(&WebsocketServer::onClose, this, ::_1));
    server_.set_message_handler(bind(&WebsocketServer::onMessage, this, ::_1, ::_2));
    server_.set_tls_init_handler(bind(&WebsocketServer::onTlsInit, this, ::_1));
}

void WebsocketServer::spawn()
{
    thread_ = std::thread([this] { this->run(DEFAULT_PORT); });
}

void WebsocketServer::run(uint16_t port)
{
    server_.listen(9002);
    server_.start_accept();
    server_.run();
};
void WebsocketServer::onOpen(connection_hdl hdl)
{
    connections_.insert(hdl);
};
void WebsocketServer::onClose(connection_hdl hdl)
{
    connections_.erase(hdl);
};
void WebsocketServer::onMessage(connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg)
{
    for (auto it : connections_)
    {
        server_.send(it, msg);
        server_.send(it, "Hejka", websocketpp::frame::opcode::text);
    }
};
void WebsocketServer::sendStringToAll(const std::string &str)
{
    for (auto it : connections_)
    {
        server_.send(it, str, websocketpp::frame::opcode::text);
    }
};

void WebsocketServer::close()
{
    for (auto it : connections_)
    {
        onClose(it);
    }
};
