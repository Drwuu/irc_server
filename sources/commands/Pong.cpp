
#include "../../headers/commands/Pong.hpp"

namespace irc {


///////////////////////////////////////////////////////////////////////
// Private 
	Pong	&Pong::operator=(const Pong &) { return *this;}


///////////////////////////////////////////////////////////////////////
// Public 
	Pong::Pong() { }
	Pong::Pong(Server *server) : command(server) { }
	Pong::~Pong() { }

	Pong::Pong(const Pong &source) {
		_args = source._args;
		_server = source._server;
	}

	void	Pong::exec_cmd(User &) { }

	bool	Pong::is_valid_args(const User &user) {
		if (_args.size() != 2)
			throw error(":Not enough parameters", ERR_NEEDMOREPARAMS);
		else if (_args[1] != _server->get_name() && _args[1] != user.get_nickname())
			throw error(":No such server", ERR_NOSUCHSERVER);
		return true;
	}
}
