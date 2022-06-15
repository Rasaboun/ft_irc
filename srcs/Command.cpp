#include "Command.hpp"

Command::Command(std::string& command):
	prefix(),
	name(),
	params()
{
	if (command[0] == ':')
	{
		this->prefix = strtok(const_cast<char *>(command.c_str()), " ");

		if (command.length() == this->prefix.length())
			return ;
 	}

	if (this->prefix.length() > 0)
		this->name = strtok(NULL, " ");
	else
		this->name = strtok(const_cast<char *>(command.c_str()), " ");
	
	char *token = strtok((NULL), " ");
	while (token != NULL)
	{
		this->params.push_back(token);
		token = strtok((NULL), " ");
	}
}

void Command::exec(Client* client)
{
	std::cout << "Executing " << this->name << std::endl;
	(void)client;
}

std::string	Command::getPrefix() const { return (this->prefix); }
std::string	Command::getName() const { return (this->name); }
const std::vector<std::string>&	Command::getParams() const { return (this->params); }