/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 13:56:30 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/27 17:25:58 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SOCKETS_HPP_
# define _SOCKETS_HPP_

# include <Defines.hpp>
# include <Utils.hpp>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <iostream>

class Sockets
{
	private:
		Sockets( void );
		Sockets( const Sockets& socket );
		~Sockets( void );
		Sockets	operator=( const Sockets& socket );
	public:
		static socket_t				socketCreate( int domain, int type, int protocol );
		static struct sockaddr_in	fillSockAddr( int family, uint16_t port, uint32_t ip_addr );
		static void					bindSocket( socket_t fd, struct sockaddr_in addr );
		static void					listenFromSocket( socket_t fd, int backlog );
		static socket_t				acceptConnection( socket_t fd );
		static socket_t				createPassiveSocket( int port, int backlog );
};

#endif
