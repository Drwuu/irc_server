#include "../../headers/error/error.hpp"

namespace irc {
/* Constructors & Destructors */
	error::~error() throw() {};
	error::error(): _msg(NULL) {};
	// :iridium.libera.chat 432 * ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; :Erroneous Nickname
	error::error(string const &arg, error_type const &type) {
		std::stringstream s;
		s << type << " * " << arg;
		_msg = s.str();
	};
/* Operators */
/* Getters & Setters */
/* Functions */
	const char	*error::what() const throw() {
		return _msg.c_str();
	}
}
