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
    topic_editor = editor->getNickname();
    std::time_t curr_time = std::time(NULL);
    topic_time = convert_time(&curr_time);
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
    //std::string     base = " " + name + " " + name + " :";
    std::string     res;
    if (topic.length())
    {
        res = reply_prefix(serv->getName(), RPL_TOPIC, name) + name + " " + topic;
        client->print(res);
        res = reply_prefix(serv->getName(), RPL_TOPICWHOTIME, name) + name + \
                + " " + topic_editor + " " + topic_time;
    }
    else
        res = reply_prefix(serv->getName(), RPL_NOTOPIC, name) + name + " No topic is set";
    client->print(res);

}

void                Channel::printClients(Client *target) const
{
    std::string     res = reply_prefix(serv->getName(), RPL_NAMREPLY, target->getNickname()) + " = " + name + " :";
   
    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
    {
        res += /*client prefix + */ (*it)->getNickname() + " ";
    }
    target->print(res);
    res = reply_prefix(serv->getName(), RPL_ENDOFNAMES, target->getNickname()) + name + " :End of NAMES list";
    target->print(res);
}

void                Channel::printInfos(Client *client) const
{
    std::string res = reply_prefix(serv->getName(), RPL_LIST, client->getNickname());
    client->print(res + name + " " + ft_itoa(clients.size()) + " :" + topic);
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

}