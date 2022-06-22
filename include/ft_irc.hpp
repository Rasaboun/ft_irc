#ifndef	FT_IRC_HPP
# define FT_IRC_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>

#include <unistd.h>
#include <netdb.h>

#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <cctype>
#include <ctime>

#include <errno.h>
#include <fcntl.h>

#include <vector>
#include <map>

#define		SERV_NAME	    "ircserv"
#define     PART_MESSAGE    "Leaving"

//Channel modes
#define     KEY         'k'
#define     INVITE      'i'
#define     LIMIT       'l'
#define     MODERATE    'm'
#define     NO_EXTERN   'n'
#define     SECRET      's'
#define     TOPIC       't'

//Channel permissions
#define     FOUNDER     'p'
#define     PROTECTED   'a'
#define     OPERATOR    'o'
#define     HALFOP      'h'
#define     VOICE       'v'


class Client    ;
class Command   ;
class Ircserv   ;

int	                        fatal_error(int fd, std::string message);
int 			            error(std::string message);
int 			            syscall_error(std::string message);
std::string		            ft_itoa(int nb);
std::string		            convert_code(int nb);
std::string		            convert_time(std::time_t* time);
std::string                 reply_prefix(const std::string& source, int code, const std::string& target);
int         	            reply(int code, Client* client, Ircserv& serv, Command& params, const std::string& param = std::string());
int         	            is_valid_nickname(const std::string& nickname);
int         	            is_valid_mode(const std::string& mode);
int                         is_valid_channel(const std::string& chan);
std::vector<std::string>	split(std::string param, char c);

#define		RPL_WELCOME					001
#define		RPL_YOURHOST				002
#define		RPL_CREATED					003
#define		RPL_MYINFO					004
#define     RPL_UMODEIS                 221
#define     RPL_LISTSTART               321
#define     RPL_LIST                    322
#define     RPL_LISTEND                 323
#define     RPL_NOTOPIC                 331
#define     RPL_TOPIC                   332
#define     RPL_TOPICWHOTIME            333
#define     RPL_NAMREPLY                353
#define     RPL_ENDOFNAMES              366

#define     ERR_NOSUCHNICK              401
#define     ERR_NOSUCHCHANNEL           403
#define     ERR_TOOMANYCHANNELS         405
#define     ERR_TOOMANYTARGETS          407
#define     ERR_NORECIPIENT             411
#define     ERR_NOTEXTTOSEND            412
#define     ERR_NONICKNAMEGIVEN         431
#define     ERR_ERRONEUSNICKNAME        432
#define     ERR_NICKNAMEINUSE           433
#define     ERR_NOTONCHANNEL            442
#define     ERR_NEEDMOREPARAMS          461
#define     ERR_ALREADYREGISTERED       462
#define     ERR_PASSWDMISMATCH          464
#define     ERR_BADCHANNELKEY           475
#define     ERR_CHANOPRIVISNEEDED       482
#define     ERR_UMODEUNKNOWNFLAG        501
#define     ERR_USERSDONTMATCH          502

#endif