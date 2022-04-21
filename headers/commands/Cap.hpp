
#ifndef CAP_HPP
# define CAP_HPP

# include "command.hpp"

namespace irc {

	class Cap : public command {
		private:
			Cap(const Cap &source);

		public:
			Cap();
			Cap(Server *server);
			virtual ~Cap();

			void	is_valid_args(Server const *Server, User const &user) const;
			void	exec_cmd(User &user);
	};

}

#endif
