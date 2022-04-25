#pragma once
#include "command.hpp"
#include <utility>

namespace irc {
	class Server;
	class User;
	class Channel;
	class Kick: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			std::vector<std::string>	_channel;
			std::vector<std::string>	_nickname;
			std::vector<std::pair<User *,Channel *> > _chan_user_pair;
		public:
	/* Constructors & Destructors */
		private:
			Kick(Kick const &src);
		public:
			Kick();
			Kick(Server *server);
			virtual ~Kick();
	/* Operators */
		private:
			Kick &operator=(Kick const &src);
		public:
	/* Functions */
		private:
			void	split_arguments();
		public:
			bool	is_valid_args(User const &user);
			void	exec_cmd(User &user);
	};
}
