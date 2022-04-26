#pragma once
#include "command.hpp"

namespace irc {
	class Quit: public command {
	/* Typedefs */
		private:
			typedef std::vector<Channel *>		vec_chans;
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			Quit(Quit const &src);
		public:
			Quit();
			Quit(Server *server);
			virtual ~Quit();
	/* Operators */
		private:
			Quit &operator=(Quit const &src);
		public:
	/* Getters */
		private:
		public:
	/* Setters */
		private:
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(User &user);
			bool	is_valid_args(User const &user);
	};
}
