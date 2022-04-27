#include "../../headers/structure/Channel.hpp"
#include <sys/_types/_size_t.h>

namespace irc {

	Channel::Channel(){}

	Channel::Channel(std::string name): _name(name){}

	Channel::Channel(std::string name, char *prefix)
		: _name(name), _prefix(prefix), _userlimit(10), _is_limited(false) {}

	Channel::Channel(std::string name, char *prefix, std::string key)
		: _name(name), _prefix(prefix), _key(key), _userlimit(10), _is_limited(false) {}

	Channel::Channel(Channel const & copy){*this = copy;}

	Channel::~Channel(){}


	const std::string& 				Channel::get_name() const{ return (this->_name);}
	const std::string& 				Channel::get_prefix() const{ return (this->_prefix);}
	const std::string& 				Channel::get_key() const { return _key; };
	const size_t & 					Channel::get_userlimit() const{ return (this->_userlimit);}
	const std::vector<User *> 		Channel::get_user_list() const{ return (this->_user_list);}
	const vector_string 			Channel::get_banned_user() const{ return (this->_ban_list);}
	const std::vector<std::string> 	Channel::get_invite_list() const{ return (this->_invite_list);}
	const std::vector<User *> 		Channel::get_operator_list() const{ return (this->_operator_list);}


	bool Channel::is_private() const{ return (this->_is_private);}
	bool Channel::is_secret() const{ return (this->_is_secret);}
	bool Channel::is_invite() const{ return (this->_is_invite_only);}
	bool Channel::is_invite(std::string const &nickname) const{
		return std::find(_invite_list.begin(), _invite_list.end(), nickname) != _invite_list.end();
	}
	bool Channel::is_topic() const{ return (this->_is_topic_chop_only);}
	bool Channel::is_no_external_msg() const{ return (this->_is_no_external_msg);}
	bool Channel::is_moderated() const{ return (this->_is_moderated);}
	bool Channel::is_limited() const { return this->_is_limited; }
	bool Channel::is_key() const { return !this->_key.empty(); }

	void Channel::del_key() { this->_key.clear(); }

	bool Channel::is_operator(const User *user) const {
		if (std::find(_operator_list.begin(), _operator_list.end(), user) == _operator_list.end())
			return false;
		return true;
	}

	User *	Channel::find_user(std::string nickname) {
		for (vec_user::iterator it = _user_list.begin() ; it < _user_list.end() ; ++it) {
			if ((*it)->get_nickname() == nickname)
				return *it;
		}
		return NULL;
	}

	void Channel::set_name(const std::string & name){ this->_name = name;}
	void Channel::set_prefix(const std::string & prefix){ this->_prefix = prefix;}
	void Channel::set_topic(const std::string &topic){ this->_topic = topic;}
	void Channel::set_key(const std::string &key){ this->_key = key;}
	void Channel::set_lifetime(const int & lifetime){ this->_lifetime = lifetime;}
	void Channel::set_opdelay(const int & opdelay){ this->_opdelay = opdelay;}
	void Channel::set_userlimit(const int & userlimit){ this->_userlimit = userlimit;}
	void Channel::set_limited(bool value){ _is_limited = value; }

	void Channel::set_private(bool value) { this->_is_private = value; }
	void Channel::set_external_msg(bool value) { this->_is_no_external_msg = value; }
	void Channel::set_moderated(bool value) { this->_is_moderated = value; }
	void Channel::set_secret(bool value) { this->_is_secret = value; }
	void Channel::set_invite_only(bool value) { this->_is_invite_only = value; }
	void Channel::set_topic_chanop_only(bool value) { this->_is_topic_chop_only = value; }

	void Channel::add_user(User * user){
		for (vector_string::iterator it = this->_ban_list.begin(); it != this->_ban_list.end();++it)
		{
			if (user->get_username() == (*it)) {
				dprintf(2, "User '%s' already banned from this channel\n", user->get_username().c_str());
				return ; // User already banned from this channel
			}
		}
		this->_user_list.push_back(user);
	}

	void Channel::add_operator(User * user) { _operator_list.push_back(user); }

	void Channel::del_operator(User * user) { _operator_list.erase(std::find(_operator_list.begin(), _operator_list.end(), user)); }

	void Channel::del_user(User * user)
	{
		for (std::vector<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end();++it)
		{
			if ((*it)->get_nickname() == user->get_nickname()){
				this->_user_list.erase(it);
				break;
			}
		}
	}

	void Channel::invite_user(std::string const &nickname) {
		_invite_list.push_back(nickname);
		//// could be usefull but how ?
		// user->receive_invite(*this);
	}

	void Channel::transmit_message(std::string msg, User * user)
	{
		for (std::vector<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end();++it)
		{
			if ((*it)->get_username() != user->get_username()){
				std::cout << "------------Transmit message to " << (*it)->get_username() << std::endl;
				(*it)->receive_message(user,msg);
			}
		}
	}

	void	Channel::ban_user(std::string nick) {
		if (std::find(_ban_list.begin(), _ban_list.end(), nick) != _ban_list.end())
			return ;
		_ban_list.push_back(nick);
	}

	void	Channel::unban_user(std::string nick) {
		vector_string::iterator		it = std::find(_ban_list.begin(), _ban_list.end(), nick);
		if (it != _ban_list.end())
			_ban_list.erase(it);
	}
}
