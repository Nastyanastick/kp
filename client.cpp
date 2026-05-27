#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    std::cout << "DB client started\n";
    std::cout << "Enter SQL commands ending with ';'\n";
    std::cout << "Type EXIT; to quit\n";

    std::string line;
    std::string buffer;

    while (true) {
        std::cout << "sql> ";

        std::getline(std::cin, line);

        if (!std::cin) {
            break;
        }

        buffer += line + "\n";

        if (line == "EXIT;" || line == "exit;") {
            break;
        }

        if (buffer.find(';') == std::string::npos) {
            continue;
        }

        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Error: socket creation failed\n";
            buffer.clear();
            continue;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(54000);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cout << "Error: cannot connect to server\n";
            closesocket(clientSocket);
            buffer.clear();
            continue;
        }

        send(clientSocket, buffer.c_str(), (int)buffer.size(), 0);

        char response[4096];
        int bytesReceived;
        bool gotResponse = false;

        while ((bytesReceived = recv(clientSocket, response, sizeof(response) - 1, 0)) > 0) {
            response[bytesReceived] = '\0';
            std::cout << response;
            gotResponse = true;
        }

        if (!gotResponse) {
            std::cout << "Error: empty response from server\n";
        }

        closesocket(clientSocket);
        buffer.clear();
    }

    WSACleanup();
    return 0;
}