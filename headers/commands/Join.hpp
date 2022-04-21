#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class Join: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			Join(Join const &src);
		public:
			Join();
			Join(Server *server);
			virtual ~Join();
	/* Operators */
		private:
			Join &operator=(Join const &src);
		public:
	/* Functions */
		private:
			vector_string	_get_instructions(string const &arg, char const separator) const;
			void			_erase_chars(string const chars, vector_string &vector) const;
		public:
			void			exec_cmd(User &user);
			void			is_valid_args(Server const *Server, User const &user) const;
	};
}
