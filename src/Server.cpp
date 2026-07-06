#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "../include/Logger.hpp"

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

    info("Chat to the client");

    char buffer[200];

    int byteCount = recv(acceptSock, buffer, 200, 0);

    if(byteCount > 0){
        std::cout << "Message recieved: " << buffer << std::endl;
    } else {
        WSACleanup();
    }

    system("pause");

    closesocket(acceptSock);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}
