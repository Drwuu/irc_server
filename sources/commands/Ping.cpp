
#include "../../headers/commands/Ping.hpp"

namespace irc {


///////////////////////////////////////////////////////////////////////
// Private 
	Ping	&Ping::operator=(const Ping &) { return *this;}


///////////////////////////////////////////////////////////////////////
// Public 
	Ping::Ping() { }
	Ping::Ping(Server *server) : command(server) { }
	Ping::~Ping() { }

	Ping::Ping(const Ping &source) {
		_args = source._args;
		_server = source._server;
	}

	void	Ping::exec_cmd(User &user) {
		std::string		rtn("PING " + user.get_nickname() + "\r\n\0");

		Socket_event *event = new Proxy_queue::Write(user.get_socket()->get_fd(), rtn.c_str());
		_server->get_event_list().push_back(event);
	}

	bool	Ping::is_valid_args(const User &) { return true; }
}
