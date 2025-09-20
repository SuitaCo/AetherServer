#pragma once

#include <asio.hpp>
#include <memory>
#include <array>
#include <string>
#include <iostream>

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
    explicit ClientSession(asio::ip::tcp::socket socket);
    ~ClientSession();

    void start();
    void sendMessage(const std::string& message);
    std::string getClientInfo() const;
    bool isConnected() const;

private:
    void do_read();
    void handleMessage(const std::string& message);

    asio::ip::tcp::socket socket_;
    std::array<char, 8192> buffer_; // Увеличил буфер для больших сообщений
    std::string client_address_;
    bool connected_ = false;
};