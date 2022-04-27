#include "../../headers/structure/User.hpp"
#include "../../headers/proxy/Server_queue.hpp"
#include <istream>
#include <vector>

namespace irc {

	Server * User::get_server() {
		return _server;
	}
	void User::set_server(Server *server) {
		_server = server;
	}
	void User::set_event_list(std::list<Socket_event *> &event_list) {
		_event_list = event_list;
	}
	std::list<Socket_event *> User::get_event_list() {
		return _event_list;
	}
	User::User() : _is_pass_checked(false) {}
	User::~User(){}
	User::User(Socket<Address_ipv6> const *socket): _is_pass_checked(false), _socket(socket){}
	ChanStatus::ChanStatus(Channel * channel):channel(channel),is_mute(false),is_operator(false){}


	int User::get_port() const{
		return (this->_port);}
	const std::string User::get_username() const{
		return (this->_username);}
	std::string const User::get_nickname() const{
		return (this->_nickname);}
	std::string const User::get_hostname() const{
		return (this->_hostname);}
	std::string const User::get_realname() const{
		return (this->_realname);}
	const std::string User::get_uuid() const{
		return (this->_uuid);}
	const std::string User::get_ip() const{
		return (this->_ip);}
	const std::string User::get_mode() const{
		return (this->_mode);}
	bool User::get_registered_status() const{
		return _is_registered;}
	bool User::get_password_status() const{
		return _is_pass_checked;}
	std::vector<std::string> User::get_past_username(){
		return (this->_past_username);}
	std::vector<ChanStatus> User::get_chan_list() {
		return (this->_chan_list);}
	const std::vector<ChanStatus> User::get_chan_list() const {
		return (this->_chan_list);}
	std::vector<ChanStatus>::const_iterator	User::get_chanstatus_from_list(Channel const & channel,std::vector<ChanStatus> const &chans)const {
		for (std::vector<ChanStatus>::const_iterator it = chans.begin(); it != chans.end();++it)
		{
			if ((*(it)).channel->get_name() == channel.get_name())
				return it;
		}
		return chans.end();
	}

	std::vector<ChanStatus>::iterator	User::get_chan_status(const Channel *channel) {
		for (std::vector<ChanStatus>::iterator it = _chan_list.begin() ; it != _chan_list.end() ; ++it)
			if (it->channel == channel)
				return it;
		return _chan_list.end();
	}

	Socket<Address_ipv6> const *User::get_socket() const{
		return (this->_socket);}

	bool User::get_operator_status(const Channel *channel) const{
		std::vector<ChanStatus>::const_iterator 	it = this->get_chanstatus_from_list(*channel, _chan_list);
		return _chan_list.end() != it && it->is_operator;
	}

	Channel		*User::find_channel(std::string const &name) const {
		for (std::vector<ChanStatus>::const_iterator it = _chan_list.begin() ;
				it != _chan_list.end() ; ++it)
			if (it->channel->get_name() == name)
				return it->channel;
		return NULL;
	}

	void User::set_ip(std::string ip) {
		this->_ip = ip;}
	void User::set_port(int port) {
		this->_port = port;}
	void User::set_uuid(){
		this->_uuid = this->_nickname + this->_username + this->_realname; }
	void User::set_username(std::string username){
		if (this->_username.c_str()){
			this->_past_username.push_back(this->_username);
		}
		this->_username = username;}
	void User::set_hostname(std::string hostname){
		this->_hostname = hostname;}
	void User::set_nickname(std::string nickname){ //nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
		this->_nickname = nickname;}
	void User::set_realname(std::string realname){
		this->_realname = realname;}
	void User::set_registered_status(bool status){
		this->_is_registered = status;}

	void User::set_password_status(bool status){
		this->_is_pass_checked = status;}
	void	User::set_socket(Socket<Address_ipv6> const *socket) {
		(void)socket; // FIXME : fix that shit. The function wasn't created.
	}
	void User::set_chan_status(const Channel *channel, bool op) {
		std::vector<ChanStatus>::iterator	it_chan = get_chan_status(channel);
		if (it_chan != _chan_list.end())
			it_chan->is_operator = op;
	};

	void	User::set_mute(const Channel *channel, bool value) {
		for (std::vector<ChanStatus>::iterator it = _chan_list.begin() ;
				it != _chan_list.end() ; ++it)
			if (it->channel == channel)
				it->is_mute = value;
	}

	void	User::join_channel(ChanStatus &status) {
		this->_chan_list.push_back(status);
	}

	void User::leave_channel(std::string channel) {
		for (std::vector<ChanStatus>::iterator it = _chan_list.begin(); it != _chan_list.end(); ++it){
			if ((*it).channel->get_name() == channel){
				(*it).channel->del_user(this);
				if ((*it).channel->get_user_list().size() == 0)
				{
					_server->del_channel(*(*it).channel);
				}
				(*it).channel = NULL;
				_chan_list.erase(it);
				return ; // Channel found
			}
		}
	}

	bool	User::is_mute(const Channel *channel) { return get_chan_status(channel)->is_mute; }

	void User::send_message(std::string msg, Channel & channel){
		channel.transmit_message(msg, this);
	}

	void User::send_message(std::string msg, User & user){
		user.receive_message(this,msg);
	}

	void User::receive_message(User * user,std::string msg){
		std::string ret = ":" + user->get_nickname() + msg + "\r\n\0";
		Proxy_queue::Write * new_msg = new Proxy_queue::Write(this->get_socket()->get_fd(),ret.c_str());
		_server->get_event_list().push_back(new_msg);
	}

	void User::send_invite(User & user, Channel & channel){
		user.receive_invite(channel);
		std::string ret = channel.get_name() + user.get_nickname() + " has been invited to join the channel\r\n";
		Server_queue::Message * new_msg = new Server_queue::Message(ret.c_str(),this->get_socket());
		_event_list.push_back(new_msg); // This is how you should do it
	}

	void User::receive_invite(Channel & channel){
		std::string ret = "You have been invited to " + channel.get_name() + "\r\n";
		Server_queue::Message * new_msg = new Server_queue::Message(ret.c_str(),this->get_socket());
		this->_event_list.push_back(new_msg);
	}

	void User::kick_user(User & user, Channel & channel,std::string msg){(void)user;(void)channel;(void)msg;}

	void User::ban_user(User & user, Channel & channel) {
		if (this->get_operator_status(&channel) == false)
			return ;
		channel.ban_user(user._nickname);
	}

	void User::unban_user(User & user,Channel & channel) {
		if (this->get_operator_status(&channel) == false)
			return ;
		channel.unban_user(user._nickname);
	}

	void User::change_topic(Channel & channel,std::string topic){(void)channel,(void)topic;}

	void User::unmute_user(User & user, Channel & channel){(void)user;(void)channel;}

	int	User::disconnect_user(string const msg_quit){
		int ret = this->_socket->get_fd();
		for (std::vector<ChanStatus>::const_iterator it = this->_chan_list.begin(); it != this->_chan_list.end(); ++it){
			(*it).channel->transmit_message(" PART " + (*it).channel->get_name() + " " + this->get_nickname(), this);
			(*it).channel->transmit_message(" QUIT " + msg_quit, this);
			(*it).channel->del_user(this);
		}
		this->_chan_list.clear();
		return ret;
	}
}
