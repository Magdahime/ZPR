#pragma once
#include <set>
#include <memory>
#include <thread>

#include <websocketpp/config/asio.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/message_buffer/message.hpp>

typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;


const uint16_t DEFAULT_PORT = 9002;

class WebsocketServer
{
    websocketpp::server<websocketpp::config::asio_tls> server_;
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections_;
    std::thread thread_;

public:
    WebsocketServer();
    void run(uint16_t port);
    void close();
    void onOpen(websocketpp::connection_hdl hdl);
    void onClose(websocketpp::connection_hdl hdl);
    void onMessage(websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg);
    void sendStringToAll(const std::string &str);
    void spawn();
    context_ptr onTlsInit(websocketpp::connection_hdl hdl)
    {
        namespace asio = websocketpp::lib::asio;

        context_ptr ctx = websocketpp::lib::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);

        try
        {
            ctx->set_options(asio::ssl::context::default_workarounds |
                             asio::ssl::context::no_sslv2 |
                             asio::ssl::context::no_sslv3 |
                             asio::ssl::context::no_tlsv1 |
                             asio::ssl::context::single_dh_use);
            ctx->use_certificate_chain_file("resources/ssl/server.pem");
            ctx->use_private_key_file("resources/ssl/server.pem", asio::ssl::context::pem);

            ctx->use_tmp_dh_file("resources/ssl/dh.pem");

            std::string ciphers;

            ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK";

            if (SSL_CTX_set_cipher_list(ctx->native_handle(), ciphers.c_str()) != 1)
            {
                std::cout << "Error setting cipher list" << std::endl;
            }
        }
        catch (std::exception &e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
        return ctx;
    }
};