#include "../../headers/commands/User_cmd.hpp"

namespace irc {
/* Constructors & Destructors */
	User_cmd::~User_cmd() {};
	User_cmd::User_cmd(){
		_args.push_back("USER");
	};
/* Operators */
/* Functions */
	void User_cmd::exec_cmd(command const &cmd, User &user) const {
		user.set_username(_args[1]);
		user.set_realname(_args[4]);
		if (user.get_nickname().empty() == false){ // If nickname exist the user is now registered
			user.set_registered_status(true);
			user.set_uuid();
		}
	};

	void User_cmd::is_valid_args(Server const *Server, User const &user) const {
		// Possible Error : ERR_NEEDMOREPARAMS ERR_ALREADYREGISTRED
		if (this->_args.size() < 5)
			throw error(this->_args[0] + ": Not enough parameters", ERR_NEEDMOREPARAMS);
		if (user->get_registered_status() == true)
			throw error(" You're already registered", ERR_ALREADYREGISTRED);
		for (std::string::const_iterator it = _args[1].begin(); it != _args[1].end(); ++it;) {
			if ((*it) == '%x01-09' or (*it) == %x0B-0C  or (*it) == %x0E-1F or (*it) == %x21-3F or (*it) == %x41-FF)
				throw error("Invalid username", 0);
		for (string::const_iterator it = _args[4].begin(); it != _args[4].end(); ++it;) {
			if ((*it) == %x01-09 or (*it) == %x0B-0C  or (*it) == %x0E-1F or (*it) == %x41-FF)
				throw error("Invalid realname", 0);
	};
}
