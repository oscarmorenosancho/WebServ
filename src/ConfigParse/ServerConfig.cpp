/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:36:45 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/22 12:34:09 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerConfig.hpp>

ServerConfig::ServerConfig( void ): _port( 8000 ), _host( "0.0.0.0" ), _clientMaxBodySize( 1 << 20 ) {}

ServerConfig::ServerConfig( const ServerConfig& s ): _port( s._port ), \
								_host( s._host ), \
								_locations( s._locations ), \
								_rootDir( s._rootDir ), \
								_serverNames( s._serverNames ), \
								_clientMaxBodySize( s._clientMaxBodySize ), \
								_errorPages( s._errorPages ) {}

ServerConfig::~ServerConfig( void ) {}

ServerConfig&	ServerConfig::operator=( const ServerConfig& s )
{
	if ( this != &s )
	{
		this->_port = s._port;
		this->_host = s._host;
		this->_locations = s._locations;
		this->_rootDir = s._rootDir;
		this->_serverNames = s._serverNames;
		this->_clientMaxBodySize = s._clientMaxBodySize;
		this->_errorPages = s._errorPages;
	}
	return ( *this );
}

int	ServerConfig::getPort( void ) const
{
	return ( this->_port );
}

std::string	ServerConfig::getHost( void ) const
{
	return ( this->_host );
}

LocationsVector	ServerConfig::getLocations( void ) const
{
	return ( this->_locations );
}

std::string	ServerConfig::getRoot( void ) const
{
	return ( this->_rootDir );
}

StringVector	ServerConfig::getServerNames( void ) const
{
	return ( this->_serverNames );
}

unsigned int	ServerConfig::getClientMaxBodySize( void ) const
{
	return ( this->_clientMaxBodySize );
}

ErrorPagesMap	ServerConfig::getErrorPages( void ) const
{
	return ( this->_errorPages );
}