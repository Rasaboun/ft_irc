<h1 align="center">
	🌐 ft_irc
 42cursus project 🌐
</h1>

<div align="center">
<video src="https://user-images.githubusercontent.com/40967731/227237055-0d00937f-d1e8-4b76-921b-32038f3042da.webm"/>
</div>

<h3 align="center">
  IRC server in C++ 98
</h3>

<h3 align="center">
	<a href="#about">About</a>
	<span> · </span>
	<a href="#projetstructure"> Project Structure </a>
	<span> · </span>
	<a href="#features">Features</a>
	<span> · </span>
	<a href="#usage">Usage</a>
</h3>



<h2 id="about">
About
</h2>

> The goal of the project is to develop an **IRC** server in C++ 98

<h2 id="usage">
🔨 Usage
</h2>

```shell
Make && ./ircserv [Port] [Password]
```
Then you can connect using an IRC client, such as [irssi](https://irssi.org/), which the server has been developped for.

![](https://github.com/bditte/ft_irc/blob/master/IRC.gif)

<h2 id="projetstructure">
Project Structure
</h2>

```
.
├── 📁include/
│   ├── Client.hpp
│   ├── Channel.hpp
│   ├── Command.hpp
│   ├── Ircserver.hpp
│   ├── Message.hpp
│   └── ft_irc.hpp
├── 📁srcs/
│   └── *.cpp
└── Makefile
```


* [**📂 include:**](include/) All class declaration
* [**📂 src:**](src/) All class definition

<h2 id="features">
📖 Features
</h2>

This project has been made following this [documentation](https://modern.ircdocs.horse/). However, it does not implements every single features, such as multi server or some commands.

<h3>
Command
</h3>

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

<h3>
Channels
</h3>

You can join/create a channel using ``JOIN`` command. We implemented channel modes aswell. Inside a channel, use ``MODE +[modes]`` to enable some modes or ``MODE -[modes]`` to disable.

Supported channel modes:

- ``k`` adds a password to a channel
- ``i`` channel is in invite only mode
- ``l`` limits the number of members
- ``m`` only authorized members of channel are allowed to talk
- ``n`` the channel can not receive messages from external users
- ``s`` channel is hidden from non-member users
- ``t`` only channel administrators can edit the topic
- ``b`` target user is banned from the channel
- ``o`` target user becomes a channel administrator

<h3>
User modes
</h3>

Users can change their mode using ``MODE +[modes]``.

Supported user modes:

- ``o`` user becomes server administrator (password is required) 
-  ``i`` user is hidden from other users
- ``o`` user receives ``WALLOPS`` messages
