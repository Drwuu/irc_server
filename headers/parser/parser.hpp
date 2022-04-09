#pragma once
#include "../irc.hpp"

namespace irc {
	class parser {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			string	_line;
			map		_map;
		public:

	/* Constructors & Destructors */
		private:
			parser();
		public:
			virtual ~parser();
			parser(string const &line);
			parser(parser const &src);
	/* Operators */
		private:
		public:
			parser		&operator=(parser const &src);
	/* Getters & Setters */
		private:
		public:
			map const	&get_map();
	/* Functions */
		private:
		public:
			string		get_first_arg();
			command		*get_command(string const &first_arg);
	};
}
