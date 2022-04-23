#include "../../headers/proxy/Server_queue.hpp"
#include <sys/_types/_size_t.h>

///////////////////////////////////////////////////////////////////////////////
//
irc::Server_queue::Message::Message() : _data(), _socket() { }
irc::Server_queue::Message::~Message() {}

irc::Server_queue::Message::Message(irc::Server_queue::Message::data_type data,
		const irc::Server_queue::Message::socket_type *client) :
	_data(), _socket(client) {
	bzero(_data, 513);
	strlcpy(_data, data, 512);
}

void			irc::Server_queue::Message::handle(Proxy &) { }

void			irc::Server_queue::Message::handle(Server &server) {
	User	&user = *server.get_user_from_socket(_socket);

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
			cmd->check_auth(user);
			cmd->set_args(server._parser.get_args(cmd_list.front()));
			cmd->is_valid_args(user);
			cmd->exec_cmd(user);
		}
		catch (irc::error &e) {
			// Return the event to the client
			std::stringstream	ss;
			ss << ":" << server.get_name() << " " << e.what() << "\r\n";
			server.get_event_list().push_back(new Proxy_queue::Write(user.get_socket()->get_fd(), ss.str().c_str()));
			if (!user.get_password_status())
				server.del_user(user);
		}
		cmd_list.pop_front();
	}
}

///////////////////////////////////////////////////////////////////////////////
//

irc::Server_queue::Request_connexion::Request_connexion(){ }

irc::Server_queue::Request_connexion::Request_connexion(const socket_type *client)
	: _socket(client) {
}

irc::Server_queue::Request_connexion::~Request_connexion(){ }

void			irc::Server_queue::Request_connexion::handle(Proxy &) { }

void			irc::Server_queue::Request_connexion::handle(Server &server) {
	server.get_user_from_socket(_socket);
}

///////////////////////////////////////////////////////////////////////////////
//

irc::Server_queue::Client_disconnected::Client_disconnected(){ }
irc::Server_queue::Client_disconnected::Client_disconnected(const socket_type *client)
	: _socket(client) { }
irc::Server_queue::Client_disconnected::~Client_disconnected(){ }

void			irc::Server_queue::Client_disconnected::handle(Proxy &) { }

void			irc::Server_queue::Client_disconnected::handle(Server &server) {
	irc::User	*user = server.get_user_from_socket(_socket);
	fd_type		user_sockfd = server.del_user(*user);
	if (user_sockfd != -1)
		server._event_list.push_back(new Proxy_queue::Disconnect(user_sockfd));
	else
		std::clog << "[ERROR] : Client disconnected not found." << std::endl;
}
