/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:55:51 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/14 12:06:51 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Directives.hpp>

Directives::Directives( void ): _port( 8000 ), \
								_host( "0.0.0.0" ), \
								_clientMaxBodySize( 1 << 20 ), \
								_autoindex( false ), \
								_locations( Location::locationCompare ), \
								_isEmpty( true )
{
	this->_return.first = -1;
	for ( int i = 0; i < SIZE_DIRECTIVES; i++ )
		this->_isSet[ DirectivesParser::directivesList[ i ] ] = false;
}

Directives::~Directives( void )
{
	deleteLocationsSet();
}

Directives::Directives( const Directives& d ):
								_root( d._root ), \
								_port( d._port ), \
								_host( d._host ), \
								_serverNames( d._serverNames ), \
								_errorPages( d._errorPages ), \
								_clientMaxBodySize( d._clientMaxBodySize ), \
								_uploadStore( d._uploadStore ), \
								_index( d._index ), \
								_autoindex( d._autoindex ), \
								_alias( d._alias ), \
								_return( d._return ), \
								_allowMethods( d._allowMethods ), \
								_cgis( d._cgis ), \
								_servers( d._servers ), \
								_locations( Location::locationCompare ), \
								_isSet( d._isSet ),
								_isEmpty( d._isEmpty )
{
	copyLocationsSet( d._locations );
}

Directives&	Directives::operator=( const Directives& d )
{
	if ( this != &d )
	{
		this->_root = d._root;
		this->_port = d._port;
		this->_host = d._host;
		this->_serverNames = d._serverNames;
		this->_errorPages = d._errorPages;
		this->_clientMaxBodySize = d._clientMaxBodySize;
		this->_uploadStore = d._uploadStore;
		this->_index = d._index;
		this->_autoindex = d._autoindex;
		this->_alias = d._alias;
		this->_return = d._return;
		this->_allowMethods = d._allowMethods;
		this->_cgis = d._cgis;
		this->_servers = d._servers;
		this->_isSet = d._isSet;
		this->_isEmpty = d._isEmpty;
		deleteLocationsSet();
		copyLocationsSet( d._locations );
	}
	return ( *this );
}

void	Directives::deleteLocationsSet( void )
{
	for ( LocationsSet::const_iterator it = this->_locations.begin(); \
			it != this->_locations.end(); it++ )
	{
		if ( *it != NULL )
			delete *it;
	}
	this->_locations.clear();
}

void	Directives::copyLocationsSet( const LocationsSet& locations )
{
	for ( LocationsSet::const_iterator it = locations.begin(); \
			it != locations.end(); it++ )
	{
		if ( *it != NULL )
			this->_locations.insert( new Location( **it ) );
	}
}

std::string	Directives::getRoot( void ) const { return ( this->_root ); }

int	Directives::getPort( void ) const { return ( this->_port ); }

std::string	Directives::getHost( void ) const { return ( this->_host ); }

const StringVector&	Directives::getServerNames( void ) const { return ( this->_serverNames ); }

const ErrorPagesMap&	Directives::getErrorPages( void ) const { return ( this->_errorPages ); }

unsigned int	Directives::getClientMaxBodySize( void ) const { return ( this->_clientMaxBodySize ); }

std::string	Directives::getUploadStore( void ) const { return ( this->_uploadStore ); }

const StringVector&	Directives::getIndex( void ) const { return ( this->_index ); }

bool	Directives::getAutoindex( void ) const { return ( this->_autoindex ); }

std::string	Directives::getAlias( void ) const { return ( this->_alias ); }

const ReturnPair&	Directives::getReturn( void ) const { return ( this->_return ); }

const ActionMask&	Directives::getAllowMethods( void ) const { return ( this->_allowMethods ); }

const CgisMap&	Directives::getCgis( void ) const { return ( this->_cgis ); }

const ServersVector&	Directives::getServers( void ) const { return ( this->_servers ); }

const LocationsSet&	Directives::getLocations( void ) const { return ( this->_locations ); }

const StringBoolMap&	Directives::getIsSet( void ) const { return ( this->_isSet ); }

bool	Directives::isEmpty( void ) const { return ( this->_isEmpty ); }

bool	Directives::isSet( const std::string& key ) const
{
	try
	{
		return ( this->_isSet.at( key ) );
	}
	catch ( const std::exception& )
	{
		return ( false );
	}
}

bool	Directives::getIsAllowedMethod( std::string method ) const
{
	int	action;

	action = ActionMask::whichAction( method );
	if ( action != ActionMask::INVALID )
		return ( this->_allowMethods.getAction( action ) );
    return ( false );
};

bool	Directives::getErrorPageWithCode( unsigned int code, std::string& page ) const
{
	try
	{
		page = this->_errorPages.at( code );
		return ( true );
	}
	catch ( const std::exception& ) {}
	return ( false );
}

bool	Directives::tryIndexFiles( std::string& file, std::string path ) const
{
	std::string	test;

	for ( StringVector::const_iterator it = this->_index.begin(); \
			it != this->_index.end(); it++ )
	{
		test = ConfigUtils::pathJoin( path, *it );
		if ( access( test.c_str(), F_OK ) == 0 )
		{
			file = test;
			return ( true );
		}
	}
	return ( false );
}

bool	Directives::findReturnUri( int& uriCode, std::string& uriRedir ) const
{
	if ( isSet( "return" ) )
	{
		uriCode = this->_return.first;
		uriRedir = this->_return.second;
		return ( true );
	}
	return ( false );
}

void	Directives::print( void ) const
{
	Log::Debug( "[ Config ] root: " + this->_root );
	Log::Debug( "[ Config ] host: " + this->_host );
	Log::Debug( "[ Config ] port: " + SUtils::longToString( this->_port ) );
	Log::Debug( "[ Config ] server_name: " \
				+ STLUtils::vectorToString< StringVector >( \
				this->_serverNames.begin(), this->_serverNames.end(), " " ) );
	Log::Debug( "[ Config ] error_page: " \
				+ STLUtils::mapToString< ErrorPagesMap >( \
				this->_errorPages.begin(), this->_errorPages.end() ) );
	Log::Debug( "[ Config ] client_max_body_size: " \
				+ SUtils::longToString( this->_clientMaxBodySize ) );
	Log::Debug( "[ Config ] upload_store: " + this->_uploadStore );
	Log::Debug( "[ Config ] index: " \
				+ STLUtils::vectorToString< StringVector >( \
				this->_index.begin(), this->_index.end(), " " ) );
	Log::Debug( "[ Config ] autoindex: " + std::string( \
				this->_autoindex == true ? "on" : "off" ) );
	Log::Debug( "[ Config ] alias: " + this->_alias );
	Log::Debug( "[ Config ] return: " \
				+ SUtils::longToString( this->_return.first ) \
				+ " -> " \
				+ this->_return.second );
	Log::Debug( "[ Config ] allow_methods: " \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::GET ) == true ? "GET " : "" ) \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::POST ) == true ? "POST " : "" ) \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::DELETE ) == true ? "DELETE" : "" ) );
	Log::Debug( "[ Config ] servers: " );
	Log::Debug( "[ Config ] location: " );
	std::cout << std::endl;
}
