#include "../../headers/commands/kick.hpp"

namespace irc {
/* Constructors & Destructors */
	kick::~kick() {};
	kick::kick() {
		_args.push_back("KICK");
	};
/* Operators */
/* Functions */
	void kick::exec_cmd(command const &cmd, User const &user) const {
		(void)cmd;
		(void)user;
	};
	void kick::is_valid_args(Server const *Server, User const &user) const {
		(void)Server;
		(void)user;
	};
}
