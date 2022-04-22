
#include "command.hpp"

#ifndef PASS_HPP
# define PASS_HPP

namespace irc {

	class Pass : public command {
		private:
			Pass	&operator=(const Pass &source);
			void	is_authentified();
		public:
			Pass(const Pass &source);
			Pass();
			Pass(Server *server);
			virtual~Pass();

			void	exec_cmd(User &user);
			void	is_valid_args(Server const *Server, User const &user);
	};

}
#endif
