#include "ft_irc.hpp"
#include "Client.hpp"
#include "Ircserv.hpp"

int up = 1;
int	check_params(int argc, char **argv, std::string* password, int* port)
{
	if (argc != 3)
		return (1);
	int i = 0;
	while (argv[1][i])
	{
		if (!std::isdigit(argv[1][i++]))
			return (1);
	}
	*port = std::atoi(argv[1]);
	*password = argv[2];
	return (0);
}

void 	signal_handler(int)
{
	up = 0;
}

int main(int argc, char **argv)
{
	std::string				password;
	int						port;

	if (check_params(argc, argv, &password, &port))
		return (fatal_error(STDERR_FILENO, "Wrong parameters. Correct usage : ./ircserv <port> <password>"));
	
	signal(SIGINT, signal_handler);

	Ircserv serv(port, password);
	if (serv.setup())
		return (1);
	
	while (up)
	{
		serv.run();
	}
	return (0);
}