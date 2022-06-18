#include "Client.hpp"
#include "Ircserv.hpp"

# define BUFFER_SIZE 1024

void	Client::handle_input(Ircserv& serv)
{
	int		previous_state = this->state;

	std::vector<Command>::iterator it = commands.begin();
	while (it != commands.end())
	{
		it->display();
		if (this->state == DCED)
			break ;
		if (this->state == NEED_PASS)
		{
			if (it->getName() != "PASS")
			{
				it++;
				continue ;
			}
		}
		else if (this->state == NEED_NICK)
		{
			if (it->getName() != "NICK"){

				it++;
				continue ;
			}
		}
		else if (this->state == NEED_USER)
		{
			if (it->getName() != "USER")
			{
				it++;
				continue ;
			}
		}
		
		serv.execCommand(this, *it);
		this->commands.erase(it);
		
	}
	if (this->state != previous_state && this->state != DCED) // Successfully registered
		handle_input(serv);
	sendMessages();
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
			std::cout << "Disconnected\n";
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

void	Client::sendMessages()
{
	for (std::vector<Message>::const_iterator it = messages.begin(); it != messages.end(); it++)
		it->send();
	messages.clear();
}

int		Client::print(std::string message) const
{
	message += "\r\n";
	std::cout << getNickname() << " sending : " << message;
	write(this->fd, message.c_str(), message.length());
	return (1);
}

int 				Client::getState() const { return this->state; }
int 				Client::getFd() const { return this->fd; }
const std::string&	Client::getUsername() const { return (this->username); }
const std::string&	Client::getRealname() const { return (this->realname); }
const std::string&	Client::getNickname() const { return (this->nickname); }
const std::string	Client::getFullname() const { return (this->nickname + "!" + this->username + "@" + this->hostname); }

bool				Client::getMode(const char& mode) const
{
	std::map<const char, bool>::const_iterator it = this->modes.find(mode);

	if (it == this->modes.end())
		return (false);
	return (it->second);
}
std::string		Client::getModes() const
{
	std::string res;
	
	for(std::map<const char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
	{
		if (it->second)
			res += it->first;
	}
	return (res);
}


void				Client::setUsername(const std::string& new_username) { this->username = new_username; }
void				Client::setRealname(const std::string& new_realname) { this->realname = new_realname; }
void				Client::setNickname(const std::string& new_nickname) { this->nickname = new_nickname; }
void				Client::setState(const int new_state) { this->state = new_state; }
void				Client::setMode(const char& mode, bool value)
{
	if (this->modes.count(mode) == 0)
		return ;
	this->modes[mode] = value;
}

Client::Client(int fd, struct sockaddr_in address):
			fd(fd),
			state(NEED_NICK),
			username("*"),
			realname("*"),
			nickname("*")
{

	fcntl(fd, F_SETFL, O_NONBLOCK);
	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *) &address, sizeof(address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		syscall_error("getnameinfo");
	else
		this->hostname = hostname;

	modes['a'] = false;
	modes['i'] = false;
	modes['w'] = false;
	modes['r'] = false;
	modes['o'] = false;
	modes['O'] = false;
	modes['s'] = false;
}

void		Client::addMessage(const Message& message) { messages.push_back(message); }

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
