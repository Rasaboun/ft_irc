#ifndef IRC_SERV_HPP
# define IRC_SERV_HPP

# include "ft_irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define	TIMEOUT 		300
# define	MYPING 			15
# define	NOPASS_TIMEOUT	15
class Ircserv
{	
	public:

	typedef	int (*cmd_type)(Client*, Ircserv&, Command&);

	Ircserv(int port, const std::string& password);
	~Ircserv();
	
	int									setup();
	void								run();
	int									execCommand(Client* client, Command& command);

	void								addChannel(const std::string& name);
	void								addClientToWallops(Client *client);
	void								removeClient(Client *client);
	void								removeChannel(Channel *channel);
	void								removeClientFromWallops(Client *client);

	int									isChannel(const std::string& name) const ;
	bool								isClient(const std::string& name) const;
	int									availableNickname(const std::string& nickname);

	void								sendPong(Client*, const std::string&) const;
	void								sendPing();
	void								sendToClients(const std::string&) const;

	const std::string&					getPassword() const;
	const std::string&					getName() const;
	const std::string					getPrefix() const;
	Client*								getClient(const std::string& nickname) const;
	Channel*							getChannel(const std::string& name) const;
	std::map<std::string, Channel *>	getChannels() const;
	int									getNbChannels() const;
	std::vector<Client *>				getWallopsClients() const;

	private:

	int									port;
	const std::string					password;
	const std::string					name;
	time_t								lastPing;
	int									fd;
	struct sockaddr_in					address;
	std::vector<pollfd>					client_fds;

	std::map<int, Client *>				clients;
	std::map<std::string, cmd_type>		commands;
	std::map<std::string, Channel *>	channels;
	std::vector<Client *>           	wallops_clients;
};


int		cap(Client *client, Ircserv& serv, Command& command);
int		pass(Client *client, Ircserv& serv, Command& command);
int		nick(Client *client, Ircserv& serv, Command& command);
int		user(Client *client, Ircserv& serv, Command& command);
int		oper(Client *client, Ircserv& serv, Command& command);
int		mode(Client *client, Ircserv& serv, Command& command);
int     quit(Client* client, Ircserv& serv, Command& command);
int		msg(Client *client, Ircserv& serv, Command& command);
int		join(Client *client, Ircserv& serv, Command& command);
int		part(Client *client, Ircserv& serv, Command& command);
int		ping(Client *client, Ircserv& serv, Command& command);
int		pong(Client *client, Ircserv& serv, Command& command);
int		topic(Client *client, Ircserv& serv, Command& command);
int		list(Client *client, Ircserv& serv, Command& command);
int		invite(Client *client, Ircserv& serv, Command& command);
int		kick(Client *client, Ircserv& serv, Command& command);
int		who(Client *client, Ircserv& serv, Command& command);
int		kill(Client *client, Ircserv& serv, Command& command);
int		wallops(Client *client, Ircserv& serv, Command& command);
int	notice(Client *client, Ircserv& serv, Command& command);
#endif