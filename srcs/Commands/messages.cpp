#include "Ircserv.hpp"

int	msg(Client *client, Ircserv& serv, Command& command)
{
    if (command.getNbParams() == 0)
		  return (reply(ERR_NORECIPIENT, client, serv, command));
    if (command.getNbParams() == 1)
		  return (reply(ERR_NOTEXTTOSEND, client, serv, command));
    if (serv.availableNickname(command.getParam(0)) && !serv.isChannel(command.getParam(0)))
		  return (reply(ERR_NOSUCHNICK, client, serv, command));
    if (serv.isChannel(command.getParam(0)))
    {
        //Private message to a channel
        Message mess(client->getFullname(), serv.getChannel(command.getParam(0)), command.joinParams(1));
        client->addMessage(mess);
    }
    else
    {
        //Private message to a client
        Message mess(client->getFullname(), serv.getClient(command.getParam(0)), command.joinParams(1));
        client->addMessage(mess);
    }

    return (0);
}