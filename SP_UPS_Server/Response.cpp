#include "Response.h"

void Response::sendToPlayer(Player *player, string message) {
    message = player->name + "|" + message;
    player->msg_out = message + "\n";
    sendToClient(player->socket, message);
}

void Response::sendErrorToPlayer(Player *player, string errorMessage){
    player->invalid_message_counter++;
    errorMessage = player->name + "|" + "ERR|" + errorMessage +"|INVAL_MSG_C|"+ to_string(player->invalid_message_counter);
    player->msg_out = errorMessage + "\n";
    sendToClient(player->socket, errorMessage);

}

void Response::sendErrorToPlayer1(Player *player, string errorMessage){
    player->invalid_message_counter++;
    errorMessage = "LOGIN|ERR|" + errorMessage ;
    player->msg_out = errorMessage + "\n";
    sendToClient(player->socket, errorMessage);

}

void Response::sendToClient(int sockFd, string message) {

    message = message += "\n";

    cout << "Sending a message: " + message << endl;
    send(sockFd, message.data(), message.length(), 0);

}
