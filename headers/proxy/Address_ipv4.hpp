
#ifndef ADDRESS_IPV4_HPP
# define ADDRESS_IPV4_HPP

# include <_wctype.h>
# include <netinet/in.h>
# include <string.h>
# include <string>
# include <arpa/inet.h>
#include <sys/_types/_in_addr_t.h>

// struct sockaddr_in {
	// __uint8_t       sin_len;
	// sa_family_t     sin_family;
	// in_port_t       sin_port;
	// struct  in_addr sin_addr;
	// char            sin_zero[8];
// };

class	Address_ipv4 {
	public:
		typedef		struct sockaddr_in		value_type;
		typedef		const value_type		const_value_type;

		typedef		in_port_t				port_type;
		typedef		sa_family_t				family_type; //__uint8_t
		typedef		in_addr_t				address_type;
		typedef		__uint8_t				len_type;
		typedef		char					zero_type;

	// address.sin6_family = AF_INET6;
	// address.sin6_addr = in6addr_any;
	// address.sin6_port = htons(_server.port);
	// address.sin6_len = sizeof(_server.address);

	private:
		Address_ipv4();
		value_type				_address;
		family_type				_family;
		len_type				_len;
		port_type				_port;

	public:
		Address_ipv4(const port_type &port);
		Address_ipv4(const Address_ipv4 &source);
		~Address_ipv4();
		Address_ipv4	&operator=(const Address_ipv4 &source);

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
