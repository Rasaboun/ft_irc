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

int	topic(Client *client, Ircserv& serv, Command& command)
{
	std::cout << "In topic\n";
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    
    Channel* channel = serv.getChannel(command.getParam(0));
    if (!channel)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command));
    if (!channel->isClient(client))
        return (reply(ERR_NOTONCHANNEL, client, serv, command));
    
    std::cout << "In topic\n";
    
    if (command.getNbParams() == 1)
        channel->sendTopic(client);
    else
    {
        if (command.getParam(1).length() == 1)
            channel->setTopic("");
        else
        {
            command.setParam(1, command.getParam(1).substr(1));
            channel->editTopic(client, command.joinParams(1));
        }
    }
	std::cout << "In topic\n";
    return (0);
}