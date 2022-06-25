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
	std::string mode = command.getParam(1);
	std::string modifiedModes;
	if (is_add_or_remove_mode(mode) == '-')
	{
		for (size_t i = 1; i < mode.length(); i++)
		{
			std::string mode 
			std::cout << "le modes est |" << mode[i] << "|" << std::endl;
			if ((client->setMode(mode[i], false, serv)) && modifiedModes.find(mode[i]) != std::string::npos)
				modifiedModes += mode[i]; 
		}
		if (!is_valid_mode(mode))
			reply(ERR_UMODEUNKNOWNFLAG, client, serv, command);
		if (modifiedModes.length() > 0)
			client->print("MODE " + client->getNickname() + " -" + modifiedModes);
	}
	if (is_add_or_remove_mode(mode) == '+')
	{
		std::string	actual_modes(client->getModes());
		for (size_t i = 1; i < mode.length(); i++)
		{
			std::cout << "le modes est |" << mode[i] << "|" << std::endl;
			if (mode[i] != OPERATOR)
				if ((client->setMode(mode[i], true, serv)) && modifiedModes.find(mode[i]) != std::string::npos)
					modifiedModes += mode[i];
		}
		if (!is_valid_mode(mode))
			reply(ERR_UMODEUNKNOWNFLAG, client, serv, command);
		if (modifiedModes.length() > 0)
			client->print("MODE " + client->getNickname() + " +" + modifiedModes);
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
	cli->whoisReplies(serv.getPrefix(), client);
	return (reply(RPL_ENDOFWHOIS, client, serv, command, command.getParam(0)));

}