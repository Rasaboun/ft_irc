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

const std::string		Channel::getModes() const
{
	std::string res;
	std::string params;

	for(std::map<const char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
	{
		if (it->second)
        {
            res += it->first;
            if (it->first == LIMIT)
                params += " " + ft_itoa(limit);
            if (it->first == KEY)
                params += " " + key;
            
        }
    }    
	return (res + params);
}

const std::vector<Client *>&    Channel::getClients() const { return (this->clients); }
int                 Channel::getNbClients() const { return (this->clients.size()); }
int                 Channel::getLimit() const { return (this->limit); }

void                Channel::addClient(Client* client, const std::string& key)
{

    if (modes[KEY] && key != this->key)
    {
        client->print(reply_prefix(serv->getName(), ERR_BADCHANNELKEY, client->getNickname()) + name + " :Cannot join channel (+k)");
        return ;
    }
    if (modes[INVITE] && !isInvited(client))
    {
        client->print(reply_prefix(serv->getName(), ERR_INVITEONLYCHAN, client->getNickname()) + name + " :Cannot join channel (+i)");
        return ;
    }   
    //Send join to all channel's clients
    client->addChannel(name);
    clients.push_back(client);
    sendToClients(":" + client->getFullname() + " JOIN " + name);

    if (topic.length())
        sendTopic(client);

    printClients(client);
    
}

void                Channel::addOperator(Client* client) { operators.push_back(client); }
void                Channel::addBan(Client* client) { banned.push_back(client); }

void                Channel::setTopic(const std::string& topic){ this->topic = topic; } 

void                Channel::editTopic(Client* editor, const std::string& topic)
{
    setTopic(topic);
    topic_editor = editor->getNickname();
    std::time_t curr_time = std::time(NULL);
    topic_time = convert_time(&curr_time);
    std::cout << "topic time " << topic_time << std::endl;
    sendToClients(":" + editor->getNickname() +  " TOPIC " + name + " " + topic);
}


void                Channel::setMode(char mode, const std::string& param)
{
    if (mode == BAN)
    {
        addBan(serv->getClient(param));
        return ;
    }
    modes[mode] = true; 
    if (mode == LIMIT)
        this->limit = atoi(param.c_str());
    if (mode == KEY)
    {
        this->key = param;
    }
}
void                Channel::unsetMode(char mode, const std::string& param)
{
    if (mode == BAN)
    {
        removeBan(serv->getClient(param));
        return ;
    } 
    modes[mode] = false;  
    if (mode == LIMIT)
        this->limit = MAX_INT;
    if (mode == KEY)
        this->key = "";
}
void                Channel::changeModes(Client *client, std::vector<std::string> params)
{
    std::string modestring = params[1];
    std::string res = modestring[0] == '-' ? "-" : "+";
    std::string new_params;

    params.erase(params.begin(), params.begin() + 2);
    std::vector<std::string>::iterator  args = params.begin();
    for (size_t i = 0; i < modestring.length(); i++)
    {
        if (!is_chan_mode(modestring[i]))
            continue ;
        if (modestring[0] == '-')
        {
            if (modestring[i] == BAN) 
            {
                if (args != params.end())
                {
                    unsetMode(modestring[i], *args);
                    new_params += " " + *args;
                    res += modestring[i];
                    args++;
                }
                continue ;
            }
            unsetMode(modestring[i]);
        }
        else
        {
            if (is_param_mode(modestring[i])) 
            {
                if (args != params.end())
                {
                    setMode(modestring[i], *args);
                    new_params += " " + *args;
                    res += modestring[i];
                    args++;
                }
                continue ;
            }
            setMode(modestring[i]);
        }
        res += modestring[i];
    }
    if (res.length() > 1)
        sendToClients(":" + client->getFullname() + " MODE " + name + " " + res + new_params);

}

void                Channel::kickClient(Client *client, Client *target, const std::string& message)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (*it == target)
        {   
            sendToClients(":" + client->getFullname()+ " KICK " + name + " " + target->getNickname() \
                            + " " + message);
            clients.erase(it);
            target->removeChannel(name);
            break ;
        }
    }
    removeOperator(target);
    removeInvite(target);
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
    removeOperator(client);
    removeInvite(client);
}

void                Channel::removeOperator(Client *client)
{
    for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
    {
        if (*it == client)
        {
            operators.erase(it);
            break ;
        }
    }
}

void                Channel::removeInvite(Client *client)
{
    for (std::vector<Client *>::iterator it = invites.begin(); it != invites.end(); it++)
    {
        if (*it == client)
        {
            invites.erase(it);
            break ;
        }
    }
}

void                Channel::removeBan(Client *client)
{
    for (std::vector<Client *>::iterator it = banned.begin(); it != banned.end(); it++)
    {
        if (*it == client)
        {
            banned.erase(it);
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
        res = reply_prefix(serv->getName(), RPL_TOPIC, client->getFullname()) + name + " " + topic;
        client->print(res);
        res = reply_prefix(serv->getName(), RPL_TOPICWHOTIME, client->getFullname()) + name + \
                + " " + topic_editor + " :" + topic_time;
    }
    else
        res = reply_prefix(serv->getName(), RPL_NOTOPIC, client->getFullname()) + name + " No topic is set";
    client->print(res);

}

void                Channel::printClients(Client *target) const
{
    std::string     res = reply_prefix(serv->getName(), RPL_NAMREPLY, target->getNickname()) + "= " + name + " :";
   
    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (isOperator(*it))
            res += "@";
        res += (*it)->getNickname() + " ";
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

bool                Channel::isOperator(Client *client) const
{
    for (std::vector<Client *>::const_iterator it = operators.begin(); it != operators.end(); it++)
    {
        if (*it == client)
            return (true);
    }
    return (false);
}

bool                Channel::isInvited(Client *client) const
{
    for (std::vector<Client *>::const_iterator it = invites.begin(); it != invites.end(); it++)
    {
        if (*it == client)
            return (true);
    }
    return (false);
}

bool                Channel::isBanned(Client *client) const
{
    for (std::vector<Client *>::const_iterator it = banned.begin(); it != banned.end(); it++)
    {
        if (*it == client)
            return (true);
    }
    return (false);
}

void                Channel::invite(Client *client, const std::string& name)
{
    Client* target = serv->getClient(name);
    
    if (!target)
        return ;
    invites.push_back(target);
    target->print(":" + client->getNickname() + " INVITE " + name + " " + this->name);

}


Channel::Channel(Ircserv*   serv, const std::string& name):
                name(name),
                limit(MAX_INT),
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