#ifndef CLIENT_HPP
# define CLIENT_HPP

# define NEED_PASS 1
# define NEED_NICK 2
# define NEED_USER 3
# define REGISTERED 4
# define DCED	5

# include "ft_irc.hpp"
# include "Command.hpp"


class Ircserv;

class Client
{
	public:

	Client(int fd, struct sockaddr_in address);
	Client();
	~Client();

	const Client&		operator=(const Client& rhs);
	
	void				receive(Ircserv& serv);

	int					getState() const;
	int					getFd() const;

	void				setState(const int new_state);

	private:

	int							fd;
	std::string					data;
	std::string					hostname;
	std::string					nickname;

	std::vector<Command>		commands;

	void				handle_input(Ircserv& serv);

	int					state;
};

#endif