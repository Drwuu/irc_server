#pragma once
#include "command.hpp"

namespace irc {
	class kick: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			kick(kick const &src);
		public:
			kick();
			kick(Server *server);
			virtual ~kick();
	/* Operators */
		private:
			kick &operator=(kick const &src);
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(User &user);
			bool	is_valid_args(User const &user);
	};
}
