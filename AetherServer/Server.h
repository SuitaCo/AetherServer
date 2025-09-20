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
    // Конструктор: принимает io_context и порт
    Server(asio::io_context& io_context, std::uint16_t port);

    // Запрещаем копирование и присваивание
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    // Запускает сервер (начинает принимать подключения)
    void start();

    // Останавливает сервер (опционально)
    void stop();

    // Получить информацию о сервере
    std::string getEndpointInfo() const;
    bool isRunning() const;

private:
    // Асинхронно ожидает нового подключения
    void do_accept();

    // Обрабатывает ошибки принятия подключения
    void handle_accept_error(const std::error_code& ec);

    // Ссылка на io_context (передается извне)
    asio::io_context& io_context_;

    // Акцептор для принятия подключений
    asio::ip::tcp::acceptor acceptor_;

    // Временный сокет для нового подключения
    asio::ip::tcp::socket socket_;

    // Флаг состояния сервера
    bool is_running_ = false;

    // Информация о endpoint сервера
    std::string endpoint_info_;
};



