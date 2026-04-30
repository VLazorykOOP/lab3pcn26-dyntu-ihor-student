#include <iostream>
#include <fstream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 5000;
const char* SERVER_IP = "127.0.0.1";

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        cout << "Invalid address / Address not supported" << endl;
        return 1;
    }

    ifstream inFile("network_lab3_client.cpp", ios::binary);

    vector<char> fileData(
        (istreambuf_iterator<char>(inFile)),
        istreambuf_iterator<char>()
    );

    inFile.close();

    int mode;
    cout << "Choose mode:" << endl;
    cout << "1 - send file as one fragment" << endl;
    cout << "2 - send file as several fragments" << endl;
    cin >> mode;

    if (mode == 1) {
        sendto(
            clientSocket,
            fileData.data(),
            (int)fileData.size(),
            0,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)
        );

        cout << "File sent as one fragment." << endl;
    }
    else if (mode == 2) {
        int fragmentsCount = 67;

        int fileSize = (int)fileData.size();
        int fragmentSize = fileSize / fragmentsCount;
        int offset = 0;

        for (int i = 0; i < fragmentsCount; i++) {
            int currentSize = (i == fragmentsCount - 1)
                ? fileSize - offset
                : fragmentSize;

            sendto(
                clientSocket,
                fileData.data() + offset,
                currentSize,
                0,
                (sockaddr*)&serverAddr,
                sizeof(serverAddr)
            );

            cout << "Sent fragment " << i + 1 << ": " << currentSize << " bytes" << endl;

            offset += currentSize;
        }

        cout << "File sent in fragments." << endl;
    }

    sendto(
        clientSocket,
        "END",
        3,
        0,
        (sockaddr*)&serverAddr,
        sizeof(serverAddr)
    );

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}


