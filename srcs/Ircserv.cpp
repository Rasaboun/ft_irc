#include "Ircserv.hpp"

int		Ircserv::setup()
{
	int optval = 1;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return (syscall_error("socket"));
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
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
	if (0) // if curr time - last ping > ping
	{
		//send ping to each user
	}
	else
	{
		if (client_fds[0].revents == POLLIN)
		{
			//accept new connection
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
		
	}
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

int		Ircserv::availableNickname(const std::string& nickname)
{
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (0);
	}
	return (1);
}

void	Ircserv::execCommand(Client* client, Command& command)
{

	std::map<std::string, Ircserv::cmd_type >::iterator it = commands.find(command.getName());

	if (it == commands.end())
		return ;
	(*it->second)(client, *this, command);
}

const std::string& Ircserv::getPassword() const { return (this->password); }

Ircserv::Ircserv(int port, const std::string& password):
		port(port),
		password(password)
{
	commands["PASS"] = pass;
	commands["NICK"] = nick;
}

Ircserv::~Ircserv()
{
	std::cout << "Destructor called\n" << std::endl;
	std::map<int, Client*>::iterator it = clients.begin();
	while (it != clients.end())
	{
		removeClient(it++->second);
	}
}