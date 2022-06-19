#include "Ircserv.hpp"

int	pass(Client *client, Ircserv& serv, Command& command)
{
	if (client->getState() != NEED_PASS)
		return (reply(ERR_ALREADYREGISTERED, client, serv, command));
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	if (command.getParam(0) == serv.getPassword())
		client->setState(NEED_NICK);
	return (0);
}

int	nick(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(ERR_NONICKNAMEGIVEN, client, serv, command));
	if (!is_valid_nickname(command.getParam(0)))
		return (reply(ERR_ERRONEUSNICKNAME, client, serv, command));
	if (!serv.availableNickname(command.getParam(0)))
		return (reply(ERR_NICKNAMEINUSE, client, serv, command));

	client->setNickname(command.getParam(0));
	if (client->getState() == NEED_NICK)
	{
		client->setState(NEED_USER);
	}
	return (0);
}


int	user(Client *client, Ircserv& serv, Command& command)
{
	if (client->getState() != NEED_USER)
		return (reply(ERR_ALREADYREGISTERED, client, serv, command));
	if (command.getNbParams() < 4)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	
	//need to handle mode (params[1])

	if (command.getParam(3)[0] != ':')
		return (client->print("USER :Incorrect realname"));
	client->setUsername(command.getParam(0));
	std::string realname = command.getParam(3).substr(1);

	for (int i = 4; i < command.getNbParams(); i++)
	{
		realname += " " + command.getParam(i);
	}
	client->setRealname(realname);
	client->setState(REGISTERED);
	reply(RPL_WELCOME, client, serv, command);
	reply(RPL_YOURHOST, client, serv, command);
	reply(RPL_CREATED, client, serv, command);
	reply(RPL_MYINFO, client, serv, command);
	
	return (0);
}

int	oper(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() < 2)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	if (command.getParam(2) != serv.getPassword())
		return (reply(ERR_PASSWDMISMATCH, client, serv, command));
	return (0);
}

int	mode(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() < 1)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	if (command.getNbParams() == 1 && command.getParam(1) == client->getNickname())
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

int     quit(Client* client, Ircserv& serv, Command& command)
{
	(void)command;
	(void)serv;
    client->setState(DCED);
	return (0);
}