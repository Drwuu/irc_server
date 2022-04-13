/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaman <mhaman@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 23:03:17 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/13 20:04:28 by mhaman           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/proxy/Proxy.hpp"

// Should stay unaccessible
Proxy::Proxy()
	: _server(0, SOCK_STREAM), _clients(), _flags(), _poll_data(),
	_timeout(), _to_server(), _to_clients() { }

Proxy::Proxy(const port_type &port)
	: _server(port, SOCK_STREAM), _clients(), _flags(), _poll_data(),
	_timeout(), _to_server(), _to_clients() {
		this->init_flags();
		this->init_poll_args();
	}

Proxy::Proxy(const Proxy &source)
	: _server(source._server), _clients(source._clients),
	_flags(source._flags), _poll_data(source._poll_data),
	_timeout(source._timeout),  _to_server(source._to_server), _to_clients(source._to_server) { }

Proxy::~Proxy() { }

Proxy	&Proxy::operator=(const Proxy &source) {
	_server.end_connexion();
	_server = source._server;
	_clients = source._clients;
	_flags = source._flags;
	_poll_data = source._poll_data;
	_timeout = source._timeout;
	_to_server = source._to_server;
	_to_clients = source._to_clients;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
void			Proxy::switch_on() {
	this->init_server_socket();
}

void			Proxy::switch_off() {
	this->end_all_connexions();
	this->_poll_data.clear();
	this->_to_clients.clear();
	this->_to_server.clear();
}

void		Proxy::set_timeout(Proxy::milisecond_type timeout) {
	_timeout = timeout;
}


void			Proxy::queuing() {
	poll(_poll_data.data(), _poll_data.size(), _timeout);
}

Proxy::api_type	Proxy::send_data() { return _to_server; }

void			Proxy::receive_data(const api_type &data) {(void)data;}

//////////////////////////////////////////////////////////////////////////
// Private methodes

void		Proxy::end_all_connexions() {
}

void		Proxy::end_connexion() {
}

void	Proxy::init_flags() {
	fd_type		serv_sock = _server.get_fd();
	(void)serv_sock;
	_flags.insert(std::make_pair(POLLIN, new Poll_in()));
	_flags.insert(std::make_pair(POLLPRI, new Poll_priority_in()));
	_flags.insert(std::make_pair(POLLNVAL, new Poll_invalid()));
	_flags.insert(std::make_pair(POLLHUP, new Poll_hang_up()));
	_flags.insert(std::make_pair(POLLERR, new Poll_error()));
}

void	Proxy::init_server_socket() {
	_server.create_endpoint();
	_server.bind_socket();
	_server.listen_for_connexion(); // set on 5 maximum requested connexion (default)
}

void	Proxy::init_poll_args() {
	struct pollfd	server_sock;

	bzero(&server_sock, sizeof(pollfd));
	server_sock.fd = _server.get_fd();
	server_sock.events = POLLIN;
	_poll_data.push_back(server_sock);
}

void		update_flags() {
}
