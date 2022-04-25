#include "../../headers/commands/Nick.hpp"

namespace irc {
/* Constructors & Destructors */
	Nick::~Nick() {};
	Nick::Nick(){};
	Nick::Nick(Server *server): command(server) {};
/* Operators */
/* Functions */
	bool Nick::is_nickname_valid() const{
			string::const_iterator it = _args[1].begin();
			if (!(*it >= 65 && *it <= 125))
				return false;
			it++;
			for (;it != _args[1].end();++it)
			{
				if ((*it < 48 && *it != 45) or (*it > 57 and *it < 65) or (*it > 125)) {
					return false;
				}
			}
			return true;
	}
	void User_cmd::send_connection_rpl(User &user) {
		string ret;
		ret = ":" + user.get_server()->get_name() + " 001 " +user.get_nickname() + " :" + "Welcome to our 42Lyon IRC network " + user.get_nickname() + "!" + user.get_username() + "@" + user.get_hostname() + "\n"
			+ ":" + user.get_server()->get_name() + " 002 " + user.get_nickname() + " :" + "Your host is " + user.get_server()->get_name() + ", running version 1.0 " + "\n"
			+ ":" + user.get_server()->get_name() + " 003 " + user.get_nickname() + " :" + "This server was created 00:20:08 Apr 21 2022\n"
			+ ":" + user.get_server()->get_name() + " 004 " + user.get_nickname() + " " + user.get_server()->get_name() + " 1.0 BDHILRSTWcdghikorswxz ABCDEFIJKLMNOPQRSTUWXYZbcdefhijklmnoprstuvwz BEFIJLWXYZbdefhjklovw\r\n";
		Proxy_queue::Write * msg = new Proxy_queue::Write(user.get_socket()->get_fd(),ret.c_str());
		_server->get_event_list().push_back(msg);
	}
	void	Nick::exec_cmd(User &user) {
		user.set_nickname(_args[1]);
		if (user.get_registered_status() == false and !user.get_username().empty())
		{
			user.set_registered_status(true);
			user.set_uuid();
			send_connection_rpl(user);
		}
	};

	bool Nick::is_valid_args(User const &) {
		// Possible Error : ERR_NONICKNAMEGIVEN  ERR_ERRONEUSNICKNAME ERR_NICKNAMEINUSE ERR_NICKCOLLISION
		if (this->_args.size() < 1)
			throw error("No nickname given", ERR_NONICKNAMEGIVEN);
		if (is_nickname_valid() == false)
			throw error("Invalid nickname", ERR_ERRONEUSNICKNAME);

		irc::vec_user userlist = _server->get_user_list();
		if (_server->find_nickname(_args[1],userlist) != userlist.end())
		{
			throw error("Nickname already in use", ERR_NICKNAMEINUSE);
		return true;
	}
}
