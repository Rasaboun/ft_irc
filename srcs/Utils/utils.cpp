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