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
    if (nickname.length() > 9)
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

int     is_valid_mode(const std::string& mode)
{
    if (mode.length() != 2)
        return (0);
    if (mode[0] != '+' && mode[1] != '-')
        return (0);
    if (mode[1] != 'a' && mode[1] != 'i' && mode[1] != 'w' && mode[1] != 'r' && mode[1] != 'o' \
                        && mode[1] != 'O' && mode[1] != 's')
        return (0);
    return (1);
}