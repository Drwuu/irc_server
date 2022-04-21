#pragma once
#include "../structure/Server.hpp"

namespace irc {
	class Server;
	class command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		protected:
			vector_string	_args;
			Server			*_server;
		public:
	/* Constructors & Destructors */
		private:
			command(command const &src);
		public:
			command();
			command(Server *server);
			virtual ~command();
	/* Operators */
		private:
			command& operator=(command const &src);
		public:
	/* Getters */
		private:
		public:
			vector_string const	&get_args() const;
	/* Setters */
		private:
		public:
			void				set_args(vector_string const &args);
	/* Functions */
		private:
		public:
			virtual void		is_valid_args(Server const *Server, User const &user) const = 0;
			virtual void		exec_cmd(User &user) = 0;
	};
}
#include "../commands/invite.hpp"
#include "../commands/kick.hpp"
#include "../commands/mode.hpp"
#include "../commands/Join.hpp"
#include "../commands/Nick.hpp"
#include "../commands/Message.hpp"
#include "../commands/User_cmd.hpp"
