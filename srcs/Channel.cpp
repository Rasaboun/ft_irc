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
	print_log("Client " + client->getNickname() + " joining channel " + name);
    sendToClients(":" + client->getFullname() + " JOIN " + name);
    
    
    if (topic.length())
        sendTopic(client);

    printClients(client);
    if (client->isOperator())
        sendToClients(":" + client->getFullname() + " MODE " + name + " +o " + client->getNickname() );
    
}

int                Channel::addOperator(Client* client) 
{
    if (!client)
        return (1);
    operators.push_back(client);
    return (0);
}
int               Channel::addBan(Client* client) 
{
    if (!client)
        return (1);
    banned.push_back(client);
    return (0);
}

void                Channel::setTopic(const std::string& topic){ this->topic = topic; } 

void                Channel::editTopic(Client* editor, const std::string& topic)
{
    setTopic(topic);
    topic_editor = editor->getNickname();
    sendToClients(":" + editor->getFullname() +  " TOPIC " + name + " " + topic);
}


int               Channel::setMode(char mode, const std::string& param)
{
    if (mode == BAN)
        return (addBan(serv->getClient(param)));
    if (mode == CHANOP)
    {
        return (addOperator(serv->getClient(param)));
    }
    modes[mode] = true; 
    if (mode == LIMIT)
        this->limit = atoi(param.c_str());
    if (mode == KEY)
    {
        this->key = param;
    }
    return (0);
}

int               Channel::unsetMode(char mode, const std::string& param)
{
    if (mode == BAN)
        return (removeBan(serv->getClient(param)));  
    if (mode == CHANOP)
        return (removeOperator(serv->getClient(param)));
    modes[mode] = false;  
    if (mode == LIMIT)
        this->limit = MAX_INT;
    if (mode == KEY)
        this->key = "";
    return (0);
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
            if (modestring[i] == BAN || modestring[i] == CHANOP) 
            {
                if (args != params.end())
                {
                    if (unsetMode(modestring[i], *args))
                        continue ;
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
                    if (setMode(modestring[i], *args))
                        continue ;
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
	        print_log("Client " + client->getNickname() + " leaving channel " + name);
            break ;
        }
    }
    removeOperator(client);
    removeInvite(client);
}

int               Channel::removeOperator(Client *client)
{
    if (!client)
        return (1);
    for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
    {
        if (*it == client)
        {
            operators.erase(it);
            break ;
        }
    }
    return (0);
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

int                   Channel::removeBan(Client *client)
{
    if (!client)
        return (1);
    for (std::vector<Client *>::iterator it = banned.begin(); it != banned.end(); it++)
    {
        if (*it == client)
        {
            banned.erase(it);
            break ;
        }
    }
    return (0);
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
        res = reply_prefix(serv->getName(), RPL_TOPIC, client->getFullname()) + name + " " + topic;
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

void                 Channel::printBanList(Client *target) const
{
    std::string base = reply_prefix(serv->getName(), RPL_BANLIST, target->getNickname()) + name + " ";

    for (std::vector<Client *>::const_iterator it = banned.begin(); it != banned.end(); it++)
    {
        target->print(base + (*it)->getNickname());
    }
    target->print(reply_prefix(serv->getName(), RPL_ENDOFBANLIST, target->getNickname()) + name + " :End of channel ban list");
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

void                Channel::printWho(Client* target) const
{
    std::string base = reply_prefix(serv->getName(), RPL_WHOREPLY, target->getNickname()) + name +  " ";

    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
    {
		if (!(*it)->getMode(INVISIBLE) || ((*it)->getMode(INVISIBLE) && !sharing_channel(target, *it)))
        {
            if (isOperator(*it))
                base += "@";
            else
                base += "~";
            target->print(base + (*it)->getUsername() + " " + (*it)->getHostname() + " " + serv->getName()\
                        + " " + (*it)->getNickname() + " H :0 " + (*it)->getRealname().substr(1));
        }
    }
}

Channel::Channel(Ircserv*   serv, const std::string& name):
                name(name),
                limit(MAX_INT),
                serv(serv)
{
	print_log("Creating new channel " + name);
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
    print_log("Destroying channel " + name);
}