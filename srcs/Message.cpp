#include "Message.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Message::Message(const std::string& sender, Client *target, const std::string& content):
    sender(sender),
    targets(),
    content(content),
    channelName()
{
    targets.push_back(target);
}

Message::Message(const std::string& sender, Channel *channel, const std::string& content):
    sender(sender),
    targets(),
    content(content),
    channelName(channel->getName())
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
    std::string     base = ":" + this->sender + " PRIVMSG ";

    for (std::vector<Client *>::const_iterator it = targets.begin(); it != targets.end(); it++)
    {
        if (this->channelName.length())
            (*it)->print(base + this->channelName + " " + content);
        else
            (*it)->print(base +  (*it)->getNickname()  + " " + content);
    }
}