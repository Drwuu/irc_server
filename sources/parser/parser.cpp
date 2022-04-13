#include "../../headers/parser/parser.hpp"
#include <cstdio>

namespace irc {
/* Constructors & Destructors */
	parser::~parser() {};
	parser::parser(string const &line, map_cmd &commands): _line(line), _commands(commands) {};
/* Operators */
/* Getters & Setters */
/* Functions */
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
	map_citerator_cmd const parser::_get_command() const {
		string::const_iterator it = _line.begin();
		if (*it == '/')
			it++;
		string cmd_name = _get_arg(it);
		if (cmd_name.empty())
			throw error("EMPTY COMMAND", ERR_UNKNOWNCOMMAND);
		map_citerator_cmd cmd_it = _commands.find(cmd_name);
		if (cmd_it == _commands.end())
			throw error(cmd_name, ERR_UNKNOWNCOMMAND);
		return cmd_it;
	}
	void	parser::fill_command() const {
		vector_args args;
		command &cmd = *_get_command()->second;
		string::const_iterator it = _line.begin();
		if (*it == '/')
			it++;
		for (; it != _line.end(); it++) {
			string mstr = _get_arg(it);
			args.push_back(mstr);
			if (it == _line.end())
				break;
		}
		cmd.set_args(args);
	};
}
