#include <iostream>
#include <string>
#include <unistd.h>

int	fatal_error(int fd, std::string message)
{
	message = "ERROR " + message + "\n";
	write(fd, message.c_str(), message.length());
	return (1);
}

int syscall_error(std::string syscall)
{
	std::cerr << "System call: " << syscall << "() failed." << std::endl;
	return (1);
}