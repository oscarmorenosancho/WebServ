/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/21 15:44:43 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server( void ): _directives( NULL ) {}

Server::Server( const Server& s )
{
	this->_directives = NULL;
	if ( s._directives != NULL )
		this->_directives = new Directives( *s._directives );
}

Server::~Server( void )
{
	if ( this->_directives != NULL )
		delete this->_directives;
}

Server&	Server::operator=( const Server& s )
{
	if ( this != &s )
	{
		if ( this->_directives != NULL )
			delete this->_directives;
		this->_directives = NULL;
		if ( s._directives != NULL )
			this->_directives = new Directives( *s._directives );
	}
	return ( *this );
}

Directives	*Server::getDirectives( void ) const { return ( this->_directives ); }

Location*	Server::getLocationAtPath( std::string path ) const
{
	Location *lcp;
	LocationsSet::iterator it = this->_directives->_locations.begin();
	LocationsSet::iterator ite = this->_directives->_locations.end();
	Log::Success("Server::getLocationAtPath size: " + SUtils::longToString(this->_directives->_locations.size()));
	while (it != ite)
	{
		lcp = *it;
		Log::Success("Server::getLocationAtPath comparing: " + lcp->getPath() + " with " + path);
		if (lcp->getPath() == path)
			return lcp;
		it++;
	}
	return (NULL);
}

std::string	Server::getErrorPageWithCode( unsigned int code ) const
{
	( void ) code;
	return ( "a" );
}

bool	Server::serverMatch( std::string host, std::string port ) const
{
	StringVector::const_iterator	it;
	StringVector::const_iterator	ite = this->_directives->getServerNames().end();

	if ( SUtils::compareNumbersAsStrings( port, \
		SUtils::longToString( this->_directives->getPort()) ) )
		return ( false );
	for ( it = this->_directives->getServerNames().begin(); it != ite; it++ )
	{
		// Log::Success(std::string("Server::serverMatch iterate: " + *it + " to locate: " + host));
		if ( *it == host )
			return ( true );
	}
	return ( false );
}

const std::string	Server::getCgiBinary( std::string ext, std::string route) const
{
	Location* loc = getLocationAtPath(route);
	if (!loc)
		return (std::string(""));
	return (loc->getCgiBinary(ext));
}