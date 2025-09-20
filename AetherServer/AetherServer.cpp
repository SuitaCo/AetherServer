// AetherServer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "Server.h"
#include <asio.hpp>
#include <iostream>
#include <csignal>
#include <atomic>
#include <locale>
#include <codecvt>
#include <io.h>
#include <fcntl.h>

void setup_console_encoding() {
    // Устанавливаем локаль для консоли
    setlocale(LC_ALL, "Russian");

    // Настраиваем кодировку вывода для Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Для широких символов
    std::wcout.imbue(std::locale("Russian"));
    std::cout.imbue(std::locale("Russian"));
}

std::unique_ptr<Server> server;
std::atomic<bool> running{ true };

void signal_handler(int signum) {
    std::cout << "\nReceived signal " << signum << ", shutting down server..." << std::endl;
    running = false;
    if (server) {
        server->stop();
    }
}

int main() {
    setup_console_encoding();
    try {
        asio::io_context io_context;

        // Регистрируем обработчик сигналов для graceful shutdown
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        // Создаем сервер на порту 33333
        server = std::make_unique<Server>(io_context, 33333);
        server->start();

        std::cout << "TCP Server started. Press Ctrl+C to stop." << std::endl;

        // Запускаем обработку событий
        while (running) {
            try {
                io_context.run();
                break; // run() завершится когда не останется работы
            }
            catch (const std::exception& e) {
                std::cerr << "Exception in io_context.run(): " << e.what() << std::endl;
                if (!running) break;
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Server stopped gracefully." << std::endl;
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
