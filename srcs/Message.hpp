#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include "ft_irc.hpp"

class Client;
class Channel;

class Message
{
    public:

    Message(const std::string& sender, Client *, const std::string& content);
    Message(const std::string& sender, Channel *channel, const std::string& content);

    void            addTarget(Client* target);

    void            send() const;

    private:

    std::string             sender;
    std::vector<Client *>   targets;
    std::string             content;
};

#endif