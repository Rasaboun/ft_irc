#include "Channel.hpp"
#include "Ircserv.hpp"

const std::string&  Channel::getName() const { return (this->name); }
const std::string&  Channel::getTopic() const { return (this->topic); }
const std::vector<Client *>&    Channel::getClients() const { return (this->clients); }
int                 Channel::getNbClients() const { return (this->clients.size()); }

void                Channel::addClient(Client* client)
{        
    //Send join to all channel's clients
    clients.push_back(client);
    sendToClients(":" + client->getFullname() + " JOIN " + name);

    sendTopic(client);

    printClients(client);
    
}

void                Channel::setTopic(const std::string& topic){ this->topic = topic; } 


void                Channel::editTopic(Client* editor, const std::string& topic)
{
    setTopic(topic);
    sendToClients(":" + editor->getNickname() +  " TOPIC " + name + " " + topic);
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

void                Channel::sendTopic(Client *client) const
{
    std::string     base = " " + name + " " + name + " :";
 
    if (topic.length())
        base = ":ircserv " + convert_code(RPL_TOPIC) + base + topic;
    else
        base = ":ircserv " + convert_code(RPL_NOTOPIC) + base + "No topic is set";
    client->print(base);
}

void                Channel::printClients(Client *target) const
{
    std::string     res = ":ircserv " + convert_code(RPL_NAMREPLY) + " " + target->getNickname() + " = " + name + " :";
   
    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
    {
        res += /*client prefix + */ (*it)->getNickname() + " ";
    }
    target->print(res);
    res = ":ircserv " + convert_code(RPL_ENDOFNAMES) + " " + target->getNickname() + " " + name + " :End of NAMES list";
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

Channel::Channel(Ircserv*   serv, const std::string& name):
                name(name),
                serv(serv)
{
    (void)this->serv;
}

Channel::~Channel()
{
    std::cout << "Channel destructor called\n";
}