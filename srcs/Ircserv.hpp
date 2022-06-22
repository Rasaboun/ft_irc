#ifndef IRC_SERV_HPP
# define IRC_SERV_HPP

# include "ft_irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# define	TIMEOUT 1000
# define	MYPING 5
# define	CHANLIMIT 1

class Ircserv
{	
	public:

	typedef	int (*cmd_type)(Client*, Ircserv&, Command&);

	Ircserv(int port, const std::string& password);
	~Ircserv();
	
	int									setup();
	void								run();
	void								execCommand(Client* client, Command& command);

	void								addChannel(const std::string& name);
	void								removeClient(Client *client);
	void								removeChannel(Channel *channel);

	int									isChannel(const std::string& name) const ;
	int									availableNickname(const std::string& nickname);

	void								sendPong(Client*, const std::string&) const;
	void									sendPing();

	const std::string&					getPassword() const;
	const std::string&					getName() const;
	const std::string					getPrefix() const;
	Client*								getClient(const std::string& nickname) const;
	Channel*							getChannel(const std::string& name) const;
	std::map<std::string, Channel *>	getChannels() const;
	int									getNbChannels() const;

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
};


int		pass(Client *client, Ircserv& serv, Command& command);
int		nick(Client *client, Ircserv& serv, Command& command);
int		user(Client *client, Ircserv& serv, Command& command);
int		oper(Client *client, Ircserv& serv, Command& command);
int		mode(Client *client, Ircserv& serv, Command& command);
int     quit(Client* client, Ircserv& serv);
int		msg(Client *client, Ircserv& serv, Command& command);
int		join(Client *client, Ircserv& serv, Command& command);
int		part(Client *client, Ircserv& serv, Command& command);
int		ping(Client *client, Ircserv& serv, Command& command);
int		topic(Client *client, Ircserv& serv, Command& command);
int		list(Client *client, Ircserv& serv, Command& command);
#endif