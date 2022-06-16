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
#include <cctype>

#include <errno.h>
#include <fcntl.h>

#include <vector>
#include <map>

class Client    ;
class Command   ;
class Ircserv   ;

int 		error(std::string message);
int 		syscall_error(std::string message);
int         reply(int code, Client* client, Ircserv& serv, Command& params);
int         is_valid_nickname(const std::string& nickname);
#endif