#include "ClientSession.h"
#include "ClientManager.h"

ClientSession::ClientSession(asio::ip::tcp::socket socket)
    : socket_(std::move(socket))
    , connected_(false) {

    try {
        // Получаем информацию о клиенте
        auto endpoint = socket_.remote_endpoint();
        client_address_ = endpoint.address().to_string() + ":" +
            std::to_string(endpoint.port());
    }
    catch (const std::exception& e) {
        client_address_ = "unknown";
    }

    std::cout << "ClientSession created: " << client_address_ << std::endl;
}

ClientSession::~ClientSession() {
    if (connected_) {
        std::cout << "ClientSession destroyed: " << client_address_ << std::endl;
    }
}

void ClientSession::start() {
    if (connected_) {
        return;
    }

    connected_ = true;

    // Добавляем в менеджер клиентов
    ClientManager::getInstance().addClient(shared_from_this());

    std::cout << "Session started: " << client_address_ << std::endl;

    // Отправляем приветственное сообщение
    sendMessage("Welcome to server! Your address: " + client_address_ + "\r\n");

    // Начинаем асинхронное чтение
    do_read();
}

void ClientSession::sendMessage(const std::string& message) {
    if (!connected_) {
        return;
    }

    // Синхронная отправка (для простоты)
    try {
        asio::write(socket_, asio::buffer(message + "\r\n"));
    }
    catch (const std::exception& e) {
        std::cerr << "Send error to " << client_address_ << ": " << e.what() << std::endl;
        connected_ = false;
    }
}

std::string ClientSession::getClientInfo() const {
    return client_address_;
}

bool ClientSession::isConnected() const {
    return connected_;
}

void ClientSession::do_read() {
    if (!connected_) {
        return;
    }

    auto self(shared_from_this());

    socket_.async_read_some(
        asio::buffer(buffer_),
        [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                // Преобразуем полученные данные в строку
                std::string message(buffer_.data(), length);

                // Обрабатываем сообщение
                handleMessage(message);

                // Продолжаем чтение
                do_read();
            }
            else {
                // Ошибка или отключение
                connected_ = false;

                if (ec == asio::error::eof) {
                    std::cout << "Client disconnected: " << client_address_ << std::endl;
                }
                else {
                    std::cerr << "Read error from " << client_address_ << ": " << ec.message() << std::endl;
                }

                // Удаляем из менеджера
                ClientManager::getInstance().removeClient(shared_from_this());
            }
        }
    );
}

void ClientSession::handleMessage(const std::string& message) {
    // Убираем лишние символы (возврат каретки, перевод строки)
    std::string clean_message = message;
    clean_message.erase(std::remove(clean_message.begin(), clean_message.end(), '\r'), clean_message.end());
    clean_message.erase(std::remove(clean_message.begin(), clean_message.end(), '\n'), clean_message.end());

    if (clean_message.empty()) {
        return;
    }

    std::cout << "From " << client_address_ << ": " << clean_message << std::endl;

    // Рассылаем сообщение всем клиентам
    std::string broadcast_msg = "[" + client_address_ + "]: " + clean_message;
    ClientManager::getInstance().broadcastMessage(broadcast_msg);
}