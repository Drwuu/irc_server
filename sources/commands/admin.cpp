#include "../../headers/commands/admin.hpp"

namespace irc {
/* Constructors & Destructors */
	admin::~admin() {};
	admin::admin() {};
	admin::admin(string const &name) {
		_name = name;
	};
	admin::admin(admin const &src) {
		*this = src;
	};
	admin &admin::operator=(admin const &src) {
		(void)src;
		return *this;
	};
/* Operators */
/* Functions */
}
