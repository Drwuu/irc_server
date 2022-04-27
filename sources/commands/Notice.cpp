#include "../../headers/commands/Notice.hpp"
#include "../../headers/Help.hpp"

namespace irc {
/* Constructors & Destructors */
	Notice::~Notice() {};
	Notice::Notice(){};
	Notice::Notice(Server *_server): command(_server) {};

	bool Notice::find_receiver(Server const *_server,const std::string receiver) {
		std::vector<User *> userlist = _server->get_user_list();
		if (_server->find_nickname(receiver, userlist) != userlist.end())
		{
			return true;
		}
		return false;
	}

	bool Notice::is_valid_nickname(const std::string &receiver) const{
			string::const_iterator it = receiver.begin();
			if (!(*it >= 65 && *it <= 125))
				return false;
			it++;
			for (;it != receiver.end();++it)
			{
				if ((*it < 48 && *it != 45) or (*it > 57 and *it < 65) or (*it > 125))
					return false;
			}
			return true;
	}

	void	Notice::exec_cmd(User &user ) {
			if (is_valid_nickname(_args[1]) == true){
				std::vector<User *>		user_list = _server->get_user_list();
				std::vector<User *>::const_iterator receiver = _server->find_nickname(_args[1], user_list);
				std::clog << "RECEIVER NICKNAME = " << (*receiver)->get_nickname() << std::endl;
				if ((*receiver)->get_nickname() != "Help_bot")
					user.send_message(" NOTICE " + (*receiver)->get_nickname() + " " + _args[2], *(*receiver));
				else
				{
					(*receiver)->send_message("NOTICE " + (*receiver)->get_nickname() + " " + _args[2], user);
				}
			}
	}

	bool	Notice::is_valid_args(User const &user) {
		if (user.get_registered_status() == false)
			return false;
		if (_args.size() < 3)
			return false;
		else if (_args.size() < 4 && _args[2].size() == 0)
			return false;
		else if (!is_valid_nickname(_args[1]))
			return false;
		else if (!find_receiver(_server,_args[1]))
			return false;
		return true;
	}
}
