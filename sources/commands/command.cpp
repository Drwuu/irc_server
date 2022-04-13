#include "../../headers/commands/command.hpp"

namespace irc {
/* Constructors & Destructors */
	command::~command() {};
	command::command(): _args() {};
	command::command(vector_args const &args): _args(args) {};
/* Operators */
	// string command::operator()(command const &cmd) const {

	// };
/* Getters & Setters */
	vector_args const &command::get_args() const {
		return _args;
	}
	void command::set_args(vector_args const &args) {
		_args = args;
	};

/* Functions */
	// string command::send_instructions(command const &cmd) {

	// };
}
