#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class Nick: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			Nick(Nick const &src);
		public:
			Nick();
			virtual ~Nick();
	/* Operators */
		private:
			Nick &operator=(Nick const &src);
		public:
	/* Functions */
		private:
			bool 	is_nickname_valid();
			bool	is_special_char(int c);
		public:
			void	exec_cmd(command const &cmd,  User const &user) const;
			void	is_valid_args(Server const *Server, User const &user) const;
	};
}
