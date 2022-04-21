#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class Privmsg: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			Privmsg(Privmsg const &src);
		public:
			Privmsg();
			Privmsg(Server *server);
			virtual ~Privmsg();
	/* Operators */
		private:
			Privmsg &operator=(Privmsg const &src);
		public:
	/* Functions */
		private:
			bool	find_receiver(Server const *server,const std::string receiver) const;
			bool	is_valid_channel(const std::string &channel) const;
			bool	is_valid_nickname(const std::string &nickname) const;
			bool	is_valid_receiver(const std::string &receiver) const;
			bool	is_authorized(const Channel & channel, const User & user) const;
		public:
			void	exec_cmd(User &user);
			void	is_valid_args(Server const *Server, User const &user) const;
	};
}
