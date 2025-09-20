import socket
import threading
import sys
import msvcrt  # Для Windows вместо termios

class WindowsClient:
    def __init__(self, host='localhost', port=8080):
        self.host = host
        self.port = port
        self.socket = None
        self.running = False

    def connect(self):
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            self.running = True
            print(f"Connected to {self.host}:{self.port}")
            print("Type anything - it will be sent immediately!")
            print("Press 'Ctrl+C' to exit\n")
            return True
        except Exception as e:
            print(f"Connection failed: {e}")
            return False

    def receive_messages(self):
        """Поток для приема сообщений от сервера"""
        while self.running:
            try:
                data = self.socket.recv(1024)
                if not data:
                    print("\nServer closed the connection")
                    self.running = False
                    break
                print(f"\n[Server] {data.decode()}", end="")
                print("You: ", end="", flush=True)
            except:
                if self.running:
                    pass  # Игнорируем ошибки при закрытии

    def start_interactive(self):
        """Интерактивный режим с посимвольной отправкой для Windows"""
        if not self.connect():
            return

        # Запускаем поток для приема сообщений
        receiver = threading.Thread(target=self.receive_messages, daemon=True)
        receiver.start()

        try:
            print("You: ", end="", flush=True)
            while self.running:
                # Читаем по одному символу без ожидания Enter
                if msvcrt.kbhit():  # Проверяем, нажата ли клавиша
                    char = msvcrt.getch().decode('utf-8', errors='ignore')
                    
                    # Обработка Ctrl+C
                    if char == '\x03':
                        break
                    
                    # Отправляем символ на сервер
                    try:
                        self.socket.sendall(char.encode())
                        print(char, end="", flush=True)  # Выводим символ
                    except:
                        self.running = False
                        break
                        
        except KeyboardInterrupt:
            print("\nDisconnecting...")
        finally:
            self.running = False
            if self.socket:
                self.socket.close()
            print("\nClient stopped")

if __name__ == "__main__":
    client = WindowsClient('localhost', 33333)  # Укажите ваш порт
    client.start_interactive()