#include "Ircserv.hpp"

int	channel_mode(Client *client, Ircserv& serv, Command& command)
{
    Channel *chan = serv.getChannel(command.getParam(0));
    
    if (!chan)
		return (reply(ERR_NOSUCHCHANNEL, client, serv, command));
    if (command.getNbParams() == 1)
		return (reply(RPL_CHANNELMODEIS, client, serv, command));
	if (command.getParam(1)[0] != '-' && command.getParam(1).find("b") != std::string::npos && command.getNbParams() == 2)
	{
		chan->printBanList(client);
		if ((command.getParam(1)[0] == '+' && command.getParam(1).length() > 2) \
			|| (command.getParam(1)[0] != '+' && command.getParam(1).length() > 1))
       		return (reply(ERR_CHANOPRIVISNEEDED, client, serv, command));
		return (0);
	}
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
	if (is_add_or_remove_mode(command.getParam(1)) == '-')
	{
		for (size_t i = 1; i < command.getParam(1).length(); i++)
		{
			client->setMode(command.getParam(1)[i], false, serv);
		}
	}
	if (is_add_or_remove_mode(command.getParam(1)) == '+')
	{
		for (size_t i = 1; i < command.getParam(1).length(); i++)
		{
			client->setMode(command.getParam(1)[i], true, serv);
		}
	}

	return (0);
}

int	who(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() < 1)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));

	std::string		target = command.getParam(0);
	if (target[0] == '#')
	{
		Channel *channel = serv.getChannel(target);
		
		if (!channel)
			return (reply(ERR_NOSUCHCHANNEL, client, serv, command, target));
		if (channel->isClient(client))
			channel->printWho(client);
	}
	else
	{
		Client *cli = serv.getClient(target);

		if (cli)
		{
			if (!cli->getMode(INVISIBLE) || (cli->getMode(INVISIBLE) && sharing_channel(client, cli)))
			{
				std::string base = reply_prefix(serv.getName(), RPL_WHOREPLY, client->getNickname()) + "* ";
				/*if (serv.isOperator(cli))
					base += "@";
				else
					base += "~";
				*/
				client->print(base + "~" + cli->getUsername() + " " + cli->getHostname() \
						+ " " + serv.getName() + " " + cli->getNickname() + " H :0 " + cli->getRealname().substr(1));
			}
		}
	}
    client->print(reply_prefix(serv.getName(), RPL_ENDOFWHO, client->getNickname()) + target + " :End of WHO list");

	return (0);
}

int	whois(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() < 1)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	
	Client *cli	= serv.getClient(command.getParam(0));
	if (!cli)
        return (reply(ERR_NOSUCHNICK, client, serv, command, command.getParam(0)));
	if (!cli->getMode(INVISIBLE) || (cli->getMode(INVISIBLE) && sharing_channel(client, cli)))
		cli->whoisReplies(serv.getPrefix(), client);
	return (reply(RPL_ENDOFWHOIS, client, serv, command, command.getParam(0)));

}