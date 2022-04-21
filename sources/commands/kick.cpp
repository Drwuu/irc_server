#include "../../headers/commands/kick.hpp"

namespace irc {
/* Constructors & Destructors */
	kick::~kick() {};
	kick::kick() {};
/* Operators */
/* Functions */
	void kick::exec_cmd(User &user) {
		(void)user;
	};
	void kick::is_valid_args(Server const *Server, User const &user) const {
		(void)Server;
		(void)user;
	};
}
