#include "../../headers/parser/parser.hpp"
#include <sys/_types/_size_t.h>

namespace irc {
/* Constructors & Destructors */
	parser::~parser() {};
	parser::parser() {};
	parser::parser(parser const &src) { *this = src; };
/* Operators */
/* Getters & Setters */
	parser &parser::	operator=(parser const &src) {
		(void)src;
		return *this;
	};

/* Functions public */
	/*
	*	*RESUME*
	*	get_user: return empty string if no ':' at [0] or the user passed in the line
	*/
	string const parser::			get_user(string const &line) const {
		string user;
		string::const_iterator it = line.begin();
		if (it[0] != ':')
			return "";
		it++;
		for (; it != line.end(); it++) {
			if (*it == ' ') {
				while (*it && *it == ' ')
					it++;														// line parameters can be separated by multiple spaces
				break ;
			}
			user.push_back(*it);
		}
		return user;
	};

	/*
	*	*RESUME*
	*	get_command: return the command in the map if found or map end() if not
	*/
	map_citerator_cmd parser::		get_command(string const &line, map_cmd const &_commands) const {
		string cmd;
		string::const_iterator it = line.begin();
		if (it[0] == ':')
			_skip_param(line, it, ' ');											// skip user
		for (; it != line.end(); it++) {
			if (*it == ' ')
				break ;
			cmd.push_back(*it);
		}
		map_citerator_cmd cmd_it = _commands.find(cmd);
		if (cmd_it == _commands.end())
			throw error(cmd + " :Unknown command", ERR_UNKNOWNCOMMAND);
		return cmd_it;
	};

	/*
	*	*RESUME*
	*	get_args: return vector of args -including cmd name- after skiped user
	*/
	vector_string const parser::	get_args(string const &line) const {
		vector_string vargs;
		string arg;
		string::const_iterator it = line.begin();
		if (it[0] == ':')
			_skip_param(line, it, ' ');											// skip user
		for (; it != line.end(); it++) {
			if (*it == ':') {													// special case ':' arg
				while (*it)
					arg.push_back(*it++);
				vargs.push_back(arg);
				arg.clear();
				break ;
			}
			if (*it == ' ' || it == (line.end()-1)) {
				if (it == (line.end()-1)) {										// special case end of args
					arg.push_back(*it);
					vargs.push_back(arg);
					arg.clear();
					break ;
				}
				vargs.push_back(arg);
				arg.clear();
				continue ;
			}
			arg.push_back(*it);
		}
		return vargs;
	};

/* Functions private */
	/*
	*	*RESUME*
	*	_skip_param: advance pointer of the iterator one step after the separator
	*/
	void parser::					_skip_param(string const &line, string::const_iterator &it, char const separator) const {
		for (; it != line.end(); it++) {
			if (*it == separator) {
				it++;
				break ;
			}
		}
	};

	std::list<std::string> const parser::	split_command(string const &line) const {
		std::list<std::string>	cmd_list;

		for (std::string::size_type i = 0, k = 0 ; i != line.npos && i < line.size() && k < line.size() && k != line.npos ; i = k + 1) {
			k = line.find("\n", i);
			if (k == line.npos)
				k = line.size();
			std::string	str(line.substr(i, k - i));
			cmd_list.push_back(str);
		}
		return cmd_list;
	}
}
