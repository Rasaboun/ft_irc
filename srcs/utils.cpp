#include "ft_irc.hpp"
#include "Client.hpp"

std::string		ft_itoa(int nb)
{
	std::stringstream ss;
	ss << nb;
	std::string str = ss.str();
	return (str);
}

std::string		convert_code(int nb)
{
	std::string res = ft_itoa(nb);

	if (res.size() < 2)
		res = "0" + res;
	if (res.size() < 3)
		res = "0" + res;
	return (res);
}

std::string		convert_time(std::time_t time)
{
	return (std::asctime(std::localtime(&time)));
}

std::vector<std::string>	split(std::string param, char c)
{
	std::vector<std::string>	res;
	std::string 				str;
	size_t						position;
	
	if (param.length() == 0)
		return (res);

	while ((position = param.find(c)) != std::string::npos)
	{
		str = param.substr(0, position);
		param.erase(0, position + 1);
		if (!str.length())
			continue ;
		res.push_back(str);
	}
	if (param.length())
		res.push_back(param);
	return (res);
}

bool						sharing_channel(Client* client1, Client* client2)
{
	std::vector<std::string>	channels1 = client1->getChannels();
	std::vector<std::string>	channels2 = client2->getChannels();

	for (std::vector<std::string>::iterator it1 = channels1.begin(); it1 !=  channels1.end(); it1++)
	{
		for (std::vector<std::string>::iterator it2 = channels2.begin(); it2 !=  channels2.end(); it2++)
		{
			if (*it1 == *it2)
				return (true);
		}
	}
	return (false);
}

void						print_log(const std::string& message)
{
	time_t	secs = std::time(0);
	struct tm *curr_time = localtime(&secs);

	std::cout << "[" << curr_time->tm_hour << ":" << curr_time->tm_min << ":" << curr_time->tm_sec << "] " << message << std::endl;
}