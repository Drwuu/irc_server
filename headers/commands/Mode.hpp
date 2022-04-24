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
			std::string		_stage;
			std::string		_modes;
			std::string		_modes_args;
			char			_sign;
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
			void	_valid_chanMode(User const &user);
			void	_exec_chanMode();
			void	_valid_userMode(User const &user);
			void	_exec_userMode();
			bool	_is_valid_mode(string const &arg, string const &modes, string &pos) const;
			bool	_is_valid_flag_mode(string const &arg) const;
			bool 	_is_valid_arg_nb(string const &modes);
			void	_check_modes(string const &arg) const;

			bool	_sign_handler(const char new_sign);

			// With args
			void	_mode_o(vector_string::const_iterator nickname,const User &user); // User
			void	_mode_l(vector_string::const_iterator nickname,const User &user); // limit -> ONLY  if (is_positive == true)
			void	_mode_b(vector_string::const_iterator arg,const User &user); // ban mask
			void	_mode_k(vector_string::const_iterator arg,const User &user); // channel key : password
			//
			//// No args
			void	_mode_t(vector_string::const_iterator arg,const User &user);
			void	_mode_n(vector_string::const_iterator arg,const User &user);
			void	_mode_m(vector_string::const_iterator arg,const User &user);
			void	_mode_v(vector_string::const_iterator arg,const User &user);
			//
			// // flag
			void	_mode_p(vector_string::const_iterator arg,const User &user);
			void	_mode_s(vector_string::const_iterator arg,const User &user);
			void	_mode_i(vector_string::const_iterator arg,const User &user);
		public:
			void	exec_cmd(User &user);
			bool	is_valid_args(User const &user);
	};
}
