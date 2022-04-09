/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:15:16 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/05 19:48:48 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

// Function list :
// socket() - connect() - read() - write()
// server oriented => bind() - listen() - accept()

int		main(int ac, char **av) {

	if (ac < 2 || ac > 3) {
		std::cerr << "usage : ircserv [port] [password]" << std::endl;
		return -1;
	}

	// FIX : check overflow or invalid port number
	int	port_nb = std::atoi(av[1]);

	unsigned int	 clilen = 0;

	// FIX : check if the socket creation succed.
	int	sockfd_use = 0,
		sockfd_server = socket(AF_INET6, SOCK_STREAM, 0);
	// Struct to bind address on sockfd
	struct sockaddr_in6		serv_address;
	struct sockaddr_in6		client_address;
	bzero(&serv_address, sizeof(sockaddr_in6));
	serv_address.sin6_family = AF_INET6;
	serv_address.sin6_port = htons(port_nb);
	serv_address.sin6_addr = in6addr_any;
	serv_address.sin6_len = sizeof(serv_address);
	// serv_address.sin6_flowinfo = 0; // already set with bzero()

	clilen = sizeof(client_address);
	if (bind(sockfd_server, (sockaddr *)&serv_address, serv_address.sin6_len) == -1) {
		std::cerr << " bind failed " << std::endl;
	}
	if (listen(sockfd_server, 5) == -1) {
		// The sockfd is broken
		std::cerr << " listen failed " << std::endl;
	}

	// The rest can be managed by I/O Multiplexing
	// with the poll() function.
	sockfd_use = accept(sockfd_server, (sockaddr *)&client_address, &clilen);
	if (sockfd_use == -1) {
		std::cerr << " accept failed " << std::endl;
	}
	char	buff[256];
	bzero(&buff, 256);
	read(sockfd_use, &buff, 255);
	std::cout << "Message received by the client is : " << buff << std::endl;
	write(sockfd_use, &buff, 255);
	close(sockfd_use);
	close(sockfd_server);
}
