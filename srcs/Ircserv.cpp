#include "Ircserv.hpp"

int		Ircserv::setup()
{
	int optval = 1;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (syscall_error("socket"));
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)) < 0)
		return (syscall_error("setsockopt"));
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		return (syscall_error("fcntl"));
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		return (syscall_error("bind"));
	if (listen(fd, address.sin_port) < 0)
		return (syscall_error("listen"));
	
	client_fds.push_back(pollfd());
	client_fds[0].fd = fd;
	client_fds[0].events = POLLIN;

	return (0);
}

void 	Ircserv::run()
{
	if (poll(&client_fds[0], client_fds.size(), 60 * 1000) == -1)
		return ;
	std::cout << clients.size() << " USERS CONNECTED\n";
	if (client_fds[0].revents == POLLIN)
	{
		//accept new connection
		std::cout << "New client connection\n";
		struct sockaddr_in client_address;
		socklen_t csin_len = sizeof(client_address);
		int client_fd = accept(fd, (struct sockaddr *)&client_address, &csin_len);
		if (client_fd == -1)
			return ;

		clients[client_fd] = new Client(client_fd, client_address);

		client_fds.push_back(pollfd());
		client_fds.back().fd = client_fd;
		client_fds.back().events = POLLIN;
		
	}
	else
	{
		for (std::vector<pollfd>::iterator it = client_fds.begin(); it != client_fds.end(); it++)
		{
			if ((*it).revents == POLLIN)
			{
				clients[(*it).fd]->receive(*this);
			}
		}
	}
	std::map<int, Client *>::iterator it = clients.begin();
	while (it != clients.end())
	{
		Client *client = (*it++).second;
		if (client && client->getState() == DCED)
		{
			removeClient(client);
		}
		
	}

	std::map<std::string, Channel *>::iterator ite = channels.begin();
	while (ite != channels.end())
	{
		Channel *channel = (*ite++).second;
		if (channel && channel->getNbClients() == 0)
		{
			removeChannel(channel);
		}
	}
	
}

void	Ircserv::execCommand(Client* client, Command& command)
{

	std::map<std::string, Ircserv::cmd_type >::iterator it = commands.find(command.getName());

	if (it == commands.end())
		return ;
	(*it->second)(client, *this, command);
}


void	Ircserv::addChannel(const std::string& name)
{
	if (this->channels.count(name))
		return ;
	this->channels[name] = new Channel(this, name);
}

void	Ircserv::removeClient(Client* client)
{
	for (std::vector<pollfd>::iterator fd_it = client_fds.begin(); fd_it != client_fds.end(); fd_it++)
	{
		if ((*fd_it).fd == client->getFd())
		{
			client_fds.erase(fd_it);
			break ;
		}
	}
	clients.erase(client->getFd());
	delete client;
}


void	Ircserv::removeChannel(Channel* channel)
{
	channels.erase(channel->getName());
	delete channel;
}

int		Ircserv::isChannel(const std::string& name) const { return (this->channels.count(name)); }

int		Ircserv::availableNickname(const std::string& nickname)
{
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (0);
	}
	return (1);
}

void				Ircserv::sendPong(Client* target, const std::string& token) const
{ 
	target->print("PONG " + token); 

}



int					Ircserv::sendPing(Client *client, Ircserv& serv, Command& command)
{
	time_t now = std::time(0);

	for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (now - it->second->getLastPing() >= TIMEOUT)
		{
			quit(it->second, serv, command);
		}
		else{
			it->second->print("Ping " + getName());
		}
	}
}

const std::string& 	Ircserv::getPassword() const { return (this->password); }
const std::string& 	Ircserv::getName() const { return (this->name); }
const std::string	Ircserv::getPrefix() const { return (":" + this->name); }
Client*				Ircserv::getClient(const std::string& nickname) const
{
	for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
	}
	return (NULL);
}

Channel*			Ircserv::getChannel(const std::string& name) const 
{
	for (std::map<std::string, Channel *>::const_iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->second->getName() == name)
			return (it->second);
	}
	return (NULL);

}
std::map<std::string, Channel *>	Ircserv::getChannels() const { return (this->channels); }
int				Ircserv::getNbChannels() const { return (channels.size()); }


Ircserv::Ircserv(int port, const std::string& password):
		port(port),
		password(password),
		name(SERV_NAME)
{
	commands["PASS"] = pass;
	commands["NICK"] = nick;
	commands["USER"] = user;
	commands["OPER"] = oper;
	commands["MODE"] = mode;
	commands["QUIT"] = quit;
	commands["PRIVMSG"] = msg;
	commands["JOIN"] = join;
	commands["PART"] = part;
	commands["PING"] = ping;
	commands["TOPIC"] = topic;
	commands["LIST"] = list;
}

Ircserv::~Ircserv()
{
	if (fd > 0)
		close(fd);
	std::map<int, Client*>::iterator it = clients.begin();
	while (it != clients.end())
	{
		removeClient(it++->second);
	}
	std::map<std::string, Channel*>::iterator ite = channels.begin();
	while (ite != channels.end())
	{
		removeChannel(ite++->second);
	}
}