#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class User_cmd: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			User_cmd(User_cmd const &src);
		public:
			User_cmd();
			User_cmd(Server *server);
			virtual ~User_cmd();
	/* Operators */
		private:
			User_cmd &operator=(User_cmd const &src);
		public:
	/* Functions */
		private:
			void	send_connection_rpl(User &user);
		public:
			void	exec_cmd(User &user);
			bool	is_valid_args(User const &user);
	};
}
