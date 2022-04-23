#include "../../headers/commands/Pass.hpp"

///////////////////////////////////////////////////////////////////////
// Private 
irc::Pass	&irc::Pass::operator=(const Pass &) { return *this; }

///////////////////////////////////////////////////////////////////////
// Public
void		irc::Pass::exec_cmd(User &user) {
	user.set_password_status(true);
}

bool		irc::Pass::is_valid_args(User const &user) {
	if (_args.size() != 2) {
		if (!user.get_password_status())
			_server->get_event_list().push_back(new Proxy_queue::Disconnect(user.get_socket()->get_fd()));
		throw error(":Not enough parameters", ERR_NEEDMOREPARAMS);
	}
	else if (_args[1] != _server->get_password()) {
		if (!user.get_password_status())
			_server->get_event_list().push_back(new Proxy_queue::Disconnect(user.get_socket()->get_fd()));
		throw error(":Password incorrect", ERR_PASSWDMISMATCH);
	}
	else if (user.get_registered_status() || user.get_password_status())
		throw error(":You may not reregister", ERR_ALREADYREGISTRED);
	return true;
}

void	irc::Pass::check_auth(const User &) { }

///////////////////////////////////////////////////////////////////////
// Constructor - Destructors 
irc::Pass::Pass(const Pass &source) {
	_args = source._args;
	_server = source._server;
}

irc::Pass::Pass() { }
irc::Pass::Pass(Server *server) : command(server) { }
irc::Pass::~Pass() { }
