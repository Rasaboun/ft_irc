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
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    
    Channel* channel = serv.getChannel(command.getParam(0));
    if (!channel)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command));
    if (!channel->isClient(client))
        return (reply(ERR_NOTONCHANNEL, client, serv, command));
        
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
    return (0);
}

int	list(Client *client, Ircserv& serv, Command& command)
{
    reply(RPL_LISTSTART, client, serv, command);
    if (command.getNbParams() > 0)
    {
        for (int i = 0; i < command.getNbParams(); i++)
        {
            Channel *channel = serv.getChannel(command.getParam(i));
            if (!channel)
                continue ;
            channel->printInfos(client);
        }
    }
    else
    {
        std::map<std::string, Channel *> channels = serv.getChannels();
        for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            (*it).second->printInfos(client);
        }
    }
    reply(RPL_LISTEND, client, serv, command);
    return (0);
}
