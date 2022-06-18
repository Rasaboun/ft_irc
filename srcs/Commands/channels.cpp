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
        std::cout << "Users in chan : " << serv.getChannel(command.getParam(0))->getNbClients() << std::endl;
    
    return (0);
}