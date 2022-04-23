/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_queue.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaman <mhaman@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 19:32:34 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/22 20:55:40 by mhaman           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/proxy/Server_queue.hpp"
#include <sys/_types/_size_t.h>

///////////////////////////////////////////////////////////////////////////////
//
irc::Server_queue::Message::Message() : _data(), _socket() { }
irc::Server_queue::Message::~Message() { }

irc::Server_queue::Message::Message(irc::Server_queue::Message::data_type data,
		const irc::Server_queue::Message::socket_type *client) :
	_data(), _socket(client) {
	bzero(_data, 513);
	strlcpy(_data, data, 512);
	std::clog << " ------------------- MESSAGE RECEIVED creation" << std::endl;
	std::clog << " data = " << _data << " socket = " << _socket->get_fd() << std::endl;
}

void			irc::Server_queue::Message::handle(Proxy &) { }

void			irc::Server_queue::Message::handle(Server &server) {
	std::clog << " ------------------- MESSAGE RECEIVED handle " << std::endl;
	std::clog << " data = " << _data << " socket = " << _socket->get_fd() << std::endl;

	User	&user = *server.get_user_from_socket(_socket);
	std::cout << "user = " << user.get_username() << std::endl;
	user.set_server(&server);
	std::string data = _data;
	for(std::string::iterator it = data.begin(); it != data.end(); ++it)
		if (*it == '\r')
			data.erase(it);
	std::list<std::string>	cmd_list = server._parser.split_command(data);

	while (!cmd_list.empty()) {
		command *cmd = NULL;
		try {
			std::clog << " ---------- COMMAND : " << cmd_list.front() << std::endl;
			cmd = server._parser.get_command(cmd_list.front(), server._map)->second;
			cmd->set_args(server._parser.get_args(cmd_list.front()));
			// FIXME : is_valid_args not working properly
			cmd->is_valid_args(&server, user);
			cmd->exec_cmd(user);
			//
			// Execution
			//
		}
		catch (irc::error &e) {
			std::cout << " ------------ CHECK ERROR : " << e.what() << std::endl;
			// Return the event to the client
			std::stringstream	ss;
			ss << ":" << server.get_name() << " " << e.what();
			server.get_event_list().push_back(new Proxy_queue::Write(user.get_socket()->get_fd(), ss.str().c_str()));
		}
		cmd_list.pop_front();
	}
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

void			irc::Server_queue::Request_connexion::handle(Proxy &) { }

void			irc::Server_queue::Request_connexion::handle(Server &) {
	std::clog << " ------------------- HANDLING CONNEXION REQUEST "
		<< " socket = " << _socket->get_fd() << std::endl;
	// irc::User	*unknown_user = server.get_user_from_socket(_socket);
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

void			irc::Server_queue::Client_disconnected::handle(Proxy &) { }

void			irc::Server_queue::Client_disconnected::handle(Server &server) {
	std::clog << " ------------------- HANDLING DISCONNECTION "
		<< " socket = " << _socket->get_fd() << std::endl;
	irc::User	*user = server.get_user_from_socket(_socket);
	fd_type		user_sockfd = server.del_user(*user);
	if (user_sockfd != -1)
		server._event_list.push_back(new Proxy_queue::Disconnect(user_sockfd));
	else
		std::clog << "[ERROR] : Client disconnected not found." << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//

irc::Server_queue::Error::Error() : _data(), _socket() {

	std::clog << " ------------------- ERROR RECEIVED "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}

irc::Server_queue::Error::Error(data_type data, const socket_type *socket)
	: _data(), _socket(socket) {
	bzero(_data, 513);
	strlcpy(_data, data, 512);
}

irc::Server_queue::Error::~Error() { }

void			irc::Server_queue::Error::handle(Proxy &){ }

void			irc::Server_queue::Error::handle(Server &){
	std::clog << " ------------------- HANDLING ERROR "
		<< " socket = " << _socket->get_fd() << std::endl;
	std::clog << " data = " << _data << std::endl;
}

