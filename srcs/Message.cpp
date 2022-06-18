#include "Message.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Message::Message(const std::string& sender, Client *target, const std::string& content):
    sender(sender),
    targets(),
    content(content)
{
    targets.push_back(target);
}

Message::Message(const std::string& sender, Channel *channel, const std::string& content):
    sender(sender),
    targets(),
    content(content)
{
    std::vector<Client *> clients = channel->getClients();
    std::cout << "Channel size : " << clients.size() << std::endl;

    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        std::cout << "Adding new target\n";
        targets.push_back((*it));
    }
}

void    Message::addTarget(Client* target) { targets.push_back(target); }
void    Message::send() const
{
    std::string     base = ":" + this->sender + " PRIVMSG ";
    std::cout << "In send\n";
    for (std::vector<Client *>::const_iterator it = targets.begin(); it != targets.end(); it++)
    {
        (*it)->print(base + "#test "/* (*it)->getNickname() */ + " " + content);
    }
}