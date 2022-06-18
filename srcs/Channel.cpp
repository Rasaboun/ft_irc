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
    //Need to add Welcoming messages
    this->clients.push_back(client); 

}
