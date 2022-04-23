
#pragma once

#include "command.hpp"

namespace irc {

	class Server;
	class Pong : public command {
		private:
			Pong	&operator=(const Pong &source);
		public:
			Pong(const Pong &source);
			Pong();
			Pong(Server *server);
			virtual~Pong();

			void			exec_cmd(User &user);
			bool			is_valid_args(User const &user);
	};

}
