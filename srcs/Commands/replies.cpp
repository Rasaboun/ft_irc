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
            client->print(":Unautorized command (already registered)");
            break;
        case 464:
            client->print(":Password incorrect");
            break;
        case 501:
            client->print(":Uknown MODE flag");
            break;
        case 502:
            client->print(":Cannot change mode for other users");
            break;
        default:
            break;
    }
    return (0);
}

int    command_responses(int code, Client* client, Ircserv& serv, Command& command)
{
    (void)client;
    (void)serv;
    (void)command;
    
    switch (code)
    {
        case 221:
        {
            Client* target = serv.getClient(command.getParam(0));
            if (target)
                target->print(target->getNickname() + "'s user mode is : +" + target->getModes());
            break;
        }
        default:
            break;
    }
    return (0);
}


int     reply(int code, Client* client, Ircserv& serv, Command& command)
{
    if (code > 400)
        return (error_replies(code, client, serv, command));
    else
        return (command_responses(code, client, serv, command));
    return (0);
}
