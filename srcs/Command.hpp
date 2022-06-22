#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "ft_irc.hpp"

class Client;

class Command
{
	public:

	Command(std::string& command);

	std::string							getPrefix() const;
	std::string							getName() const;
	std::vector<std::string>			getParams();
	const std::string					getParam(size_t i) const;
	int									getNbParams() const;
	
	void								setParam(size_t, const std::string&);

	void								display() const;
	std::string							joinParams(size_t pos) const;
	private:

	std::string							prefix;	
	std::string							name;
	std::vector<std::string>			params;
};

#endif