#include "../../headers/parser/parser.hpp"

namespace irc {
/* Constructors & Destructors */
	parser::~parser() {};
	parser::parser(string const &line, map_cmd &commands): _line(line), _commands(commands) {
		_fill_command();
	};
/* Operators */
/* Getters & Setters */
	command &parser::get_command() const {
		return *_command;
	};
/* Functions */
	void	parser::_fill_command() {
		vector_args args;
		_command = _find_command()->second;
		string::const_iterator it = _line.begin();
		if (*it == '/')
			it++;
		for (; it != _line.end(); it++) {
			args.push_back(_get_arg(it));
			if (it == _line.end())
				break;
		}
		_command->set_args(args);
	};
	map_citerator_cmd parser::_find_command() const {
		string::const_iterator it = _line.begin();
		if (*it == '/')
			it++;
		string cmd_name = _get_arg(it);
		if (cmd_name.empty())
			throw error("Empty command", ERR_UNKNOWNCOMMAND);
		map_citerator_cmd cmd_it = _commands.find(cmd_name);
		if (_commands.find(cmd_name) == _commands.end())
			throw error(cmd_name + ": Unknown command", ERR_UNKNOWNCOMMAND);
		return cmd_it;
	}
	string	parser::_get_arg(string::const_iterator &it) const {
		string mstr;
		string insensitive_line;
		std::locale loc;
		for (; it != _line.end(); it++) {
			if (*it == ' ')
				break;
			insensitive_line.push_back(std::toupper(*it, loc));
		}
		if (insensitive_line.empty())
			return "";
		for (string::iterator it2 = insensitive_line.begin(); it2 != insensitive_line.end(); it2++)
			mstr.push_back(*it2);
		return mstr;
	}
}
