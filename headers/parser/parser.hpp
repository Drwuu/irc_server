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
			string			_line;
			map_cmd const	*_commands;
		public:

	/* Constructors & Destructors */
		private:
			parser();
			parser(parser const &src);
		public:
			virtual ~parser();
			parser(string const &line, map_cmd const *commands);
	/* Operators */
		private:
			parser		&operator=(parser const &src);
		public:
	/* Getters & Setters */
		private:
		public:
	/* Functions */
		private:
			string		_get_arg(string::const_iterator &it) const;
			bool		_is_valid_cmd(map_citerator_cmd const &cmd, vector_args const &args) const;
		public:
			vector_args	get_command_infos() const;
	};
}
