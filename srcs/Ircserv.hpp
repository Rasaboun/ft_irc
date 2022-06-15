#ifndef IRC_SERV_HPP
# define IRC_SERV_HPP

# include "ft_irc.hpp"
# include "Client.hpp"

#define CMD_TYPE	

class Client;

class Ircserv
{	
	public:

	typedef	void (*cmd_type)(Client*, Ircserv&, std::vector<std::string>);

	Ircserv(int port, const std::string& password);
	~Ircserv();
	
	int									setup();
	void								run();
	void								execCommand(Client* client, Command& command);

	void								removeClient(Client *client);

	const std::string&					getPassword() const ;

	private:

	int									port;
	const std::string					password;

	int									fd;
	struct sockaddr_in					address;
	std::vector<pollfd>					client_fds;

	std::map<int, Client *>				clients;
	std::map<std::string, cmd_type>		commands;
};


void	pass(Client *client, Ircserv& serv, std::vector<std::string> params);

#endif