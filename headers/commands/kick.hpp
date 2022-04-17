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
			virtual ~kick();
	/* Operators */
		private:
			kick &operator=(kick const &src);
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(command const &cmd, User const &user) const;
			void	is_valid_args(Server const *Server, User const &user) const;
	};
}
