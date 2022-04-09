#pragma once
#include <iostream>
#include "../errors/error_defines.hpp"

namespace irc {
	class parser {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			string	_line;
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
			parser &operator=(parser const &src);
	/* Functions */
		private:
			bool _is_valid_command(string &cmd) const;
		public:
			string get_command() const;
	};
}
