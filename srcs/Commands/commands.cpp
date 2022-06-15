#include "Ircserv.hpp"

int	pass(Client *client, Ircserv& serv, std::vector<std::string> params)
{
	if (params.size() == 0)
		return (error(ERR_NEEDMOREPARAMS));
	if (params[0] == serv.getPassword())
	{
		client->setState(NEED_USER);
		return 0;
	}
}

int	nick(Client *client, Ircserv& serv, std::vector<std::string> params)
{
	if (params.size() == 0)
		return (error(ERR_NONICKNAMEGIVEN));
}