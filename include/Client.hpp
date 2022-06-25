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

	const Client&				operator=(const Client& rhs);
	
	void						receive(Ircserv& serv);
	int							print(std::string message) const;

	int							getState() const;
	int							getFd() const;
	const std::string&			getUsername() const;
	const std::string&			getRealname() const;
	const std::string&			getNickname() const;
	const std::string&			getHostname() const;
	const std::string			getFullname() const;
	const std::string&			getReason() const;
	bool						getMode(const char& mode) const;
	std::string					getModes() const;
	time_t						getLastPing()const;
	time_t						getLastPong()const;
	std::vector<std::string>	getChannels() const;
	int							getNbChannels() const;

	void						setState(const int new_state);
	void						setUsername(const std::string&);
	void						setRealname(const std::string&);
	void						setNickname(const std::string&);
	void						setReason(const std::string&);
	void						setMode(const char& mode, bool value, Ircserv& serv);
	void						setLastPing();
	void						setLastPong();

	void						addMessage(const Message& message);
	void						sendMessages();
	void						addChannel(const std::string&);
	void						removeChannel(const std::string& channel);

	int							isOperator() const;

	private:

	int								fd;
	std::string						data;
	std::string						hostname;
	int								state;

	std::string						username;
	std::string						realname;
	std::string						nickname;
	std::string						reason;

	std::vector<Command>			commands;
	std::map<const char, bool>		modes;
	std::vector<Message>			messages;
	std::vector<std::string>		channels;

	time_t							lastPing;
	time_t							lastPong;

	void							handle_input(Ircserv& serv);

};

#endif