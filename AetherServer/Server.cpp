#include "Server.h"
#include "ClientSession.h"
#include <iostream>
#include <system_error>

Server::Server(asio::io_context& io_context, std::uint16_t port)
    : io_context_(io_context)
    , acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    , socket_(io_context) {

    // Сохраняем информацию о endpoint сервера
    try {
        auto endpoint = acceptor_.local_endpoint();
        endpoint_info_ = endpoint.address().to_string() + ":" +
            std::to_string(endpoint.port());
    }
    catch (const std::exception& e) {
        endpoint_info_ = "unknown";
    }

    std::cout << "Server constructed on " << endpoint_info_ << std::endl;
}

void Server::start() {
    if (is_running_) {
        std::cout << "Server is already running" << std::endl;
        return;
    }

    is_running_ = true;
    std::cout << "Starting server on " << endpoint_info_ << std::endl;

    // Начинаем принимать подключения
    do_accept();
}

void Server::stop() {
    if (!is_running_) {
        return;
    }

    is_running_ = false;
    std::cout << "Stopping server..." << std::endl;

    // Закрываем акцептор
    try {
        if (acceptor_.is_open()) {
            acceptor_.close();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error closing acceptor: " << e.what() << std::endl;
    }
}

void Server::do_accept() {
    if (!is_running_) {
        return;
    }

    // Асинхронно ждем нового подключения
    acceptor_.async_accept(
        socket_,
        [this](std::error_code ec) {
            if (!ec) {
                // Успешно приняли подключение
                try {
                    // Создаем сессию для нового клиента
                    auto session = std::make_shared<ClientSession>(std::move(socket_));

                    // Запускаем сессию (она сама добавится в ClientManager)
                    session->start();

                    std::cout << "New client connected" << std::endl;

                }
                catch (const std::exception& e) {
                    std::cerr << "Error creating session: " << e.what() << std::endl;
                }

                // Подготавливаем сокет для следующего подключения
                socket_ = asio::ip::tcp::socket(io_context_);

            }
            else {
                // Обрабатываем ошибку
                handle_accept_error(ec);
            }

            // Продолжаем принимать подключения (если сервер еще работает)
            if (is_running_) {
                do_accept();
            }
        }
    );

    std::cout << "Listening for connections..." << std::endl;
}

void Server::handle_accept_error(const std::error_code& ec) {
    if (ec == asio::error::operation_aborted) {
        // Сервер остановлен - это нормально
        std::cout << "Accept operation cancelled (server stopping)" << std::endl;
    }
    else {
        // Другая ошибка
        std::cerr << "Accept error: " << ec.message() << std::endl;

        // Можно добавить паузу перед повторной попыткой
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

std::string Server::getEndpointInfo() const {
    return endpoint_info_;
}

bool Server::isRunning() const {
    return is_running_;
}
