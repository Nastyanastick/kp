#include <iostream>
#include <string>
#include <winsock2.h>

#include "module/database.h"
#include "module/parser.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    try {
        ensureDataDirectory();

        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return 1;
        }

        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed\n";
            WSACleanup();
            return 1;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(54000);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed\n";
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed\n";
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "DB server started on port 54000\n";

        while (true) {
            SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed\n";
                continue;
            }

            char buffer[16384];
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';

                std::string sql = buffer;
                std::cout << "Request: " << sql << "\n";

                std::string response = executeSQLForServer(sql);

                send(clientSocket, response.c_str(), (int)response.size(), 0);
            }

            closesocket(clientSocket);
        }

        closesocket(serverSocket);
        WSACleanup();

    } catch (const std::exception& e) {
        std::cerr << "Server fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}