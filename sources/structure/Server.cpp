#include "../../headers/structure/Server.hpp"
Server::~Server() {
	for (irc::map_iterator_cmd it = _map.begin(); it != _map.end(); it++)
		if (it->second)
			delete it->second;
	_map.clear();
};
Server::Server(): _map() {
	_map.insert(std::make_pair("INVITE", new irc::invite(irc::vector_args(1, "INVITE"))));
	_map.insert(std::make_pair("KICK", new irc::kick(irc::vector_args(1, "KICK"))));
	_map.insert(std::make_pair("MODE", new irc::mode(irc::vector_args(1, "MODE"))));
};
Server::Server(Server const &src) {
	*this = src;
};
/* Getters & Setters */
irc::map_cmd &Server::get_map() {
	return _map;
};

Server::Server(std::string password,std::string port):_password(password),_port(port){}

const std::vector<Channel *> Server::get_channel_list() const{
	return (this->_channel_list);}

std::vector<User *> Server::get_user_list(){
	return (this->_user_list);}

const std::string Server::get_name() const{
	return (this->_name);}

const std::string Server::get_hostname() const{
	return (this->_hostname);}

const std::string Server::get_password() const{
	return (this->_password);}

const std::string Server::get_port() const{
	return (this->_port);}

const std::string Server::get_ip() const{
	return (this->_ip);}
void					Server::set_name(std::string name){
	this->_name = name;}
void					Server::set_label(std::string label){
	this->_label = label;}
void					Server::set_hostname(std::string hostname){
	this->_hostname = hostname;}
void					Server::set_password(std::string password){
	this->_password = password;}
void					Server::set_port(std::string port){
	this->_port = port;}
void					Server::set_ip(std::string ip){
	this->_ip = ip;}

void Server::add_user(User & user){
	this->_user_list.push_back(&user);}

void Server::add_channel(Channel & channel){
	this->_channel_list.push_back(&channel);}

void Server::del_channel(Channel & channel){
	for (std::vector<Channel *>::iterator it = this->_channel_list.begin(); it != this->_channel_list.end(); ++it){
		if (*it == &channel){
			this->_channel_list.erase(it);
			break;
		}
	}}

void Server::del_user(User & user){
	for (std::vector<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end(); ++it){
		if (*it == &user){
			this->_user_list.erase(it);
			break;
		}
	}}

void Server::exec_cmd(User & user, irc::command *command){
//command.parse(user, this);
//command.execute(user, this);
}
