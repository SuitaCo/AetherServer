#include "Server.h"

Server::Server(asio::io_context& io_context, int port)
    : io_context_(io_context),
    acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
    std::cout << "Server listening on port " << port << std::endl;
}

Server::~Server() {
    stop();
}

void Server::start() {
    do_accept();
}

void Server::stop() {
    try {
        acceptor_.close();
        sessions_.clear();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in stop: " << e.what() << std::endl;
    }
}

void Server::do_accept() {
    acceptor_.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                // Создаем новую сессию для клиента
                try {
                    auto session = std::make_shared<ClientSession>(std::move(socket));
                    sessions_.insert(session);
                    session->start();
                }
                catch (const std::exception& e) {
                    std::cerr << "Error creating session: " << e.what() << std::endl;
                }
            }
            else {
                if (ec != asio::error::operation_aborted) {
                    std::cerr << "Accept error: " << ec.message() << std::endl;
                }
                return;
            }

            // Продолжаем принимать новые подключения
            do_accept();
        });
}