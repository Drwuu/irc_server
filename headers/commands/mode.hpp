#pragma once
#include "command.hpp"
#include <ratio>
#include <sys/_types/_size_t.h>

namespace irc {
	class Mode: public command {
	/* Typedefs */
		private:
		public:
	/* Nested Classes */
		private:
		public:
			// struct Channel {

			// };
			// struct User {

			// };
	/* Variables */
		private:
				bool	_give_privilege;
		public:
	/* Constructors & Destructors */
		private:
			Mode(Mode const &src);
		public:
			Mode();
			Mode(Server *server);
			virtual ~Mode();
	/* Operators */
		private:
			Mode &operator=(Mode const &src);
		public:
	/* Functions */
		private:
			void	_valid_chanMode();
			void	_exec_chanMode();
			void	_valid_userMode();
			void	_exec_userMode();
			bool	_is_valid_mode(string const &arg, string const &modes, string &pos) const;
			bool	_is_valid_flag_mode(string const &arg) const;
			void	_check_modes(string const &arg) const;
		public:
			void	exec_cmd(User &user);
			bool	is_valid_args(User const &user);
	};
}
