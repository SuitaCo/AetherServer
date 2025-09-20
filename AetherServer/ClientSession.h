#pragma once
#include <asio.hpp>
#include <memory>
#include <unordered_set>
#include <iostream>
#include <string>
#include <vector>

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
    ClientSession(asio::ip::tcp::socket socket);
    void start();
    void send(const std::string& message);

private:
    void do_read();
    void do_write(std::size_t length);

    asio::ip::tcp::socket socket_;
    std::vector<char> buffer_;  // Используем vector вместо array
    std::size_t bytes_read_{ 0 };
};