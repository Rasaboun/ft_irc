#include "Ircserv.hpp"

int	pass(Client *client, Ircserv& serv, Command& command)
{
	if (client->getState() != NEED_PASS)
		return (reply(462, client, serv, command));
	if (command.getNbParams() == 0)
		return (reply(461, client, serv, command));
	if (command.getParam(0) == serv.getPassword())
		client->setState(NEED_NICK);
	return (0);
}

int	nick(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(431, client, serv, command));
	if (!is_valid_nickname(command.getParam(0)))
		return (reply(432, client, serv, command));
	if (!serv.availableNickname(command.getParam(0)))
		return (reply(433, client, serv, command));
	client->setNickname(command.getParam(0));
	if (client->getState() == NEED_NICK)
		client->setState(NEED_USER);
	
	return (0);
}