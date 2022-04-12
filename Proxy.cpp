/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 23:03:17 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/12 15:38:45 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Proxy.hpp"

// Should stay unaccessible
Proxy::Proxy()
	: _server(), _client(), _to_server(), _to_client() { }

Proxy::Proxy(const port_type &port)
	: _server(), _client(), _to_server(), _to_client() {
	_server.sockfd = 0;
	_server.port = port;
	_server.type = SOCK_STREAM;
	// It means : let the system decide based on the socket 'type'(sort).
	_server.protocol = 0;

	_server.address.sin6_family = AF_INET6;
	_server.address.sin6_addr = in6addr_any;
	_server.address.sin6_port = htons(_server.port);
	_server.address.sin6_len = sizeof(_server.address);

	_server.len = _server.address.sin6_len;
}

Proxy::Proxy(const Proxy &source) { *this = source; }

Proxy::~Proxy() { }

Proxy	&Proxy::operator=(const Proxy &source) {
	this->_client = source._client;
	this->_to_client = source._to_server;
	
	this->_server.sockfd = source._server.sockfd;
	this->_server.port = source._server.port;
	this->_server.address = source._server.address;
	this->_server.len = source._server.len;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//

//////////////////////////////////////////////////////////////////////////
// Private methodes

void	Proxy::create_socket(socket_type &new_socket) {
	std::clog << "Creating new_socket on port " << new_socket.port << std::endl;
	new_socket.sockfd = socket(new_socket.address.sin6_family, new_socket.type, new_socket.protocol);
	if (new_socket.sockfd == -1) {
		std::cerr << " [ERROR] : new_socket server failed " << std::endl;
		return ;
	}
}

void	Proxy::manage_new_connexion(socket_type &new_client) {
	new_client.sockfd = accept(_server.sockfd, (sockaddr *)&new_client.address, &new_client.len);
	if (new_client.sockfd == -1) {
		std::cerr << " accept failed " << std::endl;
	}
}

void	Proxy::switch_on() {
	create_socket(_server);
	bind_socket();
	listen();
}

