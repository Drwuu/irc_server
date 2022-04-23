#include "../../headers/commands/kick.hpp"

namespace irc {
/* Constructors & Destructors */
	kick::~kick() {};
	kick::kick() {};
	kick::kick(Server *server): command(server) {};
/* Operators */
/* Functions */
	void kick::exec_cmd(User &user) {
		(void)user;
	};
	bool kick::is_valid_args(User const &user) {
		(void)user;
		return true;
	};
}
