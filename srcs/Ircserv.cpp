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
	if (poll(&client_fds[0], client_fds.size(), 5
	 * 1000) == -1)
		return ;
	std::cout << clients.size() << " USERS CONNECTED\n";
	if (std::time(0) - lastPing >= MYPING)
	{
		std::cout << "PING SEND" << std::endl;
		sendPing();
		lastPing = std::time(0);
	}
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

int		Ircserv::execCommand(Client* client, Command& command)
{

	std::map<std::string, Ircserv::cmd_type >::iterator it = commands.find(command.getName());
	if (it == commands.end())
		return (1);
	(*it->second)(client, *this, command);
	return (0);
}


void	Ircserv::addChannel(const std::string& name)
{
	if (this->channels.count(name))
		return ;
	this->channels[name] = new Channel(this, name);
}

void	Ircserv::addClientToWallops(Client *client)
{
	if (!client)
		return ;
	this->wallops_clients.push_back(client);
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

	std::vector<std::string> channels = client->getChannels();
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		getChannel(*it)->removeClient(client, client->getReason());
	}

	clients.erase(client->getFd());
	delete client;
}


void	Ircserv::removeChannel(Channel* channel)
{
	channels.erase(channel->getName());
	delete channel;
}

void	Ircserv::removeClientFromWallops(Client *client)
{
	if (!client)
		return ;
	for (std::vector<Client *>::iterator it = wallops_clients.begin(); it != wallops_clients.end(); it++)
    {
    	if (*it == client)
    	{
    		this->wallops_clients.erase(it);
    		break ;
    	}
    }
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



void					Ircserv::sendPing()
{
	time_t now = std::time(0);

	for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (now - it->second->getLastPong() >= TIMEOUT)
		{
			std::cout << "PING TIMEOUT" << std::endl;
			it->second->setState(DCED);
		}
		else{
			it->second->print("Ping " + getName());
		}
	}
}

void				Ircserv::sendToClients(const std::string& message) const
{
	for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		it->second->print(message);
	}

}

const std::string& 	Ircserv::getPassword() const { return (this->password); }
const std::string& 	Ircserv::getOpPassword() const { return (this->opPassword); }
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
int									Ircserv::getNbChannels() const { return (channels.size()); }

std::vector<Client *>				Ircserv::getWallopsClients() const { return (this->wallops_clients); }

Ircserv::Ircserv(int port, const std::string& password):
		port(port),
		password(password),
		opPassword(OP_PASSWORD),
		name(SERV_NAME),
		lastPing(std::time(0))
{
	commands["CAP"] = cap;
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
	commands["PONG"] = pong;
	commands["TOPIC"] = topic;
	commands["LIST"] = list;
	commands["INVITE"] = invite;
	commands["KICK"] = kick;
	commands["WHO"] = who;
	commands["kill"] = kill;
	commands["wallops"] = wallops;
	commands["NOTICE"] = notice;
	commands["WHOIS"] = whois;
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