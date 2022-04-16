#include "../../headers/commands/invite.hpp"

namespace irc {
/* Constructors & Destructors */
	invite::~invite() {};
	invite::invite(vector_args const &args) {
		_args = args;
	};
/* Operators */
/* Functions */
	void invite::exec_cmd(command const &cmd) const {
		(void)cmd;
	};
	void invite::_is_valid_args(vector_args const &args) throw() {
		(void)args;
	};
}
