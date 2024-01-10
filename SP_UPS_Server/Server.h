//
// Created by David on 26.11.2021.
//

#ifndef RPS_SERVER_SERVER_H
#define RPS_SERVER_SERVER_H

#include <string>
#include <cstdio>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>
#include <sys/socket.h>
#include <iostream>
#include <sys/ioctl.h>

#include "ClientManager.h"
#include "GameManager.h"
#include "GameLogic.h"
#include "Request.h"

using namespace std;
/**
 * Server class
 */
class Server {

public:

    /**
     * used for server initialization and validation cmd line arguments
     * @param ip ip for the server
     * @param port port for listening
     * @return -1 error, 0 ok
     */
    int init(string ip, int port);

    /**
     * listening loop of socket select
     */
    int listenConnections();

    /**
     * close socket comunication
     * @param socket socekt fd id
     */
    static void closeSocket(int socket);
/**
     * disconnect socket
     * @param socket socekt fd id
     */
    void disconnect(int socket);

    static fd_set client_socks, tests;

private:
    int server_socket, client_socket;
    static struct sockaddr_in server, client, peer_addr;
};


#endif //RPS_SERVER_SERVER_H
