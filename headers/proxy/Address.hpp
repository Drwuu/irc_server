/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 16:40:21 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/14 19:21:08 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDRESS_HPP
# define ADDRESS_HPP

# include <_wctype.h>
# include <netinet/in.h>
# include <string.h>
# include <string>
# include <arpa/inet.h>

// struct sockaddr_in6 {
// 		__uint8_t       sin6_len;       [> length of this struct(sa_family_t) <]
// 		sa_family_t     sin6_family;    [> AF_INET6 (sa_family_t) <]
// 		in_port_t       sin6_port;      [> Transport layer port # (in_port_t) <]
// 		__uint32_t      sin6_flowinfo;  [> IP6 flow information <]
// 		struct in6_addr sin6_addr;      [> IP6 address <]
// 		__uint32_t      sin6_scope_id;  [> scope zone index <]
// };

class	Address_ipv6 {
	public:
		typedef		struct sockaddr_in6		value_type;
		typedef		const value_type		const_value_type;

		typedef		in_port_t				port_type;
		typedef		sa_family_t				family_type; //__uint8_t
		typedef		sa_family_t				len_type; //__uint8_t
		typedef		in6_addr_t				address_type;

	// address.sin6_family = AF_INET6;
	// address.sin6_addr = in6addr_any;
	// address.sin6_port = htons(_server.port);
	// address.sin6_len = sizeof(_server.address);

	private:
		Address_ipv6();
		value_type				_address;
		family_type				_family;
		len_type				_len;
		port_type				_port;

	public:
		Address_ipv6(const port_type &port);
		Address_ipv6(const Address_ipv6 &source);
		~Address_ipv6();
		Address_ipv6	&operator=(const Address_ipv6 &source);

		value_type		&operator*();
		value_type		data();

		void			set_len(const len_type &len);
		void			set_port(const port_type &port);
		void			set_family(const family_type &family);

		port_type		get_port() const;
		address_type	get_address() const;
		std::string		get_readable_address() const;
		len_type		get_len() const;
		len_type		get_family() const;

		void			renew(const port_type &port);
};

#endif
