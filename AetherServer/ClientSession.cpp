#include "ClientSession.h"
#include <iostream>

ClientSession::ClientSession(asio::ip::tcp::socket socket)
    : socket_(std::move(socket)), buffer_(1024) {  // Инициализируем buffer_
}

void ClientSession::start() {
    try {
        std::cout << "Client connected: "
            << socket_.remote_endpoint().address().to_string()
            << ":" << socket_.remote_endpoint().port() << std::endl;
        do_read();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in start: " << e.what() << std::endl;
    }
}

void ClientSession::do_read() {
    auto self(shared_from_this());

    socket_.async_read_some(asio::buffer(buffer_),
        [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                bytes_read_ = length;

                // Создаем копию данных для безопасного использования
                std::string message(buffer_.data(), length);
                std::cout << "Received from client "  + socket_.remote_endpoint().address().to_string() + ": " << message << std::endl;

                // Эхо-ответ: отправляем обратно те же данные
                do_write(length);

            }
            else {
                if (ec != asio::error::eof) {
                    std::cerr << "Read error: " << ec.message() << std::endl;
                }
                std::cout << "Client disconnected" << std::endl;
            }
        });
}

void ClientSession::do_write(std::size_t length) {
    auto self(shared_from_this());

    // Используем буфер напрямую, без создания временных строк
    asio::async_write(socket_, asio::buffer(buffer_.data(), length),
        [this, self](std::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                // Продолжаем чтение только после успешной записи
                do_read();
            }
            else {
                std::cerr << "Write error: " << ec.message() << std::endl;
            }
        });
}

void ClientSession::send(const std::string& message) {
    // Создаем копию сообщения для асинхронной операции
    auto self(shared_from_this());
    auto message_copy = std::make_shared<std::string>(message);

    asio::async_write(socket_, asio::buffer(*message_copy),
        [this, self, message_copy](std::error_code ec, std::size_t /*length*/) {
            if (ec) {
                std::cerr << "Send error: " << ec.message() << std::endl;
            }
        });
}

