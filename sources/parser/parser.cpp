#include "../../headers/parser/parser.hpp"
#include "../../headers/error/error.hpp"

namespace irc {
/* Constructors & Destructors */
	parser::~parser() {};
	parser::parser(): _line(nullptr) {};
	parser::parser(string const &line, map const *commands): _line(line), _commands(commands) {};
/* Operators */
/* Getters & Setters */
/* Functions */
	string	parser::_get_cmd_name() const {
		string mstr;
		if (_line.empty())
			return NULL;
		size_t i = 0;
		if (_line[0] == '/') i++;
		while (_line[i] && _line[i] != ' ')
			mstr.push_back(_line[i++]);
		return mstr;
	}
	command	*parser::get_command() const {
		string cmd_name = _get_cmd_name();
		string insensitive_cmd;
		std::locale loc;
		for (size_t i = 0; i < cmd_name.size(); i++)
			insensitive_cmd.push_back(std::toupper(cmd_name[i], loc));
		citerator it = _commands->find(insensitive_cmd);
		if (it == _commands->end())
			throw error(cmd_name, ERR_UNKNOWNCOMMAND);
		else
			return it->second;
	};
}
