#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class Notice: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			Notice(Notice const &src);
		public:
			Notice();
			Notice(Server *server);
			virtual ~Notice();
	/* Operators */
		private:
			Notice &operator=(Notice const &src);
		public:
	/* Functions */
		private:
			bool	find_receiver(Server const *server,const std::string receiver);
			bool	is_valid_nickname(const std::string &nickname) const;
		public:
			void			exec_cmd(User &user);
			virtual bool	is_valid_args(User const &user);
	};
}
