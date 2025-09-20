#pragma once
#include <asio.hpp>
#include <memory>
#include <unordered_set>
#include <iostream>
#include <functional>
#include "ClientSession.h"

#pragma once

#include <asio.hpp>
#include <memory>
#include <cstdint>

class Server {
public:
    // �����������: ��������� io_context � ����
    Server(asio::io_context& io_context, std::uint16_t port);

    // ��������� ����������� � ������������
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    // ��������� ������ (�������� ��������� �����������)
    void start();

    // ������������� ������ (�����������)
    void stop();

    // �������� ���������� � �������
    std::string getEndpointInfo() const;
    bool isRunning() const;

private:
    // ���������� ������� ������ �����������
    void do_accept();

    // ������������ ������ �������� �����������
    void handle_accept_error(const std::error_code& ec);

    // ������ �� io_context (���������� �����)
    asio::io_context& io_context_;

    // �������� ��� �������� �����������
    asio::ip::tcp::acceptor acceptor_;

    // ��������� ����� ��� ������ �����������
    asio::ip::tcp::socket socket_;

    // ���� ��������� �������
    bool is_running_ = false;

    // ���������� � endpoint �������
    std::string endpoint_info_;
};



