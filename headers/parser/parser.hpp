#pragma once
#include "../error/error.hpp"

namespace irc {
	class command;
	class parser {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			string			_line;
			map_cmd			&_commands;
			command			*_command;
		public:
	/* Constructors & Destructors */
		private:
			parser();
			parser(parser const &src);
		public:
			parser(string const &line, map_cmd &commands);
			virtual ~parser();
	/* Operators */
		private:
		public:
			parser		&operator=(parser const &src);
	/* Getters & Setters */
		private:
		public:
			command				&get_command() const;
	/* Functions */
		private:
			void				_fill_command();
			map_citerator_cmd	_find_command() const;
			string				_get_arg(string::const_iterator &it) const;
		public:
	};
}
#include "../commands/command.hpp"
