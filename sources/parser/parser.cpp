#include "../../headers/parser/parser.hpp"
#include "../../headers/commands/admin.hpp"

namespace irc {
/* Constructors & Destructors */
	parser::~parser() {
		_map.clear();
	};
	parser::parser(): _line(NULL), _map() {};
	parser::parser(string const &line): _line(line){
		_map.insert(std::make_pair("ADMIN", new admin("ADMIN")));
		// _map.insert(std::make_pair("AWAY", admin()));
		// _map.insert(std::make_pair("HELP", admin()));
		// _map.insert(std::make_pair("INFO", admin()));
		// _map.insert(std::make_pair("INVITE", admin()));
		// _map.insert(std::make_pair("ISON", admin()));
		// _map.insert(std::make_pair("JOIN", admin()));
		// _map.insert(std::make_pair("KICK", admin()));
		// _map.insert(std::make_pair("LINKS", admin()));
		// _map.insert(std::make_pair("LIST", admin()));
		// _map.insert(std::make_pair("LUSERS", admin()));
		// _map.insert(std::make_pair("ME", admin()));
		// _map.insert(std::make_pair("MODE", admin()));
		// _map.insert(std::make_pair("MOTD", admin()));
		// _map.insert(std::make_pair("NAMES", admin()));
		// _map.insert(std::make_pair("NICK", admin()));
		// _map.insert(std::make_pair("NOTICE", admin()));
		// _map.insert(std::make_pair("OPER", admin()));
		// _map.insert(std::make_pair("PART", admin()));
		// _map.insert(std::make_pair("PRIVMSG", admin()));
		// _map.insert(std::make_pair("QUERY", admin()));
		// _map.insert(std::make_pair("QUIT", admin()));
		// _map.insert(std::make_pair("STATS", admin()));
		// _map.insert(std::make_pair("SUMMON", admin()));
		// _map.insert(std::make_pair("TIME", admin()));
		// _map.insert(std::make_pair("TOPIC", admin()));
		// _map.insert(std::make_pair("USERHOST", admin()));
		// _map.insert(std::make_pair("USERS", admin()));
		// _map.insert(std::make_pair("VERSION", admin()));
		// _map.insert(std::make_pair("WHO", admin()));
		// _map.insert(std::make_pair("WHOIS", admin()));
		// _map.insert(std::make_pair("WHOWAS", admin()));
	};
	parser::parser(parser const &src): _line(NULL) {
		*this = src;
	};

/* Operators */
	parser &parser::operator=(parser const &src) {
		_line = src._line;
		return *this;
	};
/* Getters & Setters */
	map const &parser::get_map() {
		return _map;
	};

/* Functions */
	command *parser::get_command() {
		string mstr;
		if (_line.empty())
			return NULL;
		size_t i = 0;
		if (_line[0] == '/') i++;
		while (_line[i] && _line[i] != ' ')
			mstr.push_back(_line[i++]);

		string insensitive_cmd;
		std::locale loc;
		for (size_t i = 0; i < mstr.size(); i++)
			insensitive_cmd.push_back(std::toupper(mstr[i], loc));
		citerator it = _map.find(insensitive_cmd);
		if (it == _map.end())
			return NULL;
		else
			return it->second;
	};
}
