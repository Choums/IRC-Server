/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 17:04:51 by root              #+#    #+#             */
/*   Updated: 2023/05/08 11:56:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// HAND SHAKE
#define RPL_WELCOME(user) (":" + user.getHostname() + " 001 " + user.getNickname() + " Welcome to the Internet Relay Network " + user.getUsername() + "\n")
#define RPL_YOURHOST(user) (":" + user.getHostname() + " 002 " + user.getNickname() + " Your host is " + Server::running_serv->getHostname() + ", running version 4.2\n")
#define RPL_CREATED(user) (":" + user.getHostname() + " 003 " + user.getNickname() + " This server was created " + "2023" + "\n")
#define RPL_MYINFO(user) (":" + user.getHostname() + " 004 " + user.getNickname() + Server::running_serv->getHostname() + " v4.2 oi iob\n")

// PING
#define PONG(user) (":" + user.getHostname() + " PONG :" + user.getNickname() + "\r\n")
#define ERR_NOORIGIN() ": No origin specified\n"

// PASS
#define ERR_PASSWDMISMATCH(user) (":" + user.getNickname() + " 464 " + user.getNickname() + " :" + user.getHostname() + " PASSWORD MISSMATCH\n")


// NICK
#define NICK(user, nick) (":" + user.getUsername() + " NICK " + nick + "\r\n")
#define ERR_NICKNAMEINUSE(user, nick) (":" + user.getHostname() + " 433 " + user.getNickname() + " " + nick + " :Nickname is already in use.\r\n")
#define ERR_ERRONEUSNICKNAME(user) (":" + user.getHostname() + " 432 " + user.getNickname() + " :Erroneous nickname\r\n")
#define RPL_WHOISUSER(requester, target) (":" + (requester).getUsername() + " 311 " + (requester).getNickname() + " " + (target).getNickname() + " " + (target).getUsername() + " " + (target).getHostname() + " * :" + (target)->getRealName() + "\r\n")
#define RPL_WHOISSERVER(requester, target) (":" + (requester).getUsername() + " 312 " + (requester).getNickname() + " " + (target).getNickname() + " " + (requester).getUsername() + " :This server\r\n")
#define RPL_ENDOFWHOIS(requester, target) (":" + (requester).getUsername() + " 318 " + (requester).getNickname() + " " + (target).getNickname() + " :End of /WHOIS list.\r\n")
#define ERR_NOSUCHNICK(requester, nick) (":" + (requester).getUsername() + " 401 " + (requester).getNickname() + " " + (nick) + " :No such nick/channel\r\n")                 
#define ERR_NONICKNAMEGIVEN(requester) (":" + (requester).getUsername() + " 431 " + (requester).getNickname() + " :No nickname given\r\n")

//MODE
#define ERR_CHANOPRIVSNEEDED(user, channel) (":" + user.getUsername() + " 482 " + user.getNickname() + " " + (channel) + " :You're not channel operator\r\n")
#define ERR_USERSDONTMATCH(user) (":" + user.getUsername() + " 502 " + user.getNickname() + " :Cannot change mode for other users\r\n")
// :server_name 221 nick_name +iowBx (modes utilisateur)
#define RPL_UMODEIS(server, user, mode) (":" + (server) + " 221 " + user.getNickname() + " " + (mode) + "\r\n")

// « nick@user!hostname MODE target [mode] »
#define USERMODE(user, mode) (":" + user.getNickname() + " MODE " + user.getNickname() + " :" + (mode) + "\r\n")
// Broadcast to chan member
#define CHANMODE(channel, mode) (":localhost MODE " + (channel) + " " + (mode) + "\r\n")
#define RPL_CHANNELMODEIS(user, channel, mode) (":localhost 324 " + user.getNickname() + " " + (channel) + " " + (mode) + "\r\n")

// PRIVMSG
#define PRIVMSGCHAN(sender, recv, word) (": " + user.getUsername() + " PRIVMSG " + recv.getNickname() + " :" + word + "\r\n")
#define ERR_NORECIPIENT(user, command) (":" + user.getUsername() + " 411 " + user.getNickname() + " :No recipient given " + (command) + "\r\n")
#define ERR_CANNOTSENDTOCHAN(user, channel) (":" + user.getUsername() + " 404 " + user.getNickname() + " " + (channel) + ":Cannot send to channel" + "\r\n")
#define ERR_NOTEXTTOSEND(user) (":localhost 412 " + user.getNickname() + ":No text to send" + "\r\n")

// UNKNOWN
#define ERR_NOTIMPLEMENTED(word) (":" + user.getHostname() + " 449 : " + word + " command not implemented\n")
               
// CHAN
#define ERR_NOSUCHCHANNEL(user, channel) (":" + user.getUsername() + " 403 " + user.getNickname() + " " + channel + " :No such channel\r\n")
#define ERR_NOUSERONCHANNEL(user, channel, target) (":" + user.getUsername() + " 441 " + user.getNickname() + " " + (target) + " " + (channel) + " :No such user in channel\n")
#define ERR_USERONCHANNEL(user, nick, channel) ( ":" + user.getHostname() + " 443 " + user.getNickname() + " " + nick + " " + channel + " :is already on channel\n" )
#define ERR_NOTONCHANNEL(user, channel) ( ":" + user.getHostname() + " 442 " + user.getNickname() + " " + channel + " :You're not on that channel" + "\n" )
// #define RPL_ADDEDCHANOPER(user, channel, operator) (":" + user.getUsername() + " 482 " + user.getNickname() + " " + (channel) + " :You have been added as a channel operator by operator : "+ operator.getNickname() + " \n")
// #define RPL_REMOVEDCHANOPER(user, channel, operator) (":" + user.getUsername() + " 482 " + user.getNickname() + " " + (channel) + " :You have been removed as a channel operator by operator : "+ operator.getNickname() + " \n")
#define ERR_BANNEDFROMCHAN(user, channel) ( ":" + user.getHostname() + " 474 " + user.getNickname() + " " + (channel)->getName() + " :Cannot join " + (channel)->getName() + " (+b)\r\n" )
#define RPL_UNBANUSER(user, channel, operator) (":" + user.getUsername() + " MODE " + channel->getName() + " -b " + user.getNickname() + " : You have been unbanned from " + channel->getName() + " by operator : "+ operator.getNickname() + " \r\n")

// INVITE
// #define	RPL_INVITING(user, channel, new_user) ( ":" + user.getUsername() + " 341 " + user.getNickname() + " has invited on " + channel + " " + new_user.getNickname() + "\n")
#define RPL_INVITING(user, invited, channel) (":" + user.getUsername() + " 341 " + user.getNickname() + " " + invited.getNickname() + " " + (channel) + "\r\n")
#define RPL_INVITE(user, invited, channel) (":" + user.getUsername() + " INVITE " + invited.getNickname() + " " + (channel) + "\r\n")
#define ERR_NEEDINVITE(user, channel) ( ":" + user.getHostname() + " 473 " + user.getNickname() + " " + (channel) + " :Cannot join " + (channel) + " (+i)\r\n" )

// KICK
# define RPL_KICK(user, channel, target, reason) (":" + user.getNickname() + " KICK " + (channel) + " " + target.getNickname() + " " + (reason) + "\r\n")

//LIST
#define RPL_LIST(user, channel, topic) (": " + user.getUsername() + " 322 " + user.getNickname() + " " + channel->getName() + " " + channel->getSNumUsers() + " " + (topic) + "\n")
#define RPL_LISTEND(user) (": " + user.getUsername() + " 323 " + user.getNickname() + " :End of LIST\n")

//TOPIC
#define RPL_TOPIC(user, channel, topic) (":" + user.getUsername() + " 332 " + user.getNickname() + " " + (channel) + " :" + (topic) + "\n")

//OPER
#define RPL_YOUREOPER(user) (":" + user.getUsername() + " 381 " + user.getNickname() + " :You are now an IRC operator\r\n")
#define ERR_NOPRIVILEGES(user) ( ":" + user.getHostname() + " 481 " + user.getNickname() + " :Permission Denied- You're not an IRC operator\r\n" )
#define ERR_YOUDEAD(victim, killer) ( ":" + victim.getHostname() + " 483 " + victim.getNickname() + " :You have been killed from server by the IRC operator : " + killer.getNickname() + "\r\n" )
#define ERR_SRVDEAD(victim, killer) ( ":" + victim.getHostname() + " 483 " + victim.getNickname() + " :Server has been killed by the IRC server operator : " + killer.getNickname() + "\r\n" )

#define ERR_NEEDMOREPARAMS(user, command) (":" + user.getUsername() + " 461 " + user.getNickname() + " " + (command) + " :Not enough parameters\r\n")
