/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server111.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelauna <tdelauna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:44:23 by aptive            #+#    #+#             */
/*   Updated: 2023/05/23 16:12:05 by tdelauna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//------------------------------------------------------------------------------
// Fichier a supprimer, tranformation en classe Server
//------------------------------------------------------------------------------

// #include "../includes/web_serv.hpp"

// void todo_connexion(int fd)
// {
// 	// Accepter une nouvelle connexion entrante
// 	struct sockaddr_in client_addr;
// 	socklen_t client_addr_len = sizeof(client_addr);
// 	int client_server_fd = accept(fd, (struct sockaddr*)&client_addr, &client_addr_len);
// 	if (client_server_fd == -1)
// 	{
// 		std::cerr << "Erreur lors de l'acceptation d'une connexion entrante" << std::endl;

// 	}


// 	// TODO: traiter la connexion entrante

// 	affichage_socket(client_server_fd, client_addr);

// 	send(client_server_fd, "Server : Salut de la part du server !", 38, MSG_NOSIGNAL);

// 	// receive a message
// 	// int ok = 1;
// 	// while (ok)
// 	// {
// 		char buffer[1024];
// 		ssize_t numBytes = recv(client_server_fd, buffer, sizeof(buffer), 0);
// 		if (numBytes == -1) {
// 			std::cerr << "Error receiving message\n";
// 			// handle error
// 		}
// 		else if (numBytes == 0) {
// 			std::cout << "Connection closed by peer\n";
// 			// ok = 0;
// 			// handle connection closed by peer
// 		}
// 		else {
// 			buffer[numBytes] = '\0'; // add null terminator
// 			std::cout << "Received message: " << buffer << "\n";
// 			// handle received message
// 		}
// 	// }
// 	// Fermer le socket de la connexion
// }
