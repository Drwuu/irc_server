#pragma once

#include "command.hpp"

namespace irc {

	class Server;
	class Pass : public command {
		private:
			Pass	&operator=(const Pass &source);
			void	is_authentified();
		public:
			Pass(const Pass &source);
			Pass();
			Pass(Server *server);
			virtual~Pass();

			void			exec_cmd(User &user);
			bool			is_valid_args(User const &user);
			virtual void	check_auth(const User &user);
	};

}
