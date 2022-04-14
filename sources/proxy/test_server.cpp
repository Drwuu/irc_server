/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:15:16 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/13 14:54:30 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <unistd.h>
#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/event.h>

#include "Socket.hpp"


// Function list :
// socket() - connect() - read() - write()
// server oriented => bind() - listen() - accept()

int		main(int ac, char **av) {
	if (ac < 2 || ac > 3) {
		std::cerr << "usage : ircserv [port] [password]" << std::endl;
		return -1;
	}

	// FIX : check overflow or invalid port number
	int				port_nb = std::atoi(av[1]);

	
	Socket<Address_ipv6>		server_socket(port_nb, SOCK_STREAM);

	server_socket.create_endpoint();
	server_socket.bind_socket();
	server_socket.listen_for_connexion(5);
	Socket<Address_ipv6>	client_socket = server_socket.accept_connexion();

	std::clog << client_socket.get_fd() << std::endl;

	char buff[10];
	bzero(buff, 10);
	if (read(client_socket.get_fd(), buff, 9) != -1)
		std::cout << "Message received by the client is : " << buff << std::endl;
	bzero(buff, 10);
	// write(client_socket.get_fd(), buff, 9);


	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	// Connection side

	// FIX : check if the socket creation succed.
	// int	sockfd_use = 0,
		// sockfd_server = socket(AF_INET6, SOCK_STREAM, 0);
	// if (sockfd_server == -1)
		// std::cerr << " socket server failed " << std::endl;
	//
	// // Struct to bind address on sockfd
	// struct sockaddr_in6		serv_address;
	// struct sockaddr_in6		client_address;
	// bzero(&serv_address, sizeof(sockaddr_in6));
	// serv_address.sin6_family = AF_INET6;
	// serv_address.sin6_port = htons(port_nb);
	// serv_address.sin6_addr = in6addr_any;
	// serv_address.sin6_len = sizeof(serv_address);
	// // serv_address.sin6_flowinfo = 0; // already set with bzero()
//
	// clilen = sizeof(client_address);
	// if (bind(sockfd_server, (sockaddr *)&serv_address, serv_address.sin6_len) == -1) {
		// std::cerr << " bind failed " << std::endl;
	// }
	// if (listen(sockfd_server, 5) == -1) {
		// // The sockfd is broken
		// std::cerr << " listen failed " << std::endl;
	// }
//
	// // The rest can be managed by I/O Multiplexing
	// // with the poll() function.
	// std::cerr << " [SERVER] REACH ACCEPT" << std::endl;
	// sockfd_use = accept(sockfd_server, (sockaddr *)&client_address, &clilen);
	// if (sockfd_use == -1) {
		// std::cerr << " accept failed " << std::endl;
	// }

	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	// Conversation side


	//// Basic read/write message :
	//
	// char	buff[256];
	// bzero(&buff, 256);
	// read(sockfd_use, &buff, 255);
	// std::cout << "Message received by the client is : " << buff << std::endl;
	// write(sockfd_use, &buff, 255);
	

}