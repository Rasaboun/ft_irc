#include "Client.hpp"
#include "Ircserv.hpp"

# define BUFFER_SIZE 1024

void	Client::handle_input(Ircserv& serv)
{
	int curr_state = this->state;

	if (curr_state == DCED)
		return ;
	for (size_t i = 0; i < commands.size(); i++)
	{
		if (curr_state == NEED_PASS)
		{
			if (commands[i].getName() != "PASS")
				continue ;
		}
		else if (curr_state == NEED_NICK)
		{
			if (commands[i].getName() != "NICK")
				continue ;
		}
		else if (curr_state == NEED_USER)
		{
			if (commands[i].getName() != "USER")
				continue ;
		}
		serv.execCommand(this, commands[i]);
		//commands[i].exec(this);
		this->commands.erase(commands.begin() + i);
	}
}

void 	Client::receive(Ircserv& serv)
{	
	std::string crlf("\r\n");

	while (this->data.find(crlf) == std::string::npos)
	{
		char buffer[BUFFER_SIZE + 1];
		ssize_t size;
		if ((size = recv(this->fd, &buffer, BUFFER_SIZE, 0)) < 0)
			return ;
		std::cout << "Received : " << buffer << std::endl;
		if (size == 0)
		{
			this->state = DCED;
			std::cout << "Disconected\n";
			return ;
		}
		buffer[size] = 0;
		this->data += buffer;

	}

	size_t		position;
	while ((position = this->data.find(crlf)) != std::string::npos)
	{
		std::string command = this->data.substr(0, position);
		this->data.erase(0, position + 2);
		if (!command.length())
			continue ;
		
		this->commands.push_back(Command(command));
	}
	handle_input(serv);
}

void		Client::print(std::string message) const
{
	message += "\n";
	write(this->fd, message.c_str(), message.length());
}

int 				Client::getState() const { return this->state; }
int 				Client::getFd() const { return this->fd; }
const std::string&	Client::getNickname() const { return (this->nickname); }

void				Client::setNickname(const std::string& new_nickname) { this->nickname = new_nickname; }
void				Client::setState(const int new_state) { this->state = new_state; }

Client::Client(int fd, struct sockaddr_in address): fd(fd), state(NEED_PASS)
{

	fcntl(fd, F_SETFL, O_NONBLOCK);
	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *) &address, sizeof(address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		syscall_error("getnameinfo");
	else
		this->hostname = hostname;
	//print("Welcome to my IRC server\n");
}

Client::Client():
		state(NEED_PASS)
{
	
}

const Client& Client::operator=(const Client& rhs)
{
	this->fd = rhs.fd;
	this->hostname = rhs.hostname;
	
	return *this;
}

Client::~Client()
{
	close(this->fd);
	std::cout << "Client destructor called\n";
}
