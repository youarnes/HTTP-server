// socket programming is a way for two computers or processes to talk over the network
#include <winsock2.h>
#include <iostream>
#include "./utils/http_request_parser.h"
#include "./utils/http_response.h"
#include "./utils/file_utils.h"

#define DEFAULT_BUFLEN 1024

int main () {
    // Initialize WSA variables
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    int iResult; // var to store integer intermediate results 

    // Check for initialization success
    if (wserr != 0) {
        std::cout << "The winsock dll not found" << std::endl;
        return 1;
    } else {
        std::cout << "The Winsock dll found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( serverSocket == INVALID_SOCKET ) {
        printf("The server socket definition failed with error %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(80);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    iResult = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if ( iResult == SOCKET_ERROR ) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    iResult = listen(serverSocket, SOMAXCONN);
    if ( iResult == SOCKET_ERROR ) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    char buffer[DEFAULT_BUFLEN];
    SOCKET clientSocket;

    do {
        clientSocket = accept(serverSocket, nullptr, nullptr);
        if ( clientSocket == INVALID_SOCKET ) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            continue;
        }

        do {
            iResult = recv(clientSocket, buffer, DEFAULT_BUFLEN, 0);
            if ( iResult > 0 )
                {
                    printf("Bytes received: %d\n", iResult);
                    try {
                        HttpRequest req = parse_http_request(buffer);

                        std::cout << "Method: " << req.method << "\n";
                        std::cout << "Target: " << req.target << "\n";
                        std::cout << "Version: " << req.version << "\n";
                        std::cout << "Host: " << req.headers["Host"] << "\n";

                        std::string body;
                        std::string response;
                        if (req.method == "GET" && req.target == "/") {
                            body = read_file("./index.html");
                            std::cout << "Body:\n"<< body << "\n";
                            response = http_response(200, "OK", "text/html", body);
                        } else {
                            body = "<h1>404 Not Found</h1>";
                            response = http_response(404, "Not Found", "text/html", body);
                        }
                        send(clientSocket, response.c_str(), response.size(), 0);
                        if (iResult == SOCKET_ERROR) {
                            wprintf(L"send failed with error: %d\n", WSAGetLastError());
                            closesocket(clientSocket);
                            WSACleanup();
                            return 1;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error: " << e.what() << "\n";
                    }
                }

            else if ( iResult == 0 )
                {
                    printf("Connection closed\n\n");
                    closesocket(clientSocket);
                }

            else {
                printf("recv failed: %d\n", WSAGetLastError());
            }
            
        } while ( iResult > 0 );

    } while( 1 );
    WSACleanup();
    return 0;
}