#ifndef IRC_SERV_HPP
# define IRC_SERV_HPP

# include "ft_irc.hpp"
# include "Client.hpp"

#define CMD_TYPE	

class Client;

class Ircserv
{	
	public:

	typedef	int (*cmd_type)(Client*, Ircserv&, Command&);

	Ircserv(int port, const std::string& password);
	~Ircserv();
	
	int									setup();
	void								run();
	void								execCommand(Client* client, Command& command);

	void								removeClient(Client *client);
	int									availableNickname(const std::string& nickname);

	const std::string&					getPassword() const ;
	Client*								getClient(const std::string& nickname) const;
	private:

	int									port;
	const std::string					password;

	int									fd;
	struct sockaddr_in					address;
	std::vector<pollfd>					client_fds;

	std::map<int, Client *>				clients;
	std::map<std::string, cmd_type>		commands;
};


int		pass(Client *client, Ircserv& serv, Command& command);
int		nick(Client *client, Ircserv& serv, Command& command);
int		user(Client *client, Ircserv& serv, Command& command);
int		oper(Client *client, Ircserv& serv, Command& command);
int		mode(Client *client, Ircserv& serv, Command& command);
int     quit(Client* client, Ircserv& serv, Command& command);

#endif