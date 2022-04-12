#pragma once
#include "../irc.hpp"
#include "../server.hpp"

namespace irc {
	class parser {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			string		_line;
			map const	*_commands;
		public:

	/* Constructors & Destructors */
		private:
			parser();
			parser(parser const &src);
		public:
			virtual ~parser();
			parser(string const &line, map const *commands);
	/* Operators */
		private:
			parser		&operator=(parser const &src);
		public:
	/* Getters & Setters */
		private:
		public:
	/* Functions */
		private:
			string		_get_cmd_name() const;
		public:
			command		*get_command() const;
	};
}
