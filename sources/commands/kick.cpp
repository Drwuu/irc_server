#include "../../headers/commands/kick.hpp"

namespace irc {
/* Constructors & Destructors */
	kick::~kick() {};
	kick::kick() {
		_args.push_back("KICK");
	};
/* Operators */
/* Functions */
	void kick::exec_cmd(command const &cmd) const {
		(void)cmd;
	};
	void kick::is_valid_args(void) const {
	};
}
