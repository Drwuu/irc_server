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
		ret = ":" + this->get_nickname() + " NOTICE " + user->get_nickname() + " : ";
		_args.push_back(tmp.substr(prev));
		if (_args[2] == "PRIVMSG"){
			ret += "PRIVMSG Parameters: <receiver>{,<receiver>} <text to be sent>\r\n";
		}
		else if (_args[2] == "JOIN"){
			ret += "JOIN Parameters: <channel>{,<channel>} [<key>{,<key>}]\r\n";
		}
		else if (_args[2] == "KICK"){
			ret += "KICK Parameters: <channel>{,<channel>} <user>{,<user>} [<comment>]\r\n";
		}
		else if (_args[2] == "MODE"){
			ret += "MODE Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]\r\n";
		}
		else if (_args[2] == "USER"){
			ret += "USER Parameters: <username> <mode> <hostname> <realname>\r\n";
		}
		else if (_args[2] == "NICK"){
			ret += "NICK Parameters: <nickname>\r\n";
		}
		else if (_args[2] == "PART"){
			ret += "PART Parameters: <channel>{,<channel>}\r\n";
		}
		else if (_args[2] == "QUIT"){
			ret += "QUIT Parameters: [<Quit message>]\r\n";
		}
		else if (_args[2] == "NOTICE"){
			ret += "NOTICE Parameters: <nickname> <text>\r\n";
		}
		else if (_args[2] == "INVITE"){
			ret += "INVITE Parameters: <nickname> <channel>\r\n";
		}
		else if (_args[2] == "PING"){
			ret += "PING Parameters: <server1> [<server2>]\r\n";
		}
		else if (_args[2] == "PONG"){
			ret += "PONG Parameters: <daemon> [<daemon2>]\r\n";
		}
		else
			ret += _args[2]  + ": Available Commands: PRIVMSG JOIN MODE KICK INVITE USER NICK NOTICE PART QUIT PING PONG\r\n";
		Proxy_queue::Write * new_msg = new Proxy_queue::Write(user->get_socket()->get_fd(),ret.c_str());
		_server->get_event_list().push_back(new_msg);
	}

	void Help::send_message(std::string msg, User & user){
		this->receive_message(&user,msg);
	}
/* Operators */
/* Functions */
}
