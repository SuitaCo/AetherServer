// client_manager.hpp
#pragma once

#include <memory>
#include <unordered_set>
#include <mutex>
#include <string>

class ClientSession; // ��������������� ����������

class ClientManager {
public:
    // ������� ������������ ����������� � ������������
    ClientManager(const ClientManager&) = delete;
    ClientManager& operator=(const ClientManager&) = delete;

    // ����������� ����� ��� ��������� ������������� ����������
    static ClientManager& getInstance();

    // �������� ������ ���������� ���������
    void addClient(std::shared_ptr<ClientSession> client);
    void removeClient(std::shared_ptr<ClientSession> client);

    // �������� ���������
    void broadcastMessage(const std::string& message);

    // ���������� � ��������
    size_t getClientCount() const;
    void listClients() const;

private:
    // ��������� ����������� � ����������
    ClientManager();
    ~ClientManager();

    std::unordered_set<std::shared_ptr<ClientSession>> clients_;
    mutable std::mutex mutex_;
};