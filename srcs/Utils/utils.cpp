#include "ft_irc.hpp"

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

std::string		convert_time(std::time_t* time)
{
	return (std::asctime(std::localtime(time)));
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