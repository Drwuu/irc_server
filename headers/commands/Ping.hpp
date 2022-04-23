#pragma once

#include "command.hpp"

namespace irc {

	class Server;
	class Ping : public command {
		private:
			Ping	&operator=(const Ping &source);
		public:
			Ping(const Ping &source);
			Ping();
			Ping(Server *server);
			virtual~Ping();

			void			exec_cmd(User &user);
			bool			is_valid_args(User const &user);
	};

}
