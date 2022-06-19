#include "Ircserv.hpp"

std::string    error_replies(int code, Client* client, Ircserv& serv, Command& command)
{
    (void)client;
    (void)serv;

    switch (code)
    {
        case 401:
            return(command.getParam(0) + ":No such nick/channel");  
        case 403:
            return(command.getParam(0) + ":No such channel");  
        case 407:
            return(command.getParam(0) + " :Too many targets");     
        case 411:
            return(":No recipient given " + command.getName());    
        case 412:
            return(":No text to send");     
        case 431:
            return(":No nickname given");             
        case 432:
            return(command.getParam(0) + " :Erroneous nickname");             
        case 433:
            return(command.getParam(0) + " :Nickname is already in use");      
        case 442:
            return(command.getParam(0) + " :You're not on that channel");            
        case 461:
            return(command.getName() + " :Not enough parameters");             
        case 462:
            return(":Unautorized command (already registered)");             
        case 464:
            return(":Password incorrect");             
        case 501:
            return(":Uknown MODE flag");             
        case 502:
            return(":Cannot change mode for other users");             
        default:
            break;
    }
    return (0);
}

std::string   command_responses(int code, Client* client, Ircserv& serv, Command& command)
{
    (void)command;
    switch (code)
    {
        case 001:         
            return("Welcome to the Internet Relay Network " + client->getFullname());         
        case 002:         
            return("Your host is " + serv.getName() + ", running version 1.0");         
        case 003:         
            return("This server was created today");   //Need to add correct date      
        case 004:         
            return(serv.getName() + " 1.0 (need to add available modes)");         
        case 221:         
            return("+" + client->getModes()); 
        case 331:
            return (command.getParam(0) + " :No topic is set");
        case 332:
            return (command.getParam(0) + " :" + serv.getChannel(command.getParam(0))->getTopic());             
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
