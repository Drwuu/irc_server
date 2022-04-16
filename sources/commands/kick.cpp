#include "../../headers/commands/kick.hpp"

namespace irc {
/* Constructors & Destructors */
	kick::~kick() {};
	kick::kick(vector_args const &args) {
		_args = args;
	};
/* Operators */
/* Functions */
	void kick::exec_cmd(command const &cmd) const {
		(void)cmd;
	};
	void kick::_is_valid_args(vector_args const &args) throw() {
		(void)args;
	};
}
