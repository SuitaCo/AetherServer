#pragma once
#include <asio.hpp>
#include <memory>
#include <unordered_set>
#include <iostream>
#include <functional>
#include "ClientSession.h"

class Server {
public:
    Server(asio::io_context& io_context, int port);
    ~Server();

    void start();
    void stop();

private:
    void do_accept();

    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
    std::unordered_set<std::shared_ptr<ClientSession>> sessions_;
};



