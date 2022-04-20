#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class Privmsg: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			Privmsg(Privmsg const &src);
		public:
			Privmsg();
			virtual ~Privmsg();
	/* Operators */
		private:
			Privmsg &operator=(Privmsg const &src);
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(command const &cmd,  User const &user) const;
			void	is_valid_args(Server const *Server, User const &user) const;
	};
}
