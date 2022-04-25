/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy_queue.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 19:31:37 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/23 21:23:44 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/proxy/Proxy_queue.hpp"


///////////////////////////////////////////////////////////////////////////////
//
irc::Proxy_queue::Write::Write() { }

irc::Proxy_queue::Write::Write(fd_type socketfd, data_type data)
	: _data(strdup(data)), _socketfd(socketfd) {
	}

irc::Proxy_queue::Write::~Write() { }

void			irc::Proxy_queue::Write::handle(Server &) { }
void			irc::Proxy_queue::Write::handle(Proxy &proxy) {
	proxy.push_back_queue(_socketfd, _data);
}

///////////////////////////////////////////////////////////////////////////////
//
irc::Proxy_queue::Write_priority::Write_priority() { }
irc::Proxy_queue::Write_priority::Write_priority(fd_type socketfd, data_type data)
	: _data(data), _socketfd(socketfd) {
	}
irc::Proxy_queue::Write_priority::~Write_priority() { }

void			irc::Proxy_queue::Write_priority::handle(Server &) { }
void			irc::Proxy_queue::Write_priority::handle(Proxy &proxy) {
	proxy.push_back_queue(_socketfd, _data);
}

///////////////////////////////////////////////////////////////////////////////
//
irc::Proxy_queue::Disconnect_all::Disconnect_all() { }
irc::Proxy_queue::Disconnect_all::~Disconnect_all() { }
void			irc::Proxy_queue::Disconnect_all::handle(Server &) { }
void			irc::Proxy_queue::Disconnect_all::handle(Proxy &proxy) { proxy.end_all_connexions(); }

///////////////////////////////////////////////////////////////////////////////
//
irc::Proxy_queue::Disconnect::Disconnect() { }
irc::Proxy_queue::Disconnect::Disconnect(const fd_type &socket_id)
	:_socket_id(socket_id) {
	}
irc::Proxy_queue::Disconnect::~Disconnect() { }

void			irc::Proxy_queue::Disconnect::handle(Server &) { }
void			irc::Proxy_queue::Disconnect::handle(Proxy &proxy) {
	irc::Proxy::client_tree_type::iterator	it = proxy._clients.find(_socket_id);

	if (it == proxy._clients.end())
		std::cerr << " [ERROR] No client to disconnect. [" << _socket_id << "]" << std::endl;
	proxy.end_connexion(*it->second);
}

