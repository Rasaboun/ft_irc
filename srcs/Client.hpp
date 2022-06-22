#ifndef CLIENT_HPP
# define CLIENT_HPP

# define NEED_PASS 1
# define NEED_NICK 2
# define NEED_USER 3
# define REGISTERED 4
# define DCED	5

# include "ft_irc.hpp"
# include "Command.hpp"
# include "Message.hpp"

class Ircserv;

class Client
{
	public:

	Client(int fd, struct sockaddr_in address);
	Client();
	~Client();

	const Client&		operator=(const Client& rhs);
	
	void				receive(Ircserv& serv);
	int					print(std::string message) const;

	int					getState() const;
	int					getFd() const;
	const std::string&	getUsername() const;
	const std::string&	getRealname() const;
	const std::string&	getNickname() const;
	const std::string	getFullname() const;
	bool				getMode(const char& mode) const;
	std::string			getModes() const;
	char				getChanPerm(const std::string&) const;
	time_t				getLastPing()const;
	int					getNbChannels() const;

	void				setState(const int new_state);
	void				setUsername(const std::string& new_username);
	void				setRealname(const std::string& new_realname);
	void				setNickname(const std::string& new_nickname);
	void				setMode(const char& mode, bool value);
	void				setPerm(const std::string&, char);
	void				setLastPing();

	void				addMessage(const Message& message);
	void				sendMessages();
	void				addChannel(const std::string& channel, char);
	void				removeChannel(const std::string& channel);

	private:

	int								fd;
	std::string						data;
	std::string						hostname;
	int								state;

	std::string						username;
	std::string						realname;
	std::string						nickname;

	std::vector<Command>			commands;
	std::map<const char, bool>		modes;
	std::vector<Message>			messages;
	std::map<std::string, char>		channels;

	time_t								lastPing;
	void				handle_input(Ircserv& serv);

};

#endif