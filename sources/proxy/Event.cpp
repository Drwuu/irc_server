/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwourms <lwourms@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 16:40:50 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/19 17:36:03 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/proxy/Event.hpp"

///////////////////////////////////////////////////////////////////////////////
//
Socket_event::Socket_event() { }
Socket_event::~Socket_event() { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Write::Write() { }

Proxy_queue::Write::Write(fd_type socketfd, data_type data)
	: _data(data), _socketfd(socketfd) {
	std::clog << " ------------------- MESSAGE to SEND :" << _data << std::endl;
	std::clog << " ---------- fd = " << _socketfd << std::endl;
	}

Proxy_queue::Write::~Write() { }
void			Proxy_queue::Write::handle() { }
void			Proxy_queue::Write::handle(Proxy &) {
}

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Write_priority::Write_priority() { }
Proxy_queue::Write_priority::Write_priority(fd_type socketfd, data_type data)
	: _data(data), _socketfd(socketfd) { }
Proxy_queue::Write_priority::~Write_priority() { }
void			Proxy_queue::Write_priority::handle() {
	std::clog << " ------------------- MESSAGE to SEND :" << _data << std::endl;
	std::clog << " ---------- fd = " << _socketfd << std::endl;
}
void			Proxy_queue::Write_priority::handle(Proxy &) { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Disconnect_all::Disconnect_all() { }
Proxy_queue::Disconnect_all::~Disconnect_all() { }
void			Proxy_queue::Disconnect_all::handle() { }
void			Proxy_queue::Disconnect_all::handle(Proxy &) { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Disconnect::Disconnect() { }
Proxy_queue::Disconnect::Disconnect(const fd_type &socket_id)
	:_socket_id(socket_id) {
		std::clog << " ------------------- DISCONNECT RECEIVED " << std::endl;
		std::clog << " ---------- fd = " << _socket_id << std::endl;
	}
Proxy_queue::Disconnect::~Disconnect() { }
void			Proxy_queue::Disconnect::handle() { }
void			Proxy_queue::Disconnect::handle(Proxy &) {
	std::clog << " ------------------- DISCONNECT RECEIVED " << std::endl;
	std::clog << " ---------- fd = " << _socket_id << std::endl;
}

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
Server_queue::Message::Message() : _data(), _socket() { }
Server_queue::Message::~Message() { }

Server_queue::Message::Message(Server_queue::Message::data_type data,
		const Server_queue::Message::socket_type *client) :
	_data(data), _socket(client) {
	std::clog << " ------------------- MESSAGE RECEIVED " << std::endl;
	std::clog << " data = " << _data << " socket = " << _socket->get_fd() << std::endl;
}

void			Server_queue::Message::handle() { }

void			Server_queue::Message::handle(Server &) {
	std::clog << " ------------------- MESSAGE RECEIVED " << std::endl;
	std::clog << " data = " << _data << " socket = " << _socket->get_fd() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//
Server_queue::Message_priority::Message_priority() {
	std::clog << " ------------------- PRIORITY MESSAGE RECEIVED " << std::endl;
	std::clog << " data = " << _data << " socket = " << _socket->get_fd() << std::endl;
}

Server_queue::Message_priority::~Message_priority() { }

void			Server_queue::Message_priority::handle() { }

void			Server_queue::Message_priority::handle(Server &) {
	std::clog << " ------------------- HANDLING PRIORITY MESSAGE" << std::endl;
	std::clog << " data = " << _data << std::endl;
	std::clog << " socket = " << _socket->get_fd() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//

Server_queue::Request_connexion::Request_connexion(){ }

Server_queue::Request_connexion::Request_connexion(const socket_type *client)
	: _socket(client) {
	std::clog << " ------------------- CONNEXION REQUEST RECEIVED "
		<< " socket = " << client->get_fd() << std::endl;
}

Server_queue::Request_connexion::~Request_connexion(){ }

void			Server_queue::Request_connexion::handle() { }

void			Server_queue::Request_connexion::handle(Server &) {
	std::clog << " ------------------- HANDLING CONNEXION REQUEST "
		<< " socket = " << _socket->get_fd() << std::endl;
	// Call parser
	// Call client
}

///////////////////////////////////////////////////////////////////////////////
//

Server_queue::Client_disconnected::Client_disconnected(){ }
Server_queue::Client_disconnected::Client_disconnected(const socket_type *client)
	: _socket(client) {

	std::clog << " ------------------- CLIENT DISCONNECTION RECEIVED "
		<< " socket = " << client->get_fd() << std::endl;
}
Server_queue::Client_disconnected::~Client_disconnected(){ }

void			Server_queue::Client_disconnected::handle() { }

void			Server_queue::Client_disconnected::handle(Server &) {
	std::clog << " ------------------- HANDLING DISCONNECTION "
		<< " socket = " << _socket->get_fd() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//

Server_queue::Error::Error(){
	std::clog << " ------------------- ERROR RECEIVED "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}
Server_queue::Error::~Error(){ }

void			Server_queue::Error::handle(){ }

void			Server_queue::Error::handle(Server &){
	std::clog << " ------------------- HANDLING ERROR "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//

Server_queue::Signal::Signal(){
	std::clog << " ------------------- SIGNAL RECEIVED "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}
Server_queue::Signal::~Signal(){ }

void			Server_queue::Signal::handle(){ }

void			Server_queue::Signal::handle(Server &){
	std::clog << " ------------------- HANDLING SIGNAL "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}
