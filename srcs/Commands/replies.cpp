#include "Ircserv.hpp"

int     error_replies(int code, Client* client, Ircserv& serv, Command& command)
{
    (void)client;
    (void)serv;

    switch (code)
    {
        case 431:
            client->print(":No nickname given");
            break;
        case 432:
            client->print(command.getParam(0) + " :Erroneous nickname");
            break;
        case 433:
            client->print(command.getParam(0) + " :Nickname already in use");
            break;
        case 461:
            client->print(command.getName() + " :Not enough parameters");
            break;
        case 462:
            client->print("Unautorized command (already registered)");
            break;
        default:
            break;
    }
    return (0);
}

int     reply(int code, Client* client, Ircserv& serv, Command& command)
{
    if (code > 400)
        return (error_replies(code, client, serv, command));
    return (0);
}

