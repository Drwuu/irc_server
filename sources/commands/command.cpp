#include "../../headers/commands/command.hpp"

namespace irc {
/* Constructors & Destructors */
	command::~command() {};
	command::command(): _name() {};
	command::command(string const &name): _name(name) {};
	command::command(command const &src) {
		*this = src;
	};
/* Operators */
	command& command::operator=(command const &src) {
		_name = src._name;
		return *this;
	};
	// string command::operator()(command const &cmd) const {

	// };
/* Getters & Setters */
	string const	&command::get_name() const {
		return _name;
	};

/* Functions */
	// string command::send_instructions(command const &cmd) {

	// };
}
