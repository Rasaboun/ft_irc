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
    const std::string&              getKey() const;
    bool                            getMode(char) const;
    const std::string               getModes() const;
    const std::vector<Client *>&    getClients() const;
    int                             getNbClients() const;
    int                             getLimit() const;

    void                            setTopic(const std::string&);
    void                            editTopic(Client*, const std::string&);
    int                             setMode(char, const std::string& param = std::string());
    int                             unsetMode(char, const std::string& param = std::string());
    void                            changeModes(Client *, std::vector<std::string>);
    
    void                            addClient(Client *, const std::string& key = std::string());
    int                             addOperator(Client *);
    void                            addBan(Client *);
    void                            kickClient(Client *, Client *, const std::string&);
    void                            removeClient(Client *, const std::string&);
    void                            removeInvite(Client *);
    int                             removeOperator(Client *);
    void                            removeBan(Client *);

    void                            sendToClients(const std::string&) const;
    void                            sendTopic(Client *) const;
    void                            printClients(Client *) const;
    void                            printInfos(Client * ) const;
    int                             isClient(Client *) const;
    bool                            isOperator(Client *) const;
    bool                            isInvited(Client *) const;
    bool                            isBanned(Client *) const;

    void                            invite(Client*, const std::string&);
    void                            printWho(Client *) const;
    
    private:

    std::string                     name;
    std::string                     topic;
    std::string                     key;
    std::string                     topic_editor;
    std::string                     topic_time;
    int                             limit;

    std::vector<Client *>           clients;
    std::vector<Client *>           operators;
    std::vector<Client *>           invites;
    std::vector<Client *>           banned;
    Ircserv*                        serv;
	std::map<const char, bool>		modes;
    
};

#endif