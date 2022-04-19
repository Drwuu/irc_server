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
		public:
	/* Constructors & Destructors */
		private:
		public:
			parser();
			parser(parser const &src);
			virtual ~parser();
	/* Operators */
		private:
		public:
			parser				&operator=(parser const &src);
	/* Getters & Setters */
		private:
		public:
	/* Functions */
		private:
			void				_skip_param(string const &line, string::const_iterator &it, char const separator) const;
		public:
			string const		get_user(string const &line) const;
			map_citerator_cmd	get_command(string const &line, map_cmd const &_commands) const;
			vector_string const	get_args(string const &line) const;
	};
}
#include "../commands/command.hpp"
