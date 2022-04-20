#include "../../headers/commands/User_cmd.hpp"

namespace irc {
/* Constructors & Destructors */
	User_cmd::~User_cmd() {};
	User_cmd::User_cmd(){
		_args.push_back("USER");
	};
/* Operators */
/* Functions */
	void User_cmd::exec_cmd(User &user) {
		user.set_username(_args[1]);
		user.set_hostname(_args[3]);
		user.set_realname(_args[4]);
			if (user.get_registered_status() == false)
			{
				user.set_registered_status(true);
				user.set_uuid();
				//throw error("Welcome to our 42Lyon IRC network " + user.get_nickname(), RPL_WELCOME);
				std::string ret = "Welcome to our 42Lyon IRC network " + user.get_nickname();
				Proxy_queue::Write * msg = new Proxy_queue::Write(user.get_socket()->get_fd(),ret.c_str());
			}
	};

	// Server is unused here.
	void User_cmd::is_valid_args(Server const *, User const &user) const {
		// Possible Error : ERR_NEEDMOREPARAMS ERR_ALREADYREGISTRED
		if (this->_args.size() < 5)
			throw error(this->_args[0] + ": Not enough parameters", ERR_NEEDMOREPARAMS);
		if (user.get_registered_status() == true)
			throw error(" You're already registered", ERR_ALREADYREGISTRED);
		for (std::string::const_iterator it = _args[1].begin(); it != _args[1].end(); ++it)
			if ((*it) == 0 or (*it) == 10  or (*it) == 13 or (*it) == 64 or (*it) == 32)
				throw error("Invalid username", ERR_ERRONEUSNICKNAME);
				// WIP verfication du hostname a faire
		for (string::const_iterator it = _args[4].begin(); it != _args[4].end(); ++it)
			if ((*it) == 0 or (*it) == 10  or (*it) == 13 or (*it) == 64)
				throw error("Invalid realname", ERR_ERRONEUSNICKNAME); // Maybe add some custom error
	};
}
