/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_queue.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 19:32:34 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/19 20:06:45 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../headers/proxy/Server_queue.hpp"


///////////////////////////////////////////////////////////////////////////////
//
irc::Server_queue::Message::Message() : _data(), _socket() { }
irc::Server_queue::Message::~Message() { }

irc::Server_queue::Message::Message(irc::Server_queue::Message::data_type data,
		const irc::Server_queue::Message::socket_type *client) :
	_data(data), _socket(client) {
	std::clog << " ------------------- MESSAGE RECEIVED " << std::endl;
	std::clog << " data = " << _data << " socket = " << _socket->get_fd() << std::endl;
}

void			irc::Server_queue::Message::handle() { }

void			irc::Server_queue::Message::handle(Server &) {
	std::clog << " ------------------- MESSAGE RECEIVED " << std::endl;
	std::clog << " data = " << _data << " socket = " << _socket->get_fd() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//
irc::Server_queue::Message_priority::Message_priority() {
	std::clog << " ------------------- PRIORITY MESSAGE RECEIVED " << std::endl;
	std::clog << " data = " << _data << " socket = " << _socket->get_fd() << std::endl;
}

irc::Server_queue::Message_priority::~Message_priority() { }

void			irc::Server_queue::Message_priority::handle() { }

void			irc::Server_queue::Message_priority::handle(Server &) {
	std::clog << " ------------------- HANDLING PRIORITY MESSAGE" << std::endl;
	std::clog << " data = " << _data << std::endl;
	std::clog << " socket = " << _socket->get_fd() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//

irc::Server_queue::Request_connexion::Request_connexion(){ }

irc::Server_queue::Request_connexion::Request_connexion(const socket_type *client)
	: _socket(client) {
	std::clog << " ------------------- CONNEXION REQUEST RECEIVED "
		<< " socket = " << client->get_fd() << std::endl;
}

irc::Server_queue::Request_connexion::~Request_connexion(){ }

void			irc::Server_queue::Request_connexion::handle() { }

void			irc::Server_queue::Request_connexion::handle(Server &) {
	std::clog << " ------------------- HANDLING CONNEXION REQUEST "
		<< " socket = " << _socket->get_fd() << std::endl;
	// Call parser
	// Call client
}

///////////////////////////////////////////////////////////////////////////////
//

irc::Server_queue::Client_disconnected::Client_disconnected(){ }
irc::Server_queue::Client_disconnected::Client_disconnected(const socket_type *client)
	: _socket(client) {

	std::clog << " ------------------- CLIENT DISCONNECTION RECEIVED "
		<< " socket = " << client->get_fd() << std::endl;
}
irc::Server_queue::Client_disconnected::~Client_disconnected(){ }

void			irc::Server_queue::Client_disconnected::handle() { }

void			irc::Server_queue::Client_disconnected::handle(Server &) {
	std::clog << " ------------------- HANDLING DISCONNECTION "
		<< " socket = " << _socket->get_fd() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//

irc::Server_queue::Error::Error() {
	std::clog << " ------------------- ERROR RECEIVED "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}
irc::Server_queue::Error::~Error() { }

void			irc::Server_queue::Error::handle(){ }

void			irc::Server_queue::Error::handle(Server &){
	std::clog << " ------------------- HANDLING ERROR "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//

irc::Server_queue::Signal::Signal() {
	std::clog << " ------------------- SIGNAL RECEIVED "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}
irc::Server_queue::Signal::~Signal() { }

void			irc::Server_queue::Signal::handle(){ }

void			irc::Server_queue::Signal::handle(Server &){
	std::clog << " ------------------- HANDLING SIGNAL "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}