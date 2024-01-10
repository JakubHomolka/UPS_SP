#include <iostream>
#include <fstream>
#include <string>
#include "Server.h"

void printHelp();

int main() {

    std::string filename = "configfile.txt";
    int port = 0;

    std::ifstream configFile(filename);

    if (!configFile.is_open()) {
        std::cerr << "Error: Could not open config file." << std::endl;
        return 1;
    }

    std::string ipAddress, portStr;
    if (!(configFile >> ipAddress >> portStr)) {
        std::cout << "Error: Failed to read IP address and port from config file." << std::endl;
        configFile.close();
        return 1;
    }

    configFile.close();

    try {
        port = std::stoi(portStr);
    }
    catch(std::invalid_argument& e){
        std::cout << "ERROR: Port must be integer." << std::endl;
        printHelp();
        return 1;
    }
    catch(std::out_of_range& e){
        printHelp();
        return 1;
    }

    if (port < 1 || port > 65535) {
        std::cout << "ERROR: Port is out of range." << std::endl;
        printHelp();
        return 1;
    }

    Server *server = new Server();
    int result = server->init(ipAddress, port);

    if (result == 0) {
        server->listenConnections();
    }

    cout << "end" << endl;
    //delete server;

    return 0;
}

void printHelp() {
    std::cout << "IP_ADDR \t- Server IP adress" << std::endl;
    std::cout << "PORT \t\t- Port to listening - number from <1 , 65535>" << std::endl;
}