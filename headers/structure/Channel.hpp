#pragma once
#include "Server.hpp"
#include <sys/_types/_size_t.h>

namespace irc {
	class User;
	class Channel{
		private:
			std::string					_name;
			std::string					_topic;
			std::string					_prefix;
			std::string					_key;
			int							_lifetime;
			int							_opdelay;
			size_t						_userlimit;
			bool						_is_private;
			bool						_is_secret;
			bool						_is_invite_only;
			bool						_is_topic_chop_only;
			bool						_is_no_external_msg;
			bool						_is_moderated;
			bool						_is_limited;
			vector_string 				_ban_list; // Needed for ISBANNED function
			std::vector<User *>			_user_list;
			std::vector<User *> 		_invite_list;
			std::vector<User *> 		_operator_list;
			Channel(Channel const & copy);
		protected:
			/*Arg*/
		public:
			Channel();
			Channel(std::string name);
			Channel(std::string name, char *prefix);
			Channel(std::string name, char *prefix, std::string key);
			~Channel();
			const std::string&			get_name() const;
			const std::string&			get_prefix() const;
			const std::string&			get_topic() const;
			const std::string&			get_key() const;
			const size_t&				get_userlimit() const;
			const std::vector<User *>	get_user_list() const;
			const vector_string			get_banned_user() const; // Needed for ISBANNED function
			const std::vector<User *>	get_invite_list() const;
			const std::vector<User *>	get_operator_list() const;
			bool						get_private() const;
			bool						is_private() const;
			bool						is_secret() const;
			bool						is_invite() const;
			bool						is_invite(const User *user) const;
			bool						is_invite(std::string nickname) const;
			bool						is_topic() const;
			bool						is_no_external_msg() const;
			bool						is_moderated() const;
			bool						is_limited() const;
			bool						is_operator(const User *user) const;

			User *						find_user(std::string nickname);

			void						set_name(const std::string& name);
			void						set_prefix(const std::string& prefix);
			void						set_topic(const std::string& topic); // Use for TOPIC function
			void						set_key(const std::string& key);
			void						set_mode(const std::string& mode);
			void						set_flag(const std::string& flag);
			void						set_lifetime(const int& lifetime);
			void						set_opdelay(const int& opdelay);
			void						set_userlimit(const int& userlimit);
			void						set_channel_identifier(const std::string& channel_identifier);

			void						set_external_msg(bool value);
			void						set_moderated(bool value);
			void						set_private(bool value);
			void						set_secret(bool value);
			void						set_invite_only(bool value);
			void						set_topic_chanop_only(bool value);

			void						add_user(User * user);
			void						add_operator(User * user);
			void						del_operator(User * user);
			void						del_user(User * user);
			void						ban_user(std::string nick);
			void						unban_user(std::string nick);
			void						invite_user(User * user);
			void						op_user(std::string nick);
			void						transmit_message(std::string msg, User * user);
	};
}
