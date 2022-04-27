/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaman <mhaman@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:15:16 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/27 18:21:04 by guhernan         ###   ########.fr       */
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
#include <stdint.h>

#include "headers/proxy/Proxy.hpp"
#include "headers/structure/Server.hpp"


// Function list :
// socket() - connect() - read() - write()
// Server oriented => bind() - listen() - accept()

int		main(int ac, char **av) {
	if (ac < 2 || ac > 3) {
		std::cerr << "usage : ircserv [port] [password]" << std::endl;
		return -1;
	}

	irc::Server server;

	int		port_nb = std::atoi(av[1]);
	if (port_nb > UINT16_MAX || port_nb < 1024) {
		std::cerr << "Invalid port number: " << port_nb << std::endl;
		return -1;
	}

	std::string		password = av[2];
	// If the password is longer than the IRC Message max_size
	if (password.size() > 512) {
		std::cerr << "Invalid password :too long" << std::endl;
		return -1;
	}

	server.set_port(port_nb);
	server.set_password(password);
	server.set_name("irc.42lyon.fr");
	server.set_motd("Welcome to 42lyon irc server");

	irc::Proxy	proxy(port_nb);
	irc::Proxy::api_type		to_proxy;
	irc::Proxy::api_type		from_proxy;

	proxy.switch_on();

	proxy.set_timeout(40 * 1000);


	while (1){

		proxy.queuing();

		from_proxy = proxy.send_api();
		server.receive_api(from_proxy);

		to_proxy = server.get_event_list();
		proxy.receive_api(to_proxy);
	}

	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	// Manual test for Server socket -> connexion test
	///////////////////////////////////////////////////////////////////////////////////

	// Socket<Address_ipv4>		server_socket(port_nb, SOCK_STREAM);

	// server_socket.create_endpoint();
	// server_socket.bind_socket();
	// server_socket.listen_for_connexion(5);
	// Socket<Address_ipv4>	client_socket = server_socket.accept_connexion();

	// std::clog << client_socket.get_fd() << std::endl;

	// char buff[10];
	// bzero(buff, 10);
	// if (read(client_socket.get_fd(), buff, 9) != -1)
	// 	std::cout << "Message received by the client is : " << buff << std::endl;
	// bzero(buff, 10);
	// write(client_socket.get_fd(), buff, 9);


	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	// Connection side
	///////////////////////////////////////////////////////////////////////////////////

	// int	sockfd_use = 0,
		// sockfd_server = socket(AF_INET6, SOCK_STREAM, 0);
	// if (sockfd_server == -1)
		// std::cerr << " socket Server failed " << std::endl;


	// clilen = sizeof(client_address);
	// if (bind(sockfd_server, (sockaddr *)&serv_address, serv_address.sin6_len) == -1) {
		// std::cerr << " bind failed " << std::endl;
	// }
	// if (listen(sockfd_server, 5) == -1) {
		// // The sockfd is broken
		// std::cerr << " listen failed " << std::endl;
	// }

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
	///////////////////////////////////////////////////////////////////////////////////

	//// Basic read/write message :
	//
	// char	buff[256];
	// bzero(&buff, 256);
	// read(sockfd_use, &buff, 255);
	// std::cout << "Message received by the client is : " << buff << std::endl;
	// write(sockfd_use, &buff, 255);


}
