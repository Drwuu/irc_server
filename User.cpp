#include "User.hpp"

User::User(){}
User::~User(){}

const std::string User::get_username() const{
	return (this->_username);}
const std::string User::get_nickname() const{
	return (this->_nickname);}
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
std::vector<std::string> User::get_past_username(){
	return (this->_past_username);}
std::vector<t_chanstatus> User::get_chan_list(){
	return (this->_chan_list);}

void User::set_password(std::string password){
	this->_password = password;}
void User::set_ip(std::string ip){
	this->_ip = ip;}
void User::set_port(std::string port){
	this->_port = port;}
void User::set_uuid(std::string uuid){
	this->_uuid = uuid;}
void User::set_username(std::string username){
	if (this->_username.c_str()){
		this->_past_username.push_back(this->_username);
	}
	this->_username = username;}
void User::set_nickname(std::string nickname){
	this->_nickname = nickname;}
