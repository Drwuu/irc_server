#include "../../headers/commands/Nick.hpp"

namespace irc {
/* Constructors & Destructors */
	Nick::~Nick() {};
	Nick::Nick(){
		_args.push_back("NICK");
	};
/* Operators */
/* Functions */
	bool Nick::is_special_char(int c){
		if (c == '[' or c == ']' or c == '\\' or
		c == '`' or c == '_' or c == '^' or
		c == '{' or c == '}' or c == '|' or c == '-' )
			return true;
		return false;
	}
	bool Nick::is_nickname_valid() {
			string::iterator it = argv[1].begin();
			if (!isalpha(*it) or !is_special_char(*it))
				return false;
			it++;
			for (;it != args[1].end();++it)
			{
				if (!isalpha(*it) or !isdigit(*it) or !is_special_char(*it))
					return false;
			}
			return true;
	}
	void Nick::exec_cmd(command const &cmd, User const &user) const {
		user.set_nickname(cmd.get_args()[1]);
		if (user.get_username().c_str() != "" && user.get_realname().c_str() != "")
		user.get_registered_status() = true;
	};

	void Nick::is_valid_args(Server const *Server, User const &user) const {
		// Possible Error : ERR_NONICKNAMEGIVEN  ERR_ERRONEUSNICKNAME ERR_NICKNAMEINUSE ERR_NICKCOLLISION
		if (this->_args.size() < 1)
			throw error("No nickname given", ERR_NONICKNAMEGIVEN);
		if (is_nickname_valid() == false)
			throw error("Invalid nickname", ERR_ERRONEUSNICKNAME);

		// if (Server->find_nickname(this->_args[1]) != NULL && Server->find_nickname(this->_args[1]).get_socket() != NULL) Possible integration
		if (Server->find_nickname(_args[1]) != user.end())
			throw error("Nickname already in use", ERR_NICKNAMEINUSE);
	}
