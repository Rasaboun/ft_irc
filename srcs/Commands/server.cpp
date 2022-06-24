#include "Ircserv.hpp"

int	channel_mode(Client *client, Ircserv& serv, Command& command)
{
    Channel *chan = serv.getChannel(command.getParam(0));
    
    if (!chan)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command));
    if (command.getNbParams() == 1)
		return (reply(RPL_CHANNELMODEIS, client, serv, command));
    if (!chan->isOperator(client))
        return (reply(ERR_CHANOPRIVISNEEDED, client, serv, command));
    chan->changeModes(client, command.getParams());
    return (0);
}

int	mode(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() < 1)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	if (command.getParam(0)[0] == '#')
		return (channel_mode(client, serv, command));
	if (command.getNbParams() == 1 && command.getParam(0) == client->getNickname())
		return (reply(RPL_UMODEIS, client, serv, command));
	if (client->getNickname() != command.getParam(0))
		return (reply(ERR_USERSDONTMATCH, client, serv, command));
	if (!is_valid_mode(command.getParam(1)))
		return (reply(ERR_UMODEUNKNOWNFLAG, client, serv, command));
	if (command.getParam(1)[0] == '-')
		client->setMode(command.getParam(1)[1], false);
	else
		client->setMode(command.getParam(1)[1], true);
	return (0);
}