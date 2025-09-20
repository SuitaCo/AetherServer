// client_manager.hpp
#pragma once

#include <memory>
#include <unordered_set>
#include <mutex>
#include <string>

class ClientSession; // Предварительное объявление

class ClientManager {
public:
    // Удаляем конструкторы копирования и присваивания
    ClientManager(const ClientManager&) = delete;
    ClientManager& operator=(const ClientManager&) = delete;

    // Статический метод для получения единственного экземпляра
    static ClientManager& getInstance();

    // Основные методы управления клиентами
    void addClient(std::shared_ptr<ClientSession> client);
    void removeClient(std::shared_ptr<ClientSession> client);

    // Рассылка сообщений
    void broadcastMessage(const std::string& message);

    // Информация о клиентах
    size_t getClientCount() const;
    void listClients() const;

private:
    // Приватный конструктор и деструктор
    ClientManager();
    ~ClientManager();

    std::unordered_set<std::shared_ptr<ClientSession>> clients_;
    mutable std::mutex mutex_;
};