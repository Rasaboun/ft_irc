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

std::string	Command::getPrefix() const { return (this->prefix); }
std::string	Command::getName() const { return (this->name); }
std::vector<std::string>	Command::getParams() { return (this->params); }
const std::string	Command::getParam(size_t i) const 
{
	if (i >= this->params.size())
		return (std::string());
	return (this->params[i]);
}

int				Command::getNbParams() const { return (this->params.size()); }

void			Command::setParam(size_t pos, const std::string& value)
{
	if (pos >= params.size())
		return ;
	params[pos] = value;
}

void			Command::display() const 
{
	std::cout << this->getName();
	for (size_t i = 0; i < params.size(); i++)
	{
		std::cout << " " << params[i];
	}
	std::cout << std::endl;
}

std::string		Command::joinParams(size_t pos) const
{
	std::string res = "";
	size_t		nb_params = params.size();

	while (pos < nb_params)
	{
		res += params[pos++];
		if (pos < nb_params)
			res += " ";
		else
			break ;
	}
	return (res);
}