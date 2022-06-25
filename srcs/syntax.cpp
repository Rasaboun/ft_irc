#include "ft_irc.hpp"

int     isspecial(const char& c)
{
    if (c != '[' && c != ']' && c != '\\' && c != '`' && c != '_' && \
        c != '^' && c != '{' && c != '}' && c != '|')
        return (0);
    return (1);
}

int     is_valid_nickname(const std::string& nickname)
{
    if (nickname.length() > NICKLEN)
        return (0);
    if (!isspecial(nickname[0]) && !isalpha(nickname[0]))
        return (0);
    for (size_t i = 1; i < nickname.length(); i++)
    {
        if (!isalpha(nickname[i]) && !isdigit(nickname[i]) && !isspecial(nickname[i]) \
                                  && nickname[i] != '-')
            return (0);
    }
    return (1);
}

// int     is_valid_mode(const std::string& mode)
// {
//     if (mode.length() != 2)
//         return (0);
//     if (mode[0] != '+' && mode[0] != '-')
//         return (0);
//     if (mode[1] != 'a' && mode[1] != 'i' && mode[1] != 'w' && mode[1] != 'r' && mode[1] != 'o' \
//                         && mode[1] != 'O' && mode[1] != 's')
//         return (0);
//     return (1);
// }

char     is_add_or_remove_mode(const std::string& mode)
{
    char    sign = '?';
    for (int i = 0; i < mode.length() && (mode[i] == '-' || mode[i] == '+'); i++)
    {
        if (mode[i] == '-')
            sign = '-';
        if (mode[i] == '+')
            sign = '+';
    }
    return (sign);
}

int     is_valid_client_mode(const char& mode)
{
    if (mode != 'o' && mode != 'w' && mode != 'i' && mode != 'a' && mode != '+' && mode != '-')
        return (0);
    return (1);
}

int     is_valid_mode(const std::string& mode)
{
    std::cout << " le MODE = |" << mode << "|" << std::endl;
    if (mode.length() <= 1)
        return (0);
    if (mode[0] != '+' && mode[0] != '-')
        return (0);
    for (int i = 1; i < mode.length(); i++)
    {
        if (!is_valid_client_mode(mode[i]))
            return (0);
    }
    return (1);
}

int     is_chan_mode(char mode)
{
    if (mode != 'k' && mode != 'i' && mode != 'l' && mode != 'm' && mode != 'n' && \
        mode != 's' && mode != 't' && mode != 'b' && mode != 'o')
        return (0);
    return (1);
}

int     is_param_mode(char mode)
{
    if (mode != 'k' && mode != 'l' && mode != 'b' && mode != 'o')
        return (0);
    return (1);
}

int     is_valid_channel(const std::string& chan)
{
    if (chan.length() > 50)
        return (0);
    if (chan[0] != '#')
        return (0);
    for (size_t i = 1; i < chan.length(); i++)
    {
        if (chan[i] == 7 || chan[i] == ' ')
            return (0);
    }
    return (1);
}