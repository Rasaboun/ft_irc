#include "Ircserv.hpp"

int	pass(Client *client, Ircserv& serv, Command& command)
{
	if (client->getState() != NEED_PASS)
		return (reply(ERR_ALREADYREGISTERED, client, serv, command));
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	if (command.getParam(0) == serv.getPassword())
		client->setState(NEED_NICK);
	else
	{
		reply(ERR_PASSWDMISMATCH, client, serv, command);
		client->setFailedPass();
		client->setState(DCED); //Close connection if wrong pass;
		return (0); //(fatal_error(client->getFd(), "Wrong password. Closing connection"));
	}
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

	if (command.getParam(3)[0] != ':')
		return (client->print("USER :Incorrect realname"));
	if (command.getParam(3).length() == 1)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	client->setUsername(command.getParam(0));
	client->setRealname(command.joinParams(3));
	client->setState(REGISTERED);
	reply(RPL_WELCOME, client, serv, command);
	reply(RPL_YOURHOST, client, serv, command);
	reply(RPL_CREATED, client, serv, command);
	reply(RPL_MYINFO, client, serv, command);
	
	return (0);
}

int	ping(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	serv.sendPong(client, command.getParam(0));
	return (0);
}

int	pong(Client *client, Ircserv& serv, Command& command)
{
	if (command.getNbParams() == 0)
		return (reply(ERR_NEEDMOREPARAMS, client, serv, command));
	client->setLastPong();
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

int     quit(Client* client, Ircserv& serv, Command& command)
{
	(void)serv;
	client->setReason(command.joinParams(0));
    client->setState(DCED);
	return (0);
}