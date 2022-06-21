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
    ~Channel();

    const std::string&              getName() const;
    const std::string&              getTopic() const;
    const std::vector<Client *>&    getClients() const;
    int                             getNbClients() const;

    void                            setTopic(const std::string&);
    void                            editTopic(Client*, const std::string&);

    void                            addClient(Client *);
    void                            removeClient(Client *, const std::string&);

    void                            sendToClients(const std::string&) const;
    void                            sendTopic(Client *) const;
    void                            printClients(Client *) const;
    void                            printInfos(Client * ) const;
    int                             isClient(Client *) const;

    private:

    std::string                 name;
    std::string                 topic;
    std::string                 topic_editor;
    std::string                 topic_time;

    std::vector<Client *>       clients;
    Ircserv*                    serv;
    
};

#endif