#include "Ircserv.hpp"

int	msg(Client *client, Ircserv& serv, Command& command)
{
    if (command.getNbParams() == 0)
		  return (reply(ERR_NORECIPIENT, client, serv, command));
    if (command.getNbParams() == 1)
		  return (reply(ERR_NOTEXTTOSEND, client, serv, command));

    std::vector<std::string> targets = split(command.getParam(0), ',');

    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
    {
        std::string message = command.joinParams(1);

        if (serv.availableNickname(*it) && !serv.isChannel(*it))
            return (reply(ERR_NOSUCHNICK, client, serv, command, *it));
        if (serv.isChannel(*it))
        {
            //Private message to a channel
            Channel *channel = serv.getChannel(*it);
            if (channel->getMode(NO_EXTERN) && !channel->isClient(client))      //Mode n is on
                return (reply(ERR_CANNOTSENDTOCHAN, client, serv, command, *it));
            if (channel->getMode(MODERATE) && !channel->isOperator(client))     //Mode m is on
                return (reply(ERR_CANNOTSENDTOCHAN, client, serv, command, *it));
            if (channel->isBanned(client))
                return (reply(ERR_CANNOTSENDTOCHAN, client, serv, command, *it));

            Message mess(client->getFullname(), channel, message);
            client->addMessage(mess);
        }
        else
        {
            //Private message to a client
            Message mess(client->getFullname(), serv.getClient(*it), message);
            client->addMessage(mess);
        }
    }
    return (0);
}

int	wallops(Client *client, Ircserv& serv, Command& command)
{
    if (command.getNbParams() == 0)
        return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
  /*  if (!serv.isOperator(client))
        return (reply(ERR_NOPRIVILEGES, client, serv, command));
    
    std::string             message = command.joinParams(0);
    if (message.length() == 0)
        return (1);
    message = ":" + client->getNickname() + " WALLOPS " + message;

    std::vector<Client *>   wallops = serv.getClients();
    for (std::vector<Client *>::iterator it = wallops.begin(); it != wallops.end(); it++)
    {
        if (serv.isWallop(*it))
            (*it)->print(message);
    }
    */
    return (0);
    
}