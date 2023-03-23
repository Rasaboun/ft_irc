# ft_irc
IRC Server in C++


[irc.webm](https://user-images.githubusercontent.com/40967731/227237055-0d00937f-d1e8-4b76-921b-32038f3042da.webm)


# Usage
You can by running `make` command and then start the server with `./ircserv [port] [password]` .
Then you can connect using an IRC client, such as [irssi](https://irssi.org/), which the server has been developped for.

# Features
This project has been made following this [documentation](https://modern.ircdocs.horse/). However, it does not implements every single features, such as multi server or some commands.

### Commands
These are all the commands implemented in the server.

``CAP``
``PASS`` 
``NICK`` 
``USER`` 
``OPER`` 
``MODE`` 
``QUIT``
``MSG`` 
``JOIN`` 
``PART`` 
``PING`` 
``PONG`` 
``TOPIC`` 
``LIST`` 
``INVITE`` 
``KICK`` 
``WHO`` 
``KILL`` 
``WALLOPS``
``NOTICE`` 
``WHOIS``

### Channels
You can join/create a channel using ``JOIN`` command. We implemented channel modes aswell. Inside a channel, use ``MODE +[modes]`` to enable some modes or ``MODE -[modes]`` to disable.

Supported channel modes:

``k`` adds a password to a channel

``i`` channel is in invite only mode

``l`` limits the number of members

``m`` only authorized members of channel are allowed to talk

``n`` the channel can not receive messages from external users

``s`` channel is hidden from non-member users

``t`` only channel administrators can edit the topic

``b`` target user is banned from the channel

``o`` target user becomes a channel administrator


### User modes
Users can change their mode using ``MODE +[modes]``.

Supported user modes:

``o`` user becomes server administrator (password is required) 

``i`` user is hidden from other users

``o`` user receives ``WALLOPS`` messages

# Example
![](https://github.com/bditte/ft_irc/blob/master/IRC.gif)
