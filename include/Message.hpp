#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include "ft_irc.hpp"

class Client;
class Channel;

class Message
{
    public:

    Message(const std::string& sender, Client *, const std::string& content, bool notice);
    Message(const std::string& sender, Channel *channel, const std::string& content, bool notice);

    void            addTarget(Client* target);

    void            send() const;

    private:

    std::string             sender;
    std::vector<Client *>   targets;
    std::string             content;
    std::string             channelName;
    bool                    notice;
};

#endif