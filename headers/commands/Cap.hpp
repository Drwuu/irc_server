
#pragma once
# include "command.hpp"

namespace irc {

	class Cap : public command {
		private:
			Cap(const Cap &source);

		public:
			Cap();
			Cap(Server *server);
			virtual ~Cap();

			bool	is_valid_args(User const &user);
			void	exec_cmd(User &user);
	};

}
