/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:17 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 11:09:11 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Router.hpp>

Router::Router( void ) {}

Router::~Router( void ) {}

Router::Router( const Router& ) {}

Router&	Router::operator=( const Router& )
{
	return ( *this );
}

int	Router::updateResponse( Response &res, Request &req, Client& cli )
{
	Log::Debug("updateResponse");
	res.setServer( SERVER );
	if ( req.getUseCgi() )
		formatCgiResponse( res,req, cli );
	else
		formatGenericResponse( res, req );
	res.updateResString();
	return ( 0 );
}

std::string Router::determineContentType(Response& res, Request& req)
{
	(void)res;
	if ( req.getMethod() == "GET" || req.getMethod() == "HEAD" )
	{
		std::string contentType = MimeMap::getMime(req.getDocExt());
		return (contentType);
	}
	return ( "text/plain" );
}

Response	*Router::formatGenericResponse( Response& res, Request& req )
{
	Log::Debug("formatGenericResponse");
	res.appendHeader( Header( "Content-Type", determineContentType( res, req ) ) );
	res.setProtocol( req.getProtocol() );
	if ( req.getRedir() == true )
		res.appendHeader( Header( "Location", req.getUriRedir() ) );
	res.setStatus( req.getError() );
	res.setMethod( req.getMethod() );
	if ( req.getMethod() != "HEAD" )
	{
		if ( req.getOutput().length() > 0 )
			res.setBody( req.getOutput() );
		else if (! res.getHeaders().firstWithKey("Content-Length"))
			res.appendHeader(Header("Content-Length", "0"));
	}
	else
		res.setBodyLength( req.getOutputLength() );
	return ( &res );
}

bool	Router::checkStatMode( std::string path, unsigned int mode )
{
	struct stat	checkMode;

	if ( stat( path.c_str(), &checkMode ) == -1 )
		return ( false );
	return ( ( checkMode.st_mode & mode ) == mode );
}

bool	Router::isDir( std::string path ) { return ( checkStatMode( path, S_IFDIR ) ); }

int	Router::openReadFile( std::string file )
{
	int	fd;

	fd = open( file.c_str(), O_RDONLY | O_NONBLOCK );
	if ( fd < 0 )
		return ( fd );
	Log::Debug( "Open file read: " + SUtils::longToString( fd ) );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	return ( fd );
}

int	Router::openWriteFile( std::string file )
{
	int	fd;

	fd = open( file.c_str(), O_WRONLY | O_TRUNC | O_NONBLOCK | O_CREAT, 0666 );
	if ( fd < 0 )
		return ( fd );
	Log::Debug( "Open file write: " + SUtils::longToString( fd ) );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	return ( fd );
}

bool	Router::checkPathExist( Request& req, std::string path )
{
	if ( checkStatMode( path, F_OK ) == false )
	{
		req.setError( HTTP_NOT_FOUND_CODE );
		return ( false );
	}
	return ( true );
}

bool	Router::checkPathCanRead( Request& req, std::string path )
{
	if ( checkStatMode( path, R_OK ) == false )
	{
		req.setError( HTTP_FORBIDDEN_CODE );
		return ( false );
	}
	return ( true );
}

bool	Router::isValidDirectory( std::string dir )
{
	return ( dir[ dir.length() -1 ] == '/' );
}

bool	Router::processDirectory( Request& req, std::string path, \
									std::string& output )
{
	if ( isValidDirectory( path ) == false )
		req.setError( HTTP_NOT_FOUND_CODE );
	else if ( req.isAutoindexAllow() == true \
		&& FolderLs::getLs( output, path, req.getRoute() ) == LsEntry::NONE )
	{
		req.setStatus( HTTP_OK_CODE );
		req.setDocExt( "html" );
	}
	else
		req.setError( HTTP_FORBIDDEN_CODE );
	return ( req.getError() == HTTP_OK_CODE );
}

void	Router::checkRedir( Request& req )
{
	std::string	uriRedir;
	int			uriCode;

	if ( req.findReturnUri( uriCode, uriRedir ) == true )
		req.setRedirection( uriRedir, uriCode );
}

void 	Router::checkErrorRedir( int errorStatus, Request& req )
{
	bool		redir = false;
	std::string	uriRedir;

	if ( errorStatus >= MIN_ERROR_CODE )
		redir = req.getErrorPage( errorStatus, uriRedir );
	if ( redir == true )
	{
		if ( req.getMethod() == "GET" )
			req.setRedirection( uriRedir, HTTP_MOVED_TEMPORARILY_CODE );
		else
			req.setRedirection( uriRedir, HTTP_SEE_OTHER_CODE );
	}
}

bool	Router::checkErrorBody( Request& req, int errorStatus )
{
	if ( errorStatus >= MIN_ERROR_CODE )
	{
		req.setOutput( getDefaultErrorPage( errorStatus ) );
		req.setDocExt( "html" );
		return ( true );
	}
	return ( false );
}

int	Router::getFileToRead( Request& req, std::string& retFile )
{
	std::string	path;
	std::string	file;

	path = req.getFilePathRead();
	if ( checkPathExist( req, path ) == false )
		return ( ENOENT );
	file = path;
	if ( isDir( path ) == true && req.tryIndexFiles( file ) == false )
	{
		retFile = path;
		return ( EISDIR );
	}
	if ( isValidDirectory( file ) == true )
		return ( ENOENT );
	if ( checkPathCanRead( req, file ) == false )
		return ( EACCES );
	retFile = file;
	req.setStatus( HTTP_OK_CODE );
	return ( EXIT_SUCCESS );
}

ssize_t	Router::getFileSize( std::string file )
{
	struct stat	info;

	if ( stat( file.c_str(), &info ) == -1 )
		return ( -1 );
	return ( info.st_size );
}

std::string	Router::getDefaultErrorPage( unsigned int code )
{
	std::string	def;
	std::string	codeStr;

	codeStr = SUtils::longToString( code ) + " " + StatusCodes::decode( code );
	def += "<html>\n";
	def += "<head><title>" + codeStr + "</title></head>\n";
	def += "<body>\n";
	def += "<center><h1>" + codeStr + "</h1></center>\n";
	def += "<hr><center>webserv/1.0 </center>\n";
	def += "</body>\n";
	def += "</html>\n";
	return ( def );
}
