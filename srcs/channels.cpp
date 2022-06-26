#include "Ircserv.hpp"

int	join(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    
    std::vector<std::string> channels = split(command.getParam(0), ',');
    std::vector<std::string> keys = split(command.getParam(1), ',');

    std::vector<std::string>::iterator  key_it = keys.begin();
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        int new_chan = 0;
        if (!is_valid_channel(*it))
            continue ;
        if (client->getNbChannels() == CHANLIMIT)
            return (reply(ERR_TOOMANYCHANNELS, client, serv, command, *it));
        if (!serv.isChannel(*it))
        {
            serv.addChannel(*it);
            new_chan = 1;
        }
        Channel *channel = serv.getChannel(*it);
        if (new_chan || client->isOperator())
        {
            channel->addOperator(client);
        }
        if (channel->getMode(LIMIT) && channel->getNbClients() >= channel->getLimit())
        {
            reply(ERR_CHANNELISFULL, client, serv, command, *it);
            continue ;
        }
        if (channel->isBanned(client))
        {
            reply(ERR_BANNEDFROMCHAN, client, serv, command, *it);
            continue ;
        }
        if (key_it == keys.end())
        {
            channel->addClient(client);
        }
        else
        {
            channel->addClient(client, *key_it);
            key_it++;
        }
    }
    return (0);
}

int	part(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    
    std::vector<std::string> channels = split(command.getParam(0), ',');
    

    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        Channel* channel = serv.getChannel(*it);
        
        if (!channel)
        {
		    reply(ERR_NOSUCHCHANNEL, client, serv, command, *it);
            continue ;
        }
        if (!channel->isClient(client))
        {
            reply(ERR_NOTONCHANNEL, client, serv, command, *it);
            continue ;
        }
        if (command.getNbParams() > 1)
            channel->removeClient(client, command.joinParams(1));
        else
            channel->removeClient(client, PART_MESSAGE);
    }
    return (0);
}

int	topic(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    
    Channel* channel = serv.getChannel(command.getParam(0));
    if (!channel)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command, command.getParam(0)));
    if (!channel->isClient(client))
        return (reply(ERR_NOTONCHANNEL, client, serv, command, command.getParam(0)));
        
    if (command.getNbParams() == 1)
        channel->sendTopic(client);
    else
    {
        if (channel->getMode(TOPIC) && !channel->isOperator(client))
        {
            return (reply(ERR_CHANOPRIVISNEEDED, client, serv, command));
        }
        if (command.getParam(1).length() == 1)
            channel->setTopic("");
        else
        {
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
        std::vector<std::string> channels = split(command.getParam(0), ',');
    

        for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            Channel* channel = serv.getChannel(*it);
            if (!channel || (channel->getMode(SECRET) && !channel->isClient(client)))
                continue ;
            channel->printInfos(client);
        }
    }
    else
    {
        std::map<std::string, Channel *> channels = serv.getChannels();
        for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it).second->getMode(SECRET) && !(*it).second->isClient(client))
                continue ;
            (*it).second->printInfos(client);
        }
    }
    reply(RPL_LISTEND, client, serv, command);
    return (0);
}

int	invite(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() < 2)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    Channel* channel = serv.getChannel(command.getParam(1));
    if (!channel)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command, command.getParam(1)));
    if (serv.availableNickname(command.getParam(0)))
		return (reply(ERR_NOSUCHNICK, client, serv, command, command.getParam(0)));

    if (!channel->isClient(client))
        return (reply(ERR_NOTONCHANNEL, client, serv, command, command.getParam(1)));
    if (channel->getMode(INVITE) && !channel->isOperator(client))
        return (reply(ERR_CHANOPRIVISNEEDED, client, serv, command));
    if (channel->isClient(serv.getClient(command.getParam(0))))
        return (reply(ERR_USERONCHANNEL, client, serv, command));
    reply(RPL_INVITING, client, serv, command);
    channel->invite(client, command.getParam(0));
    return (0);
}

int	kick(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() < 2)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    
    Channel* channel = serv.getChannel(command.getParam(0));
    if (!channel)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command, command.getParam(0)));
    if (!channel->isClient(client))
        return (reply(ERR_NOTONCHANNEL, client, serv, command, command.getParam(0)));
    if (!channel->isOperator(client))
        return (reply(ERR_CHANOPRIVISNEEDED, client, serv, command));
    
    std::vector<std::string> targets = split(command.getParam(1), ',');
    
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
    {
        Client *target = serv.getClient(*it);
        
        if (!target)
            continue ;
        if (!channel->isClient(target))
            return (reply(ERR_USERNOTINCHANNEL, client, serv, command, client->getNickname()));
        if (command.getNbParams() > 2)
        {
            if (command.getParam(2) != ":")
            {
                channel->kickClient(client, target, command.joinParams(2));
                continue ;
            }
        }
        channel->kickClient(client, target, KICK_MESSAGE);
    }
    return (0);
}