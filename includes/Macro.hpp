/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 17:04:51 by root              #+#    #+#             */
/*   Updated: 2023/05/23 16:36:25 by chaidel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// HAND SHAKE
#define RPL_WELCOME(user) (":" + user.getHostname() + " 001 " + user.getNickname() + " Welcome to the Internet Relay Network " + user.getNames() + "\n")
#define RPL_YOURHOST(user) (":" + user.getHostname() + " 002 " + user.getNickname() + " Your host is " + Server::running_serv->getHostname() + ", running version 4.2\n")
#define RPL_CREATED(user) (":" + user.getHostname() + " 003 " + user.getNickname() + " This server was created " + "2023" + "\n")
#define RPL_MYINFO(user) (":" + user.getHostname() + " 004 " + user.getNickname() + Server::running_serv->getHostname() + " v4.2 i ioblkt\n")

// PING
#define PONG(user) (":" + user.getHostname() + " PONG :" + user.getNickname() + "\r\n")
#define ERR_NOORIGIN() ": No origin specified\n"

// PASS
#define ERR_PASSWDMISMATCH(user) (":" + user.getNames() + " 464 " + user.getNickname() + " :" + user.getHostname() + " PASSWORD MISSMATCH\n")

// :<serveur> <code-réponse> <pseudo-canal> <pseudo-utilisateur> <hôte> <serveur-origine> <pseudo> <mode> <vrai-nom>
#define	RPL_WHOREPLY(user) (":" + user.getNames() + "352" +  + "\r\n")

// NICK
#define NICK(user, nick) (":" + user.getNames() + " NICK " + nick + "\r\n")
#define ERR_NICKNAMEINUSE(user, nick) (":" + user.getHostname() + " 433 " + user.getNickname() + " " + nick + " :Nickname is already in use.\r\n")
#define ERR_ERRONEUSNICKNAME(user) (":" + user.getHostname() + " 432 " + user.getNickname() + " :Erroneous nickname\r\n")
#define RPL_WHOISUSER(requester, target) (":" + (requester).getNames() + " 311 " + (requester).getNickname() + " " + (target).getNickname() + " " + (target).getNames() + " " + (target).getHostname() + " * :" + (target)->getRealName() + "\r\n")
#define RPL_WHOISSERVER(requester, target) (":" + (requester).getNames() + " 312 " + (requester).getNickname() + " " + (target).getNickname() + " " + (requester).getNames() + " :This server\r\n")
#define RPL_ENDOFWHOIS(requester, target) (":" + (requester).getNames() + " 318 " + (requester).getNickname() + " " + (target).getNickname() + " :End of /WHOIS list.\r\n")
#define ERR_NOSUCHNICK(requester, nick) (":" + (requester).getNames() + " 401 " + (requester).getNickname() + " " + (nick) + " :No such nick/channel\r\n")                 
#define ERR_NONICKNAMEGIVEN(requester) (":" + (requester).getNames() + " 431 " + (requester).getNickname() + " :No nickname given\r\n")

//MODE
#define ERR_CHANOPRIVSNEEDED(user, channel) (":" + user.getNames() + " 482 " + user.getNickname() + " " + (channel) + " :You're not channel operator\r\n")
#define ERR_USERSDONTMATCH(user) (":" + user.getNames() + " 502 " + user.getNickname() + " :Cannot change mode for other users\r\n")
// :server_name 221 nick_name +iowBx (modes utilisateur)
#define RPL_UMODEIS(server, user, mode) (":" + (server) + " 221 " + user.getNickname() + " " + (mode) + "\r\n")
// 467    ERR_KEYSET "<channel> :Channel key already set"
#define	ERR_KEYSET(user, channel) (":" + user.getNames() + " 467 " + (channel) + " :Channel key already set" + "\r\n")


// « nick@user!hostname MODE target [mode] »
#define USERMODE(user, mode) (":" + user.getNickname() + " MODE " + user.getNickname() + " :" + (mode) + "\r\n")
// Broadcast to chan member
#define CHANMODE(channel, mode) (":localhost MODE " + (channel) + " " + (mode) + "\r\n")
#define RPL_CHANNELMODEIS(user, channel, mode) (":localhost 324 " + user.getNickname() + " " + (channel) + " " + (mode) + "\r\n")

// PRIVMSG
#define PRIVMSGCHAN(sender, recv, word) (": " + user.getNames() + " PRIVMSG " + recv.getNickname() + " :" + word + "\r\n")
#define ERR_NORECIPIENT(user, command) (":" + user.getNames() + " 411 " + user.getNickname() + " :No recipient given " + (command) + "\r\n")
#define ERR_CANNOTSENDTOCHAN(user, channel) (":" + user.getNames() + " 404 " + user.getNickname() + " " + (channel) + ":Cannot send to channel" + "\r\n")
#define ERR_NOTEXTTOSEND(user) (":localhost 412 " + user.getNickname() + ":No text to send" + "\r\n")

// UNKNOWN
#define ERR_NOTIMPLEMENTED(word) (":" + user.getHostname() + " 449 : " + word + " command not implemented\n")
               
// CHAN
#define ERR_NOSUCHCHANNEL(user, channel) (":" + user.getNames() + " 403 " + user.getNickname() + " " + channel + " :No such channel\r\n")
#define ERR_NOUSERONCHANNEL(user, channel, target) (":" + user.getNames() + " 441 " + user.getNickname() + " " + (target) + " " + (channel) + " :No such user in channel\n")
#define ERR_USERONCHANNEL(user, nick, channel) ( ":" + user.getHostname() + " 443 " + user.getNickname() + " " + nick + " " + channel + " :is already on channel\n" )
#define ERR_NOTONCHANNEL(user, channel) ( ":" + user.getHostname() + " 442 " + user.getNickname() + " " + channel + " :You're not on that channel" + "\n" )
#define ERR_BANNEDFROMCHAN(user, channel) ( ":" + user.getHostname() + " 474 " + user.getNickname() + " " + (channel)->getName() + " :Cannot join " + (channel)->getName() + " (+b)\r\n" )
#define RPL_UNBANUSER(user, channel, operator) (":" + user.getNames() + " MODE " + channel->getName() + " -b " + user.getNickname() + " : You have been unbanned from " + channel->getName() + " by operator : "+ operator.getNickname() + " \r\n")

// JOIN
// 475    ERR_BADCHANNELKEY		"<channel> :Cannot join channel (+k)"
#define ERR_BADCHANNELKEY(user, channel) ("475 " + user.getNickname() + " " + (channel) + " :Cannot join channel (+k)" + "\r\n")
// 471    ERR_CHANNELISFULL		"<channel> :Cannot join channel (+l)"
#define ERR_CHANNELISFULL(user, channel) ("471 " + user.getNickname() + " " + (channel) + " :Cannot join channel (+l)" + "\r\n")

// INVITE
// #define	RPL_INVITING(user, channel, new_user) ( ":" + user.getUsername() + " 341 " + user.getNickname() + " has invited on " + channel + " " + new_user.getNickname() + "\n")
#define RPL_INVITING(user, invited, channel) (":" + user.getUsername() + " 341 " + user.getNickname() + " " + invited.getNickname() + " " + (channel) + "\r\n")
#define RPL_INVITE(user, invited, channel) (":" + user.getNames() + " INVITE " + invited.getNickname() + " " + (channel) + "\r\n")
#define ERR_NEEDINVITE(user, channel) ( ":" + user.getHostname() + " 473 " + user.getNickname() + " " + (channel) + " :Cannot join " + (channel) + " (+i)\r\n" )

// KICK
#define RPL_KICK(user, channel, target, reason) (":" + user.getNames() + " KICK " + (channel) + " " + target.getNickname() + " :" + (reason) + "\r\n")

//LIST
#define RPL_LIST(user, channel) (": " + user.getNames() + " 322 " + user.getNickname() + " " + channel->getName() + " " + channel->getSNumUsers() + " " + channel->getTopic() + "\n")
#define RPL_LISTEND(user) (": " + user.getNames() + " 323 " + user.getNickname() + " :End of LIST\n")

//TOPIC
#define RPL_TOPIC(user, channel, topic) (":localhost 332 " + user.getNickname() + " " + (channel) + " :" + (topic) + "\n")
#define RPL_NOTOPIC(user, channel) (":localhost 331 " + user.getNickname() + " " + (channel) + " :No topic is set" + "\r\n")

#define ERR_NEEDMOREPARAMS(user, command) (":" + user.getNames() + " 461 " + user.getNickname() + " " + (command) + " :Not enough parameters\r\n")
