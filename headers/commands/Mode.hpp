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
			void	_valid_chanMode(User const &author);
			void	_exec_chanMode(User &author);
			bool	_is_valid_mode(string const &arg, string const &modes, string &pos) const;
			bool	_is_valid_flag_mode(string const &arg) const;
			bool 	_is_valid_arg_nb(string const &modes);
			void	_check_modes(string const &arg) const;

			void	_return_channel_modes(User const &author);
			void	_build_return_message(Channel *channel, User &user);

			bool	_sign_handler(const char new_sign);

			// With args
			void	_channel_mode_o(Channel *channel, vector_string::const_iterator nickname,const User &author); // User
			void	_channel_mode_l(Channel *channel, vector_string::const_iterator nickname,const User &author); // limit -> ONLY  if (is_positive == true)
			void	_channel_mode_b(size_t pos, Channel *channel, vector_string::const_iterator arg, const User &user); // ban mask
			void	_channel_mode_k(Channel *channel, vector_string::const_iterator arg,const User &author); // channel key : password
			// Mute or not mute
			void	_channel_mode_v(Channel *channel, vector_string::const_iterator arg,const User &author);
			//
			//// No args
			void	_channel_mode_t(Channel *channel, vector_string::const_iterator arg,const User &author);
			void	_channel_mode_n(Channel *channel, vector_string::const_iterator arg,const User &author);
			void	_channel_mode_m(Channel *channel, vector_string::const_iterator arg,const User &author);
			//
			// // flag
			void	_channel_mode_p(Channel *channel, vector_string::const_iterator arg,const User &author);
			void	_channel_mode_s(Channel *channel, vector_string::const_iterator arg,const User &author);
			void	_channel_mode_i(Channel *channel, vector_string::const_iterator arg,const User &author);

		public:
			void	exec_cmd(User &user);
			bool	is_valid_args(User const &user);
	};
}
