#include <iostream>
#include <string>

int error(std::string message)
{
	std::cerr << message << std::endl;
	return (1);
}

int syscall_error(std::string syscall)
{
	std::cerr << "System call: " << syscall << "() failed." << std::endl;
	return (1);
}