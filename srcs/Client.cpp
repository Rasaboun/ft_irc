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
/*		if (curr_state == NEED_PASS)
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
*/		serv.execCommand(this, commands[i]);
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

int		Client::print(std::string message) const
{
	message += "\n";
	write(this->fd, message.c_str(), message.length());
	return (1);
}

int 				Client::getState() const { return this->state; }
int 				Client::getFd() const { return this->fd; }
const std::string&	Client::getUsername() const { return (this->username); }
const std::string&	Client::getRealname() const { return (this->realname); }
const std::string&	Client::getNickname() const { return (this->nickname); }
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
		std::cout << it->first;
		if (it->second)
			std::cout << " true\n";
		else
			std::cout << " false\n";
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

Client::Client(int fd, struct sockaddr_in address): fd(fd), state(NEED_PASS)
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
