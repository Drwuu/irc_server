#include "../headers/Help.hpp"
#include "../headers/irc.hpp"

namespace irc {
/* Constructors & Destructors */
	Help::~Help() {};
	Help::Help():User(){};
	Help::Help(Server *server):User(){
		this->_server = server;
	};

	void Help::receive_message(User *user, std::string msg)
	{
		std::string ret;
		std::vector<std::string> _args;
		std::string tmp = msg;
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = tmp.find(' ', prev)) != std::string::npos) {
			_args.push_back(tmp.substr(prev, pos - prev));
			prev = pos + 1;
		}
		_args.push_back(tmp.substr(prev));
		for(std::vector<std::string>::const_iterator it = _args.begin(); it != _args.end();it++)
			std::clog << "msg = " << *it << " |ID = " << it - _args.begin() << std::endl;
		if (_args[2] == "PRIVMSG")
		{
			ret = ":" + this->get_nickname() + " NOTICE " + user->get_nickname() + " : PRIVMSG Parameters: <receiver>{,<receiver>} <text to be sent>\r\n";
		}
		Proxy_queue::Write * new_msg = new Proxy_queue::Write(user->get_socket()->get_fd(),ret.c_str());
		_server->get_event_list().push_back(new_msg);
	}

	void Help::send_message(std::string msg, User & user){
		this->receive_message(&user,msg);
	}
/* Operators */
/* Functions */
}
