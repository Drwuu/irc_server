#include "../../headers/structure/User.hpp"
#include "../../headers/proxy/Server_queue.hpp"
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
	User::User(){}
	User::~User(){}
	User::User(Socket<Address_ipv6> const *socket): _socket(socket){}
	ChanStatus::ChanStatus(Channel * channel):channel(channel),is_admin(false),is_banned(false),is_mute(false),is_operator(false){}

	const std::string User::get_username() const{
		return (this->_username);}
	std::string const User::get_nickname() const{
		return (this->_nickname);}
	std::string const User::get_hostname() const{
		return (this->_hostname);}
	std::string const User::get_realname() const{
		return (this->_realname);}
	const std::string User::get_password() const{
		return (this->_password);}
	const std::string User::get_uuid() const{
		return (this->_uuid);}
	const std::string User::get_port() const{
		return (this->_port);}
	const std::string User::get_ip() const{
		return (this->_ip);}
	const std::string User::get_mode() const{
		return (this->_mode);}
	bool User::get_registered_status() const{
		return _is_registered;}
	std::vector<std::string> User::get_past_username(){
		return (this->_past_username);}
	std::vector<ChanStatus> User::get_chan_list() {
		return (this->_chan_list);}
	const std::vector<ChanStatus> User::get_chan_list() const {
		return (this->_chan_list);}
	// ChanStatus 				*User::get_chanstatus_from_list(Channel * channel) {
	// 	std::vector<ChanStatus> chans = get_chan_list();
	// 	for (std::vector<ChanStatus>::iterator it = chans.begin(); it != chans.end();++it)
	// 	{
	// 		if ((*(it)).channel->get_name() == channel->get_name())
	// 			return &(*it);
	// 	}
	// 	std::cout << "Channel not found\n";
	// 	return NULL;
	// }
	// const ChanStatus				*User::get_chanstatus_from_list(const Channel * channel) const {
	// 	std::vector<ChanStatus> chans = get_chan_list();
	// 	for (std::vector<ChanStatus>::const_iterator it = chans.begin(); it != chans.end();++it)
	// 	{
	// 		if ((*(it)).channel->get_name() == channel->get_name())
	// 			return &(*it);
	// 	}
	// 	std::cout << "Channel not found\n";
	// 	return NULL;
	// }
	std::vector<ChanStatus>::const_iterator	User::get_chanstatus_from_list(Channel const & channel,std::vector<ChanStatus> &chans)const {
		for (std::vector<ChanStatus>::const_iterator it = chans.begin(); it != chans.end();++it)
		{
			if ((*(it)).channel->get_name() == channel.get_name())
				return it;
		}
		std::cout << "Channel not found\n";
		return chans.end();
	}

	Socket<Address_ipv6> const *User::get_socket() const{
		return (this->_socket);}

	bool User::get_operator_status() const{
		return _is_irc_operator;}

	void User::set_password(std::string password){
		this->_password = password;}
	void User::set_ip(std::string ip){
		this->_ip = ip;}
	void User::set_port(std::string port){
		this->_port = port;}
	void User::set_uuid(){
		this->_uuid = this->_nickname + this->_username + this->_realname + this->_password;}
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
	// FIXME : why do you use server ?
	void	User::join_channel(Channel * channel) {
		this->_chan_list.push_back(ChanStatus(channel));
	}

	void	User::set_socket(Socket<Address_ipv6> const *socket) {
		(void)socket; // FIXME : fix that shit. The function wasn't created.
	}

// void User::join_channel(Server & Server, std::string channel){
// 	srand(time(NULL));
// 	for (std::vector<ChanStatus>::iterator it = get_chan_list().begin(); it != get_chan_list().end(); ++it){
// 		if ((*it).channel->get_name() == channel){
// 			for (std::vector<User *>::const_iterator it2 = (*it).channel->get_user_list().begin(); it2 != (*it).channel->get_user_list().end(); ++it2){
// 				if ((*it2)->get_nickname() != this->get_nickname() && (*it2)->get_operator_status() == true){ // WIP pue la merde
// 					(*it).channel->del_user(this);
// 					get_chan_list().erase(it);
// 					return ; // Channel found
// 				}
// 			}
// 			// No operator left on the channel
// 			(*it).channel->del_user(this);
// 			get_chan_list().erase(it);
// 			if ((*it).channel->get_user_list().size() > 5){
// 				(*it).channel->get_user_list().at(rand() % (*it).channel->get_user_list().size())->get_chanstatus_from_list((*it).channel)->is_operator = true;
// 			}
// 			else{
// 				for (std::vector<User *>::const_iterator it2 = (*it).channel->get_user_list().begin(); it2 != (*it).channel->get_user_list().end(); ++it2){
// 					(*it2)->get_chanstatus_from_list((*it).channel)->is_operator = true;
// 				} // If there is less than 6 user in the channel make them all op
// 			}
// 			}
// 			break;
// 		}
// 		for (std::vector<Channel *>::const_iterator it = Server.get_channel_list().begin(); it != Server.get_channel_list().end(); ++it){
// 			if ((*it)->get_name() == channel){
// 				ChanStatus chan(*it);
// 				if ((*it)->is_moderated() == true)
// 					chan.is_mute = true;
// 				chan.channel->add_user(this);
// 				get_chan_list().push_back(chan);
// 				return ; // Channel found
// 			}
// 		}
// 		// Channel not found in channel list : create it
// 		// WIP need to add mod to the channel creation
// 		Channel *new_chan = new Channel(channel);
// 		Server.add_channel(*new_chan);
// 		ChanStatus chan_status(new_chan);
// 		chan_status.is_operator = true;
// 		chan_status.is_mute = false;
// 		get_chan_list().push_back(chan_status);
// 		new_chan->add_user(this);
// 	}

	void User::leave_channel(Channel * channel){

		std::vector<ChanStatus>		chan_list = this->get_chan_list();
		for (std::vector<ChanStatus>::iterator it = chan_list.begin(); it != chan_list.end(); ++it){
			if ((*it).channel->get_name() == channel->get_name()){
				(*it).channel->del_user(this);
				get_chan_list().erase(it);
				break;
			}
		}
	}

	void User::send_message(std::string msg, Channel & channel){
		channel.transmit_message(msg, this);
	}

	void User::send_message(std::string msg, User & user){
		user.receive_message(this,msg);
	}

	void User::receive_message(User * user,std::string msg){
		std::string ret = ":" + user->get_nickname() + "!~" + user->get_username() + "@" + user->get_hostname() + " " + msg + "\r\n";
		Proxy_queue::Write * new_msg = new Proxy_queue::Write(this->get_socket()->get_fd(),ret.c_str());
		_server->get_event_list().push_back(new_msg);
	}

	void User::send_invite(User & user, Channel & channel){
		channel.invite_user(&user);
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

	void User::ban_user(User & user, Channel & channel){(void)user;(void)channel;}

	void User::unban_user(User & user,Channel & channel){(void)user;(void)channel;}

	void User::op_user(User & user, Channel & channel){(void)user;(void)channel;}

	void User::change_topic(Channel & channel,std::string topic){(void)channel,(void)topic;}

	void User::unmute_user(User & user, Channel & channel){(void)user;(void)channel;}

int	User::disconnect_user(){
	int ret = this->_socket->get_fd();
	this->_socket = NULL;
	for (std::vector<ChanStatus>::const_iterator it = this->_chan_list.begin(); it != this->_chan_list.end(); ++it){
		(*it).channel->del_user(this);
	}
	this->_chan_list.clear();
	return ret;
}
}
