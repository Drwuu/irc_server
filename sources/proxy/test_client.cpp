/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_client.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaman <mhaman@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 19:12:55 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/16 22:31:49 by mhaman           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <unistd.h>
#include <iostream>

// Functions for the client side :
// socket() connect() read() write()

int main(int ac, char **av) {
	if (ac < 2 || ac > 3) {
		std::cerr << "usage : ircserv [port] [password]" << std::endl;
		return -1;
	}

	int sockfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    struct hostent *Server;
	int	port_nb = std::atoi(av[1]);

    char buffer[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)  {
		std::cerr << " Error socket creation" << std::endl;
	}
    Server = gethostbyname(av[2]);
    if (Server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)Server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         Server->h_length);
    serv_addr.sin_port = htons(port_nb);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		std::cerr << " Errr connect() " << std::endl;
		std::cerr << " ERRNO : " << std::strerror(errno) << std::endl;
		return 1;
	}
    bzero(buffer,256);
    printf("Please enter the message: ");
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) {
		std::cerr << " Error writting " << std::endl;
	}
    bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) {
		std::cerr << " Error reading " << std::endl;
	}
    printf("%s\n",buffer);
    close(sockfd);
     return 0;
}
