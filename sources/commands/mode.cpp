#include "../../headers/commands/mode.hpp"

namespace irc {
/* Constructors & Destructors */
	mode::~mode() {};
	mode::mode() {};
	mode::mode(Server *server): command(server) {};
/* Operators */
/* Functions */
	void mode::exec_cmd(User &user) {
		(void)user;
	};
	void mode::is_valid_args(Server const *Server, User const &user) {
		(void)Server;
		(void)user;
	};
}
