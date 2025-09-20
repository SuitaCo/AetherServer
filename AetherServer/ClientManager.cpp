// client_manager.cpp
#include "ClientManager.h"
#include "ClientSession.h"
#include <iostream>

// Реализация Singleton
ClientManager& ClientManager::getInstance() {
    static ClientManager instance; // Создается при первом вызове
    return instance;
}

ClientManager::ClientManager() {
    std::cout << "ClientManager created" << std::endl;
}

ClientManager::~ClientManager() {
    std::cout << "ClientManager destroyed. Clients: " << clients_.size() << std::endl;
}

void ClientManager::addClient(std::shared_ptr<ClientSession> client) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_.insert(client);
    std::cout << "Client added. Total: " << clients_.size() << std::endl;
}

void ClientManager::removeClient(std::shared_ptr<ClientSession> client) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_.erase(client);
    std::cout << "Client removed. Total: " << clients_.size() << std::endl;
}

void ClientManager::broadcastMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (clients_.empty()) {
        std::cout << "No clients to broadcast" << std::endl;
        return;
    }

    for (auto& client : clients_) {
        try {
            client->sendMessage(message);
        }
        catch (const std::exception& e) {
            std::cerr << "Error sending to client: " << e.what() << std::endl;
        }
    }

    std::cout << "Broadcast to " << clients_.size() << " clients: " << message << std::endl;
}

size_t ClientManager::getClientCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return clients_.size();
}

void ClientManager::listClients() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "--- Connected clients: " << clients_.size() << " ---" << std::endl;
    for (const auto& client : clients_) {
        std::cout << "• " << client->getClientInfo() << std::endl;
    }
}