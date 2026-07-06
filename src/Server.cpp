#include <string>
#include <thread>
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

    SOCKET serverSock, acceptSock;

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

    //creating the server socket

    info("Creating Socket....");

    serverSock = INVALID_SOCKET;
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(serverSock == INVALID_SOCKET) {
        error("Socket creation failed. Error Code : " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return 0;
    } else {
        success("Socket created successfully!");
    }

    //binding the socket to the IP and PORT

    info("Binding Socket....");

    sockaddr_in service{};
    service.sin_family = AF_INET;
    InetPton(AF_INET, "127.0.0.1", &service.sin_addr);
    service.sin_port = htons(port);

    if(bind(serverSock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR){
        error("Binding Failed! Error Code : " + std::to_string(WSAGetLastError()));
        closesocket(serverSock);
        WSACleanup();
        return 0;
    } else {
        success("Binding completed!");
    }

    //Listening to incoming requests

    info("Starting listener...");

    if(listen(serverSock, 1) == SOCKET_ERROR) {
        error("Error in listening socket. Error Code : " + std::to_string(WSAGetLastError()));
        closesocket(serverSock);
        WSACleanup();
        return 0;
    } else {
        success("Listening : Waiting for connections...");
    }

    info("Waiting for incoming client...");

    //accepting client connection once recieved

    acceptSock = accept(serverSock, NULL, NULL);

    if(acceptSock == INVALID_SOCKET) {
        error("Accept Failed. Error Code : " + std::to_string(WSAGetLastError()));
        closesocket(serverSock);
        WSACleanup();
        return -1;
    } else {
        success("Accepted Connection!");
    }

    std::cout << '\n';
    std::cout << GREEN;
    std::cout << "========================================\n";
    std::cout << "      CLIENT CONNECTED SUCCESSFULLY\n";
    std::cout << "========================================\n";
    std::cout << RESET << '\n';

    info("You can start chatting.\n[You]:");

    // char buffer[200];

    // int byteCount = recv(acceptSock, buffer, 200, 0);

    // if(byteCount > 0){
    //     std::cout << "Message recieved: " << buffer << std::endl;
    // } else {
    //     WSACleanup();
    // }

    std::thread recvThread(recieveMessages, acceptSock);
    recvThread.detach();

    std::string userInput;
    while(std::getline(std::cin, userInput)){
        if (userInput.empty()) {
            std::cout << "[You]: ";
            continue;
        }
        int sendResult = send(acceptSock, userInput.c_str(), static_cast<int>(userInput.size()), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "[System]: send failed with error: " << WSAGetLastError() << "\n";
            break;
        }
        std::cout << "[You]: ";
    }

    system("pause");

    closesocket(acceptSock);
    WSACleanup();

    return 0;
}
