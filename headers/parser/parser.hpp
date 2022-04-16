#pragma once
#include "../irc.hpp"
#include "../../headers/structure/Server.hpp"
#include "../error/error.hpp"
#include "../commands/command.hpp"


namespace irc {
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
			virtual ~parser();
			parser(string const &line, map_cmd &commands);
	/* Operators */
		private:
			parser		&operator=(parser const &src);
		public:
	/* Getters & Setters */
		private:
		public:
			command const		*get_command() const;
	/* Functions */
		private:
			void				_fill_command();
			map_citerator_cmd	_get_command() const;
			string				_get_arg(string::const_iterator &it) const;
		public:
	};
}
