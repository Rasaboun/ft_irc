#include <iostream>
#include <string>

int error(std::string message)
{
	std::cout << message << std::endl;
	return (1);
}

int syscall_error(std::string syscall)
{
	std::cout << "System call: " << syscall << "() failed." << std::endl;
	return (1);
}