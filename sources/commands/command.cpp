#include "../../headers/commands/command.hpp"

namespace irc {
/* Constructors & Destructors */
	command::~command() {};
	command::command() {

	};
	command::command(command const &src) {
		*this = src;
	};
/* Operators */
	command& command::operator=(command const &src) {
		(void)src;
		return *this;
	};
	// string command::operator()(command const &cmd) const {

	// };
/* Getters & Setters */
/* Functions */
	// string command::send_instructions(command const &cmd) {

	// };
}
