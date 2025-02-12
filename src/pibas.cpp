#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pibas.hpp"
#include "encryption.hpp"
#include <unistd.h>

#define KEYLEN 100
using namespace std;

// Setup function: creates an encrypted index
std::unordered_map<std::string, std::string> Setup(
    const std::unordered_map<std::string, std::vector<std::string>> &D)
{

    std::unordered_map<std::string, std::string> ED;
    // Implementation: Encrypt document IDs and store in ED
    vector<unsigned char> key;
    key = Encryption::generateKey(KEYLEN);
    list<int> *my_list = new list<int>;

    for ()

        return ED;
}

// Search function: retrieves document IDs for a given keyword
std::vector<std::string> Search(
    const std::unordered_map<std::string, std::string> &ED,
    const std::string &K,
    const std::string &w)
{
    std::vector<std::string> result;
    
    // Initializing client setup
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

    // sending data
    const char* message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);

    // closing socket
    close(clientSocket);

    // Initializing server setup
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Defining server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket and have 1 as listening log for one range query at a time
    bind(serverSocket, (struct sockaddr*)&serverAddress,
        sizeof(serverAddress));

    
    listen(serverSocket, 1);

    // Accept connection request
    int clientSocket
        = accept(serverSocket, nullptr, nullptr);

    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message from client: " << buffer
                << endl;

    // closing the socket.
    close(serverSocket);
    
    return result;
}