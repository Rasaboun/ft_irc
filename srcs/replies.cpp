#include "Ircserv.hpp"

std::string     reply_prefix(const std::string& source, int code, const std::string& target)
{
    return (":" + source + " " + convert_code(code) + " " + target + " ");
}

std::string     error_replies(int code, Client* client, Ircserv& serv, Command& command, std::string param)
{
    (void)client;
    (void)serv;
    switch (code)
    {
        case ERR_NOSUCHNICK:
            return(param + " :No such nick/channel");  
        case ERR_NOSUCHCHANNEL:
            return(param + " :No such channel");
        case ERR_CANNOTSENDTOCHAN:
            return(param + " :Cannot send to channel");
        case ERR_TOOMANYCHANNELS:
            return (param + " :You have joined too many channels");  
        case ERR_TOOMANYTARGETS:
            return(command.getParam(0) + " :Too many targets");     
        case ERR_NORECIPIENT:
            return(":No recipient given " + command.getName());    
        case ERR_NOTEXTTOSEND:
            return(":No text to send");  
        case ERR_UNKNOWNCOMMAND:
            return(command.getName() + " :Unknown command");          
        case ERR_NONICKNAMEGIVEN:
            return(":No nickname given");             
        case ERR_ERRONEUSNICKNAME:
            return(command.getParam(0) + " :Erroneous nickname");             
        case ERR_NICKNAMEINUSE:
            return(command.getParam(0) + " :Nickname is already in use");      
         case ERR_USERNOTINCHANNEL:
            return(param + " " + command.getParam(0) + ":They are not on that channel");
        case ERR_NOTONCHANNEL:
            return(param + " :You're not on that channel");
        case ERR_USERONCHANNEL:
            return(command.getParam(0) + " " +  command.getParam(1) + " :is already on channel");                 
        case ERR_NEEDMOREPARAMS:
            return(command.getName() + " :Not enough parameters");             
        case ERR_ALREADYREGISTERED:
            return(":Unautorized command (already registered)");             
        case ERR_PASSWDMISMATCH:
            return(" :Password incorrect");             
        case ERR_CHANNELISFULL:
            return(param + " :Cannot join channel (+l)");
        case ERR_BANNEDFROMCHAN:
            return(param + " :Cannot join channel (+b)");                    
        case ERR_CHANOPRIVISNEEDED:
            return(command.getParam(0) + " :You're not channel operator");
        case ERR_NOPRIVILEGES:
            return(" :Permission denied- You're not an IRC operator");                    
        case ERR_UMODEUNKNOWNFLAG:
            return(":Unknown MODE flag");             
        case ERR_USERSDONTMATCH:
            return(":Cannot change mode for other users");             
        default:
            break;
    }
    return ("");
}

std::string   command_responses(int code, Client* client, Ircserv& serv, Command& command, const std::string& param)
{
    (void)param;
    switch (code)
    {
        case RPL_WELCOME:         
            return("Welcome to the Internet Relay Network " + client->getFullname());         
        case RPL_YOURHOST:         
            return("Your host is " + serv.getName() + ", running version 1.0");         
        case RPL_CREATED:         
            return("This server was created today");   //Need to add correct date      
        case RPL_MYINFO:         
            return(serv.getName() + " 1.0 (need to add available modes)");         
        case RPL_UMODEIS:         
            return("+" + client->getModes());
        case RPL_ENDOFWHOIS:
            return(param + " :End of /WHOIS");  
        case RPL_LISTSTART:
            return("Channel :Users Name");  
        case RPL_LISTEND:
            return("End of /LIST");      
        case RPL_CHANNELMODEIS:         
            return(command.getParam(0) + " +" + serv.getChannel(command.getParam(0))->getModes());
        case RPL_NOTOPIC:
            return (command.getParam(0) + " :No topic is set");
        case RPL_TOPIC:
            return (command.getParam(0) + " :" + serv.getChannel(command.getParam(0))->getTopic());
        case RPL_INVITING:
            return (command.getParam(0) + " " + command.getParam(1));             
        case RPL_YOUREOPER:
            return (":You are now an IRC operator");
        default:
            break;
    }
    return ("");
}


int     reply(int code, Client* client, Ircserv& serv, Command& command, const std::string& param)
{
    std::string     res = reply_prefix(serv.getName(), code, client->getNickname());

    if (code > 400)
        res += error_replies(code, client, serv, command, param);
    else 
        res += command_responses(code, client, serv, command, param);
    client->print(res);
    return (0);
}
