/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 16:40:50 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/15 17:48:41 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/proxy/Event.hpp"

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Write::Write() { }
Proxy_queue::Write::~Write() { }
void			Proxy_queue::Write::handle() { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Write_priority::Write_priority() { }
Proxy_queue::Write_priority::~Write_priority() { }
void			Proxy_queue::Write_priority::handle() { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Confirm_connexion::Confirm_connexion() { }
Proxy_queue::Confirm_connexion::~Confirm_connexion() { }
void			Proxy_queue::Confirm_connexion::handle() { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Refuse_connexion::Refuse_connexion() { }
Proxy_queue::Refuse_connexion::~Refuse_connexion() { }
void			Proxy_queue::Refuse_connexion::handle() { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Disconnect_all::Disconnect_all() { }
Proxy_queue::Disconnect_all::~Disconnect_all() { }
void			Proxy_queue::Disconnect_all::handle() { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Disconnect::Disconnect() { }
Proxy_queue::Disconnect::~Disconnect() { }
void			Proxy_queue::Disconnect::handle() { }

///////////////////////////////////////////////////////////////////////////////
//
Proxy_queue::Signal::Signal() { }
Proxy_queue::Signal::~Signal() { }
void			Proxy_queue::Signal::handle() { }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
Server_queue::Message::Message() { }
Server_queue::Message::~Message() { }

Server_queue::Message::Message(Server_queue::Message::data_type data,
		const Server_queue::Message::socket_type *client) {
	std::cout << " ------------------- MESSAGE RECEIVED " << std::endl;
}

void			Server_queue::Message::handle() { }

///////////////////////////////////////////////////////////////////////////////
//
Server_queue::Message_priority::Message_priority() { }

Server_queue::Message_priority::~Message_priority() { }

void			Server_queue::Message_priority::handle() { }

///////////////////////////////////////////////////////////////////////////////
//

Server_queue::Request_connexion::Request_connexion(){ }

Server_queue::Request_connexion::Request_connexion(const socket_type *client){
	std::cout << " ------------------- CONNEXION REQUEST RECEIVED " << std::endl;
}

Server_queue::Request_connexion::~Request_connexion(){ }

void			Server_queue::Request_connexion::handle(){ }

///////////////////////////////////////////////////////////////////////////////
//

Server_queue::Client_disconnected::Client_disconnected(){ }
Server_queue::Client_disconnected::Client_disconnected(const socket_type *socket){ }
Server_queue::Client_disconnected::~Client_disconnected(){ }

void			Server_queue::Client_disconnected::handle(){ }

///////////////////////////////////////////////////////////////////////////////
//

Server_queue::Error::Error(){ }
Server_queue::Error::~Error(){ }
void			Server_queue::Error::handle(){ }

///////////////////////////////////////////////////////////////////////////////
//

Server_queue::Signal::Signal(){ }
Server_queue::Signal::~Signal(){ }
void			Server_queue::Signal::handle(){ }
