#include "../headers/parser.hpp"

namespace irc {
/* Constructors & Destructors */
	parser::~parser() {};
	parser::parser(): _line(NULL) {};
	parser::parser(string const &line): _line(line) {};
	parser::parser(parser const &src): _line(NULL) {
		*this = src;
	};

/* Operators */
	parser &parser::operator=(parser const &src) {
		_line = src._line;
		return *this;
	};
/* Functions */
	string parser::get_command() const {
		string mstr;
		if (_line.empty())
			return string("Error(" ERR_UNKNOWNCOMMAND "): " "NULL" " Unknown command" );
		size_t i = 0;
		if (_line[0] == '/') i++;
		while (_line[i] && _line[i] != ' ')
			mstr.push_back(_line[i++]);
		if (!_is_valid_command(mstr))
			return string("Error(" ERR_UNKNOWNCOMMAND "): " + mstr + " Unknown command" );
		return mstr;
	};

	bool parser::_is_valid_command(string &cmd) const {
		string insensitive_cmd;
		std::locale loc;
		for (size_t i = 0; i < cmd.size(); i++)
			insensitive_cmd.push_back(std::toupper(cmd[i], loc));
		if (std::strcmp(insensitive_cmd.c_str(), "ADMIN") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "AKILL") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "AWAY") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "CLEANDEAD") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "CLEARAKILLS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "CONNECT") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "DIE") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "EXPBAN") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "GLOBOPS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "HELP") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "IMPORTMOTD") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "INFO") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "INVITE") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "ISBANNED") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "ISON") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "JOIN") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "KICK") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "KILL") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "KILLBAN") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "KLINE") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "LINKS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "LIST") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "LUSERS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "ME") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "MODE") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "MOTD") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "MSG") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "NAMES") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "NICK") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "NOTICE") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "OPER") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "PART") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "PASS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "PRIVMSG") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "QUERY") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "QUIT") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "RAKILL") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "REHASH") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "SHUN") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "SILENCE") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "SQUIT") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "STATS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "SUMMON") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "TIME") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "TOPIC") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "UNBAN") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "UNKLINE") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "USERHOST") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "USERS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "VERSION") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "WALL") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "WALLOPS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "WHO") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "WHOIS") == 0)
			return true;
		if (std::strcmp(insensitive_cmd.c_str(), "WHOWAS") == 0)
			return true;
		return false;
	};

}
