#include "../../headers/commands/mode.hpp"

namespace irc {
/* Constructors & Destructors */
	mode::~mode() {};
	mode::mode() {
		_args.push_back("MODE");
	};
/* Operators */
/* Functions */
	void mode::exec_cmd(command const &cmd) const {
		(void)cmd;
	};
	void mode::is_valid_args(Server const *Server) const {
		(void)Server;
	};
}
