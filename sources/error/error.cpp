#include "../../headers/error/error.hpp"

namespace irc {
/* Constructors & Destructors */
	error::~error() throw() {};
	error::error(): _msg(nullptr) {};
	error::error(string const &arg, error_type const &type) {
		std::stringstream s;
		s << "Error " << type << " | " << arg;
		_msg = s.str();
	};
/* Operators */
/* Getters & Setters */
/* Functions */
	const char	*error::what() const throw() {
		return _msg.c_str();
	}
}
