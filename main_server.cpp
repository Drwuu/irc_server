/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaman <mhaman@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:15:16 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/19 19:50:46 by guhernan         ###   ########.fr       */
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

/////////////////////////// LUDO TESTS ///////////////////////////
	(void)av;
	irc::Server server;
	server.set_port(av[1]);
	server.set_password(av[2]);
	server.set_name("irc.42lyon.fr");
	server.set_motd("Welcome to 42lyon irc server");
	// int		port_nb = std::atoi(av[1]);
	// Proxy	server_proxy(port_nb);

	// server_proxy.switch_on();

	// server_proxy.set_timeout(10000);

	// for (int i = 0 ; i < 4 ; ++i) {
	// 	server_proxy.queuing();

	// // Reception of the list of Events
	// 	Proxy::api_type		from_proxy = server_proxy.send_api();
	// 	Proxy::api_type		to_proxy;

	// 	// Loop on the list
	// 	while (!from_proxy.empty()) {
	// 		from_proxy.front()->handle(server);
	// 		delete from_proxy.front();
	// 		from_proxy.pop_front();
	// 	}

	// // The Server send instruction to the proxy
	// // with the same type of list
	// 	to_proxy = server.send_api();
	// 	server_proxy.receive_api(to_proxy);
	// }
/////////////////////////// LUDO TESTS ///////////////////////////


	// FIXME : check overflow or invalid port number
	int		port_nb = std::atoi(av[1]); // FIXME : check overflow
	if (port_nb > 65535 || port_nb < 1024) {
		std::cerr << "invalid port number: " << port_nb << std::endl;
		return -1;
	}

	irc::Proxy	server_proxy(port_nb);

	server_proxy.switch_on();

	server_proxy.set_timeout(10000);

	for (int i = 0 ; i < 4 ; ++i) {
		server_proxy.queuing();

		irc::Proxy::api_type		from_proxy(server_proxy.send_api());
		irc::Proxy::api_type		to_proxy;

		for (irc::Proxy::api_type::iterator it = from_proxy.begin() ;
				it != from_proxy.end() ; ++it) {
			(*it)->handle();
		}
		while (!from_proxy.empty()) {
			delete from_proxy.front();
			from_proxy.pop_front();
		}

		server_proxy.receive_api(from_proxy);
	}

	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	// Manual test for Server socket -> connexion test
	///////////////////////////////////////////////////////////////////////////////////

	// Socket<Address_ipv6>		server_socket(port_nb, SOCK_STREAM);

	// server_socket.create_endpoint();
	// server_socket.bind_socket();
	// server_socket.listen_for_connexion(5);
	// Socket<Address_ipv6>	client_socket = server_socket.accept_connexion();

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
