# IRC-Server
Internet Relay Chat or IRC is a text-based communication protocol on the Internet. It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels. IRC clients connect to IRC servers in order to join channels. IRC servers are connected together to form a network.

## Introduction
This IRC Server is a simple and lightweight IRC server written in C++98. It is designed to be easy to set up and use.

## Usage
To run the IRC server, use the following command:  </br>
``` ./ircserv <port> <password> ``` </br>
**port**: The port number on which the IRC server will listen for incoming connections. </br>
**password**: The connection password required for clients to connect to the server.</br>

## Features
The IRC server provides the following features:

- **Authentication**: Clients need to provide the correct password to connect to the server.
- **Nicknames**: Clients can set their nickname to identify themselves in the chat.
- **Usernames**: Clients can set their username, which is displayed along with their nickname.
- **Channel Joining**: Clients can join channels to participate in group conversations.
- **Private Messages**: Clients can send private messages to other connected users.
- **Channel Messages**: Messages sent by one client to a channel are forwarded to all other clients in the channel.
- **Channel Operators**: Channels have operators who have additional privileges.
- **Part**: Clients can leave a channel using the ```/part``` command.
- **Kick**: Channel operators can eject a client from a channel using the ```/kick``` command.
- **Ban**: Channel operators can ban a user from a channel using the ```/ban``` command.
- **Unban**: Channel operators can remove a ban on a user using the ```/unban``` command.
- **Notice**: Clients can send notices to other connected users or channels.
- **Invite**: Channel operators can invite a client to a channel using the ```/invite``` command.
- **Topic**: Channel operators can change or view the topic of a channel using the ```/topic``` command.
- **Channel Modes**: Channel operators can change the mode of a channel using the ```/mode``` command. Supported modes include:
  + '**__i__**': Set/remove invite-only channel.
  + '**__t__**': Set/remove restrictions of the TOPIC command to channel operators.
  + '**__k__**': Set/remove the channel key (password).
  + '**__o__**': Give/take channel operator privilege.
  + '**__l__**': Set/remove the user limit to the channel.
  + '**__b__**': Set/remove ban list from the channel.

## Compatibility
The IRC server is compatible with IRC clients that support standard IRC commands and protocol. It provides a similar experience to using any official IRC server. Also compatible with netcat.

## Examples
- Join a channel: ```/join mychannel``` in irssi, ```JOIN #mychannel``` in netcat
- Send a message to a user: ```/msg username Hello, how are you?``` in irssi, ```PRIVMSG username :Hello, how are you?``` in netcat
- Send a message to a channel: ```/msg #mychannel Hello, everyone!``` in irssi, ```PRIVMSG #mychannel :Hello, how are you?``` in netcat
- Quit the channel: ```/part mychannel See you next time !``` in irssi, ```PART #mychannel :See you next time !```  in netcat

## Limitations
Please note the following limitations of the IRC server:

- The server does not support server-to-server communication.
- Only a subset of IRC commands is implemented.
