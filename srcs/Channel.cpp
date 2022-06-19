#include "Channel.hpp"
#include "Ircserv.hpp"

Channel::Channel(Ircserv*   serv, const std::string& name):
                name(name),
                serv(serv)
{
    (void)this->serv;
}

const std::string&  Channel::getName() const { return (this->name); }
const std::string&  Channel::getTopic() const { return (this->topic); }
const std::vector<Client *>&    Channel::getClients() const { return (this->clients); }
int                 Channel::getNbClients() const { return (this->clients.size()); }

void                Channel::addClient(Client* client)
{    
    std::string     base = " " + name + " " + name + " :";
    std::string     res;
    
    //Send join to all channel's clients
    clients.push_back(client);
    sendToClients(":" + client->getFullname() + " JOIN " + name);

    //Display channel's topic
    if (topic.length())
        res = ":ircserv " + convert_code(RPL_TOPIC) + base + topic;
    else
        res = ":ircserv " + convert_code(RPL_NOTOPIC) + base + "No topic is set";
    client->print(res);

    printClients(client);
    
}

void                Channel::removeClient(Client *client, const std::string& message)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (*it == client)
        {
            sendToClients(":" + client->getFullname()+ " PART " + name + " " + message);
            clients.erase(it);
            break ;
        }
    }
}

void                Channel::sendToClients(const std::string& message) const
{
    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
    {
       (*it)->print(message);
    }
    
}

void                Channel::printClients(Client *target) const
{
    std::string     res = ":ircserv " + convert_code(RPL_NAMREPLY) + " " + target->getNickname() + " " + name + " :";
   
    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
    {
        res += "[" + (*it)->getNickname() + "] ";
    }
    target->print(res);
    res = ":ircserv " + convert_code(RPL_ENDOFNAMES) + " " + name + " " + name + " :End of NAMES list";
    target->print(res);
}

int                 Channel::isClient(Client *client) const
{
    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (*it == client)
            return (1);
    }
    return (0);

}