#include "Ircserv.hpp"

std::string    error_replies(int code, Client* client, Ircserv& serv, Command& command)
{
    (void)client;
    (void)serv;

    switch (code)
    {
        case 431:
            return(":No nickname given");
            break;
        case 432:
            return(command.getParam(0) + " :Erroneous nickname");
            break;
        case 433:
            return(command.getParam(0) + " :Nickname already in use");
            break;
        case 461:
            return(command.getName() + " :Not enough parameters");
            break;
        case 462:
            return(":Unautorized command (already registered)");
            break;
        case 464:
            return(":Password incorrect");
            break;
        case 501:
            return(":Uknown MODE flag");
            break;
        case 502:
            return(":Cannot change mode for other users");
            break;
        default:
            break;
    }
    return (0);
}

std::string   command_responses(int code, Client* client, Ircserv& serv, Command& command)
{
    switch (code)
    {
        case 001:
        {
            return("Welcome to the Internet Relay Network " + client->getFullname());
        }
        case 002:
        {
            return("Your host is " + serv.getName() + ", running version 1.0");
        }
        case 003:
        {
            return("This server was created today");
        }
        case 004:
        {
            return(serv.getName() + " 1.0 (need to add available modes)");
        }
        case 221:
        {
            Client* target = serv.getClient(command.getParam(0));
            if (target)
                return(target->getNickname() + "'s user mode is : +" + target->getModes());
            break;
        }
        default:
            break;
    }
    return (0);
}


int     reply(int code, Client* client, Ircserv& serv, Command& command)
{
    std::string     res = ":ircserv " + convert_code(code) + " " + client->getNickname() + " ";
    if (code > 400)
        res += error_replies(code, client, serv, command);
    else
        res += command_responses(code, client, serv, command);
    client->print(res);
    return (0);
}
