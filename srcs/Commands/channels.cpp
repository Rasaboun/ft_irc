#include "Ircserv.hpp"

int	join(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    if (!serv.isChannel(command.getParam(0)))
    {
        serv.addChannel(command.getParam(0));
    }
    Channel*    chan = serv.getChannel(command.getParam(0));
    chan->addClient(client);
    if (chan->getTopic().length())
        return (reply(RPL_TOPIC, client, serv, command));
    return (reply(RPL_NOTOPIC, client, serv, command));
}

int	part(Client *client, Ircserv& serv, Command& command)
{
    (void)serv;
    (void)command;
    (void)client;

    return (0);
}