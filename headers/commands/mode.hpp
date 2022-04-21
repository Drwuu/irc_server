#pragma once
#include "command.hpp"

namespace irc {
	class mode: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			mode(mode const &src);
		public:
			mode();
			mode(Server *server);
			virtual ~mode();
	/* Operators */
		private:
			mode &operator=(mode const &src);
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(User &user);
			void	is_valid_args(Server const *Server, User const &user);
	};
}
