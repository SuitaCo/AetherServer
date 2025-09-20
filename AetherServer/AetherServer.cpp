// AetherServer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "Server.h"
#include "ClientManager.h"
#include <asio.hpp>
#include <iostream>
#include <csignal>
#include <windows.h>

// Глобальные переменные для обработки сигналов
std::unique_ptr<Server> server_ptr;

BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        std::cout << "\nReceived Ctrl+C, shutting down server..." << std::endl;

        // Останавливаем сервер
        if (server_ptr) {
            server_ptr->stop();
        }

        // Показываем статистику клиентов
        std::cout << "Final client count: "
            << ClientManager::getInstance().getClientCount() << std::endl;

        exit(0);
    }
    return TRUE;
}

int main() {
    try {
        // Настраиваем обработчик Ctrl+C
        SetConsoleCtrlHandler(ConsoleHandler, TRUE);

        // Создаем io_context
        asio::io_context io_context;

        // Создаем сервер на порту 33333
        server_ptr = std::make_unique<Server>(io_context, 33333);

        // Запускаем сервер
        server_ptr->start();

        std::cout << "Server is running on " << server_ptr->getEndpointInfo() << std::endl;
        std::cout << "Connect using: telnet localhost 33333" << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;

        // Запускаем цикл обработки событий
        io_context.run();

    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
