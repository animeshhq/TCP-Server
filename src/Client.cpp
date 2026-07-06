#include "thread"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "../include/Logger.hpp"

void recieveMessages( SOCKET acceptSock) {

    char buffer[BUFFER_SIZE];
    int byteRecieved;

    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        byteRecieved = recv(acceptSock, buffer, BUFFER_SIZE - 1, 0);

        if (byteRecieved > 0) {
            std::cout << "\n[Client]: " << std::string(buffer, byteRecieved) << "\n[You]: ";
            std::cout.flush();
        } else if(byteRecieved == 0) {
            std::cout << "\n[System]: Client disconnected.\n";
            break;
        } else {
            std::cerr << "\n[System]: recv failed with error: " << WSAGetLastError() << "\n";
            break;
        }
    }
}

int main() {

    //loading all dependencies here

    info("Loading all the dependencies....");

    SOCKET clientSock;

    int port = 25365;
    WSADATA wsadata;
    int wsaerr;

    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsadata);

    if (wsaerr != 0){
        error("Winsock DLL not found.");
        return 0;
    } else {
        success(std::string("Status : ") + wsadata.szSystemStatus);
    }

    // creating the client socket using the loaded DLL

    info("Creating Socket....");

    clientSock = INVALID_SOCKET;
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(clientSock == INVALID_SOCKET) {
        error("Socket creation failed. Error Code : " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return 0;
    } else {
        success("Socket created successfully!");
    }

    // since client side connects automatically, we dont need to bind it like the server

    info("Connecting with the server....");

    sockaddr_in clientService{};
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, "127.0.0.1", &clientService.sin_addr);
    clientService.sin_port = htons(port);

    if(connect(clientSock, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR){
        error("Client Connection Failed! Error Code : " + std::to_string(WSAGetLastError()));
        closesocket(clientSock);
        WSACleanup();
        return 0;
    } else {
        success("Connection completed!");
        success("Client can send and receive data!");
    }

    std::cout << '\n';
    std::cout << GREEN;
    std::cout << "========================================\n";
    std::cout << "      CONNECTED TO TCP SERVER\n";
    std::cout << "========================================\n";
    std::cout << RESET << '\n';


    info("Start chatting.\n[You]:");

    // char buffer[200];
    // info("Please enter a message to send to the server...");
    // std::cin.getline(buffer, 200);
    // int byteCount = send(clientSock, buffer, 200, 0);

    // if(byteCount > 0){
    //     std::cout << "Message sent: " << buffer << std::endl;
    // } else {
    //     WSACleanup();
    // }

    std::thread recvThread(recieveMessages, clientSock);
    recvThread.detach();

    std::string userInput;
    while (std::getline(std::cin, userInput)) {
        if (userInput.empty()) {
            std::cout << "[You]: ";
            continue;
        }

        int sendResult = send(clientSock, userInput.c_str(), static_cast<int>(userInput.size()), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "[System]: send failed with error: " << WSAGetLastError() << "\n";
            break;
        }
        std::cout << "[You]: ";
    }

    info("Closing the sockets...");
    system("pause");

    closesocket(clientSock);
    WSACleanup();

    return 0;
}
