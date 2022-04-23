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
			Nick(Server *server);
			virtual ~Nick();
	/* Operators */
		private:
			Nick &operator=(Nick const &src);
		public:
	/* Functions */
		private:
			bool 	is_nickname_valid() const;
		public:
			bool	is_valid_args(User const &user);
			void	exec_cmd(User &user);
	};
}
