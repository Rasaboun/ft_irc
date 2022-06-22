#include "Channel.hpp"
#include "Ircserv.hpp"

const std::string&  Channel::getName() const { return (this->name); }
const std::string&  Channel::getTopic() const { return (this->topic); }
const std::string&  Channel::getKey() const { return (this->key); }
bool                Channel::getMode(char mode) const 
{ 
    std::map<const char, bool>::const_iterator it = this->modes.find(mode);

	if (it == this->modes.end())
		return (false);
	return (it->second); 
}
const std::vector<Client *>&    Channel::getClients() const { return (this->clients); }
int                 Channel::getNbClients() const { return (this->clients.size()); }

void                Channel::addClient(Client* client, const std::string& key)
{

    if (modes[KEY] && key != this->key)
    {
        client->print(reply_prefix(serv->getName(), ERR_BADCHANNELKEY, client->getNickname()) + name + ":Cannot join channel (+k)");
    }
       
    //Send join to all channel's clients
    client->addChannel(name, VOICE);
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
            client->removeChannel(name);
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

bool                Channel::isOperator(Client * client) const
{
    char perm = client->getChanPerm(name);
    std::cout << perm << std::endl;
    if (perm == OPERATOR || perm == PROTECTED || perm == FOUNDER)
        return (true);
    return (false);
}

Channel::Channel(Ircserv*   serv, const std::string& name):
                name(name),
                serv(serv)
{
    modes['i'] = false;
	modes['k'] = false;
	modes['l'] = false;
	modes['m'] = false;
	modes['n'] = false;
	modes['s'] = false;
	modes['t'] = true;
}

Channel::~Channel()
{

}