#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "ft_irc.hpp"

class Client;

class Command
{
	public:

	Command(std::string& command);

	std::string					getPrefix() const;
	std::string					getName() const;
	const std::vector<std::string>&	getParams() const;

	void						exec(Client* client);

	private:

	std::string					prefix;	
	std::string					name;
	std::vector<std::string>	params;
};

#endif