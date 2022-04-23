#include "../../headers/structure/Channel.hpp"
#include <sys/_types/_size_t.h>

namespace irc {
	const std::string& Channel::get_name() const{
		return (this->_name);}
	const std::string& Channel::get_prefix() const{
		return (this->_prefix);}

	const std::string& Channel::get_key() const {
		return _key;
	};

	const size_t & Channel::get_userlimit() const{
		return (this->_userlimit);}

	const std::vector<User *> Channel::get_user_list() const{
		return (this->_user_list);}

	const std::vector<User *> Channel::get_banned_user() const{
		return (this->_ban_list);}

	const std::vector<User *> Channel::get_invite_list() const{
		return (this->_invite_list);}

	const std::vector<User *> Channel::get_operator_list() const{
		return (this->_operator_list);}

	bool Channel::is_private() const{
		return (this->_is_private);}
	bool Channel::is_secret() const{
		return (this->_is_secret);}
	bool Channel::is_invite() const{
		return (this->_is_invite);}
	bool Channel::is_topic() const{
		return (this->_is_topic);}
	bool Channel::is_no_external_msg() const{
		return (this->_is_no_external_msg);}
	bool Channel::is_moderated() const{
		return (this->_is_moderated);}
	bool Channel::is_limited() const{
		return (this->_is_limited);}

	void Channel::set_name(const std::string & name){
		this->_name = name;}

	void Channel::set_prefix(const std::string & prefix){
		this->_prefix = prefix;}

	void Channel::set_topic(const std::string &topic){
		this->_topic = topic;}

	void Channel::set_lifetime(const int & lifetime){
		this->_lifetime = lifetime;}

	void Channel::set_opdelay(const int & opdelay){
		this->_opdelay = opdelay;}

	void Channel::set_userlimit(const int & userlimit){
		this->_userlimit = userlimit;}

	void Channel::add_user(User * user){
		for (std::vector<User *>::iterator it = this->_ban_list.begin(); it != this->_ban_list.end();++it)
		{
			if (user->get_username() == (*it)->get_username()) {
				dprintf(2, "User '%s' already banned from this channel\n", user->get_username().c_str());
				return ; // User already banned from this channel
			}
		}
		this->_user_list.push_back(user);
	}

	void Channel::del_user(User * user)
	{
		for (std::vector<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end();++it)
		{
			if ((*it)->get_username() == user->get_username())
				this->_user_list.erase(it);
		}
	}

	void Channel::invite_user(User * user) {
		// You need to dereference 'this' to transform it in reference.
		// Use your fucking brain.
		user->receive_invite(*this);
	}

	void Channel::transmit_message(std::string msg, User * user)
	{
		if (this->_user_list.size() < 2)
		{
			std::cout << "Channel size is " << this->_user_list.size() << std::endl;
			return;
		}
		for (std::vector<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end();++it)
		{
			std::cout << "------------Transmit message to " << (*it)->get_username() << std::endl;
			(*it)->receive_message(user,msg);
		}
	}

	Channel::Channel(){}

	Channel::Channel(std::string name): _name(name){}

	Channel::Channel(std::string name, char *prefix): _name(name), _prefix(prefix){}

	Channel::Channel(std::string name, char *prefix, std::string key): _name(name), _prefix(prefix), _key(key){}

	Channel::Channel(Channel const & copy){*this = copy;}

	Channel::~Channel(){}
}
