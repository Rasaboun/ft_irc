#ifndef	FT_IRC_HPP
# define FT_IRC_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>

#include <unistd.h>
#include <netdb.h>

#include <string>
#include <cstring>
#include <cstdlib>
#include <csignal>


#include <fcntl.h>

#include <vector>
#include <map>

int 		error(std::string message);
int 		syscall_error(std::string message);

#define ERR_NONICKNAMEGIVEN	"No nickname given\n"

#endif