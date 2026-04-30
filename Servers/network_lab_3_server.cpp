#include <iostream>
#include <fstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 5000;
const int BUFFER_SIZE = 65507;

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    cout << "UDP server started." << endl;

    ofstream outFile("received.cpp", ios::binary);

    char buffer[BUFFER_SIZE];

    while (true) {
        int bytesReceived = recvfrom(
            serverSocket,
            buffer,
            BUFFER_SIZE,
            0,
            (sockaddr*)&clientAddr,
            &clientAddrSize
        );

        if (bytesReceived <= 0) {
            break;
        }

        string message(buffer, bytesReceived);

        if (message == "END") {
            cout << "File received successfully." << endl;
            break;
        }

        outFile.write(buffer, bytesReceived);

        cout << "Received fragment: " << bytesReceived << " bytes" << endl;
    }

    outFile.close();
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}


