#include "../../headers/commands/command.hpp"

namespace irc {
/* Constructors & Destructors */
	command::~command() {};
	command::command(): _args() {};
	command::command(Server *server): _args(), _server(server) {};
/* Operators */
	// string command::operator()(command const &cmd) const {

	// };
/* Getters & Setters */
	vector_string const &command::get_args() const {
		return _args;
	}
	void	command::set_args(vector_string const &args) {
		_args = args;
	};

	void	command::check_auth(const User &user) {
		if (!user.get_password_status())
			throw error(":You have not registered", ERR_NOTREGISTERED);
	}

/* Functions */
	// string command::send_instructions(command const &cmd) {

	// };
}
