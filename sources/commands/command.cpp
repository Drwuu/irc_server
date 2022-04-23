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

	// If auth failed, it means the user have to be disconnected and deleted.
	// Add : new Proxy_queue::Disconnect()  +  del_user() in catch {}
	void	command::check_auth(const User &user) {
		if (!user.get_password_status()) {
			_server->get_event_list().push_back(new Proxy_queue::Disconnect(user.get_socket()->get_fd()));
			throw error(":You have not registered", ERR_NOTREGISTERED);
		}
	}

/* Functions */
	// string command::send_instructions(command const &cmd) {

	// };
}
