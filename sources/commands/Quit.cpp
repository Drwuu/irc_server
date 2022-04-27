#include "../../headers/commands/Quit.hpp"
#include <vector>

namespace irc {
	Quit::~Quit() {};
	Quit::Quit() {};
	Quit::Quit(Server *server): command(server) {};

	void Quit::exec_cmd(User &user) {
		if (_args.size() >= 2)
			user.disconnect_user(_args[1]);
		else
			user.disconnect_user(user.get_nickname());
		_server->get_event_list().push_back(new Proxy_queue::Disconnect(user.get_socket()->get_fd()));
	}
	bool Quit::is_valid_args(User const &) {
		return true;
	}
}
