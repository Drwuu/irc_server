#include "../../headers/commands/mode.hpp"

namespace irc {
/* Constructors & Destructors */
	mode::~mode() {};
	mode::mode() {};
/* Operators */
/* Functions */
	void mode::exec_cmd(command const &cmd, User const &user) const {
		(void)cmd;
		(void)user;
	};
	void mode::is_valid_args(Server const *Server, User const &user) const {
		(void)Server;
		(void)user;
	};
}
