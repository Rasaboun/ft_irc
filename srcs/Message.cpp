#include "Message.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Message::Message(const std::string& sender, Client *target, const std::string& content, bool notice):
    sender(sender),
    targets(),
    content(content),
    channelName(),
    notice(notice)
{
    targets.push_back(target);
}

Message::Message(const std::string& sender, Channel *channel, const std::string& content, bool notice):
    sender(sender),
    targets(),
    content(content),
    channelName(channel->getName()),
    notice(notice)
{
    std::vector<Client *> clients = channel->getClients();

    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if ((*it)->getFullname() == sender)
            continue ;
        targets.push_back((*it));
    }
}

void    Message::addTarget(Client* target) { targets.push_back(target); }
void    Message::send() const
{
    std::string     base;
    if (notice == false)
    {
        base = ":" + this->sender + " PRIVMSG ";
    }
    else
    {
        base = ":" + this->sender + " NOTICE ";
    }

    for (std::vector<Client *>::const_iterator it = targets.begin(); it != targets.end(); it++)
    {
        if (this->channelName.length())
            (*it)->print(base + this->channelName + " " + content);
        else
            (*it)->print(base +  (*it)->getNickname()  + " " + content);
    }
}