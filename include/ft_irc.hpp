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
int         is_valid_mode(const std::string& mode);

#define     RPL_UMODEIS                 221

#define     ERR_NONICKNAMEGIVEN         431
#define     ERR_ERRONEUSNICKNAME        431
#define     ERR_NICKNAMEINUSE           431
#define     ERR_NEEDMOREPARAMS          461
#define     ERR_ALREADYREGISTERED       462
#define     ERR_PASSWDMISMATCH          464
#define     ERR_UMODEUNKNOWNFLAG        501
#define     ERR_USERSDONTMATCH          502

#endif