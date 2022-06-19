#include "Ircserv.hpp"

int	join(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    if (!serv.isChannel(command.getParam(0)))
    {
        serv.addChannel(command.getParam(0));
    }
    serv.getChannel(command.getParam(0))->addClient(client);
    return (0);
}

int	part(Client *client, Ircserv& serv, Command& command)
{
    std::cout << "In part\n";
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    Channel* channel = serv.getChannel(command.getParam(0));
    if (!channel)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command));
    if (!channel->isClient(client))
        return (reply(ERR_NOTONCHANNEL, client, serv, command));

    if (command.getNbParams() > 1)
        channel->removeClient(client, command.joinParams(1));
    else
        channel->removeClient(client, PART_MESSAGE);

    return (0);
}