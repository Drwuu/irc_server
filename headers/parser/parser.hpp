#pragma once
#include "../irc.hpp"
#include "../server.hpp"
#include "../error/error.hpp"
#include "../commands/command.hpp"


namespace irc {
	class parser {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			string	_line;
			map_cmd	&_commands;
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
	/* Functions */
		private:
			string					_get_arg(string::const_iterator &it) const;
			map_citerator_cmd const	_get_command() const;
		public:
			void					fill_command() const;
	};
}
