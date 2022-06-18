#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"
# include "Command.hpp"
# include "Message.hpp"

class Ircserv;

class Channel
{
	public:
	
    Channel(Ircserv* serv, const std::string& name);

    const std::string&              getName() const;
    const std::string&              getTopic() const;
    const std::vector<Client *>&    getClients() const;
    int                             getNbClients() const;

    void                            addClient(Client *);

    private:

    std::string                 name;
    std::string                 topic;

    std::vector<Client *>       clients;
    Ircserv*                    serv;
    
};

#endif