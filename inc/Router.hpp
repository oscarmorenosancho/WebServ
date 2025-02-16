/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:35 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 11:09:05 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ROUTER_HPP_
# define _ROUTER_HPP_

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

# include <fstream>
# include <iostream>

# include <Request.hpp>
# include <Response.hpp>
# include <CgiExecutor.hpp>
# include <ActionMask.hpp>
# include <CgiExecutor.hpp>
# include <FolderLs.hpp>
# include <MimeMap.hpp>
# include <SplitString.hpp>
# include <Defines.hpp>
# include <Log.hpp>

# define NEEDED_CGI_HEADERS_NB 3

class Router
{
private:
	static std::string	neededCgiHeaders[ NEEDED_CGI_HEADERS_NB ];
	static std::string	methods[ METHODS_NB ];
	static bool ( *process[ METHODS_NB ] )( Request& req );
	static bool	processGetRequest( Request& req );
    static bool	processHeadRequest(Request &req);
    static bool processPostRequest(Request &req);
    static bool	processPutRequest(Request &req);
    static bool processDeleteRequest(Request &req);
	static void	processGetFileRead( Request& req, Client *cli, std::string path);
	static bool	processDirectory( Request& req, std::string path, \
							std::string& output );
private:
	Router( void );
	~Router();
	Router( const Router& );
	Router&	operator=( const Router& );
public:
	static int			updateResponse(Response& res, Request& req, Client& cli);
	static Response* 	formatGenericResponse(Response& res, Request& req);
    static bool 		parseCgiHeaderLine(Response &res, Request &req, const std::string &line, bool& isValid);
    static bool 		parseCgiHeaders(Response &res, Request &req, const std::string &cgiOut);
    static bool 		parseCgiOutput(Response &res, Request &req, Client& cli);
    static Response 	*formatCgiResponse(Response &res, Request &req, Client& cli);
	static bool			processRequestHeaderReceived( Request &req );
	static std::string 	determineContentType(Response &res, Request &req);
	static bool			processCgi( Request& req );
	static bool			checkStatMode( std::string path, unsigned int mode );
	static bool			isDir( std::string path );
    static bool 		checkPathExist(Request &req, std::string path);
    static bool			checkPathCanRead( Request& req, std::string path );
	static std::string	getDefaultErrorPage( unsigned int code );
	static int			getFileToRead( Request& req, std::string& file );
	static ssize_t		getFileSize( std::string file );
	static void			checkRedir( Request& req );
	static void 		checkErrorRedir( int errorStatus, Request& req );
	static bool			checkErrorBody( Request& req, int errorStatus );
	static bool			isValidDirectory( std::string dir );
	static int			openReadFile( std::string file );
	static int			openWriteFile( std::string file );
};

#endif
