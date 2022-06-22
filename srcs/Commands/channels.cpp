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
        if (key_it == keys.end())
        {
            serv.getChannel(*it)->addClient(client);
        }
        else
        {
            serv.getChannel(*it)->addClient(client, *key_it);
            key_it++;
        }
        if (new_chan)
            serv.getChannel(*it)->addOperator(client);
    }
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

int	invite(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() < 2)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
    Channel* channel = serv.getChannel(command.getParam(1));
    if (!channel)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command));
    if (serv.availableNickname(command.getParam(0)))
		return (reply(ERR_NOSUCHNICK, client, serv, command));

    if (!channel->isClient(client))
        return (reply(ERR_NOTONCHANNEL, client, serv, command));
    if (channel->getMode(INVITE) && !channel->isOperator(client))
        return (reply(ERR_CHANOPRIVISNEEDED, client, serv, command));
    if (channel->isClient(serv.getClient(command.getParam(0))))
        return (reply(ERR_USERONCHANNEL, client, serv, command));
    reply(RPL_INVITING, client, serv, command);
    channel->invite(client, command.getParam(0));
    return (0);
}
