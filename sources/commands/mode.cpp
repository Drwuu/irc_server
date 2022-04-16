#include "../../headers/commands/mode.hpp"

namespace irc {
/* Constructors & Destructors */
	mode::~mode() {};
	mode::mode(vector_args const &args) {
		_args = args;
	};
/* Operators */
/* Functions */
	void mode::exec_cmd(command const &cmd) const {
		(void)cmd;
	};
	void mode::_is_valid_args(vector_args const &args) throw() {
		(void)args;
	};
}
