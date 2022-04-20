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
			bool 	is_nickname_valid() const;
		public:
			void	exec_cmd(command const &cmd,  User &user) const;
			void	is_valid_args(Server const *Server, User const &user) const;
	};
}
