/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:11 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/06 13:23:17 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "Router.hpp"
#include <unistd.h>
#include "Client.hpp"
#include "CgiExecutor.hpp"

PendingCgiTasks	CgiExecutor::pendingTasks;

CgiExecutor::CgiExecutor( Request& request ): request( request )
{
	this->binary = request.getCgiBinary( request.getDocExt() );
	this->argument = request.getFilePathRead();
	// Log::Info( "CgiExecutor binary: " + this->binary );
	// Log::Info( "CgiExecutor argment: " + this->argument );
	// Log::Info( "Route chain: " + request.getRouteChaineString() );
	if (!checkFileReadable(this->argument))
	{
		throw std::runtime_error("Either not found or not readable: " + this->argument);
	}
	if (!checkFileExecutable(this->binary))
	{
		throw std::runtime_error("Either not found or not executable: " + this->binary);
	}
	argv[0] = (char *)this->binary.c_str();
	argv[1] = (char *)this->argument.c_str();
	argv[2] = NULL;
	childEnv = NULL;
}

CgiExecutor::~CgiExecutor( void )
{
	envVars.clear();
	if ( childEnv != NULL )
		delete [] childEnv;
}

CgiExecutor::CgiExecutor(const CgiExecutor & b): request( b.request )
{
	
}

CgiExecutor& CgiExecutor::operator=( const CgiExecutor& )
{
	return (*this); 	
}


void	CgiExecutor::onFailFork( void )
{
	close( this->fdToChild[ FDIN ] );
	close( this->fdToChild[ FDOUT ] );
	close( this->fdFromChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	throw std::runtime_error("fork: failed create child process");
}

void	CgiExecutor::onFailToChildPipeOpen( void )
{
	throw std::runtime_error("pipe: failed to open pipe to write to child");
}

void	CgiExecutor::onFailFromChildPipeOpen( void )
{
	close( this->fdToChild[ FDIN ] );
	close( this->fdToChild[ FDOUT ] );
	throw std::runtime_error("pipe: failed to open pipe to read from child");
}

char	**CgiExecutor::getEnvVarList( void )
{
	StringVector::iterator	it = envVars.begin();
	StringVector::iterator	ite = envVars.end();
	size_t					i = 0;

	this->childEnv = new char *[ envVars.size() + 1 ];
	while ( it != ite )
	{
		this->childEnv[ i ] = ( char* )it->c_str();
		it++;
		i++;
	}
	this->childEnv[ i ] = 0;
	return ( this->childEnv );
}

void	CgiExecutor::onChildProcess( void )
{
	//On child
	signal( SIGINT, SIG_DFL );
	signal( SIGQUIT, SIG_DFL );
	signal( SIGPIPE, SIG_DFL );
	// Log::Error( "CgiExecutor::onChildProcess: -----------------");
	getEnvVarList();
	close( this->fdToChild[ FDOUT ] );
	close( this->fdFromChild[ FDIN ] );
	if ( dup2( this->fdToChild[ FDIN ], FDIN ) < 0 )
		Log::Error( "dup2: fdToChild[ 0 ] to 0 failed: " \
			+ SUtils::longToString( errno ) );
	if ( dup2( this->fdFromChild[ FDOUT ], FDOUT ) < 0 )
		Log::Error( "dup2: fdFromChild[ 1 ] to 1 failed: " \
			+ SUtils::longToString( errno ) );
	close( this->fdToChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	// Log::Error( "CgiExecutor::onChildProcess: about to execve");
	execve( binary.c_str(), argv, childEnv );
	Log::Error( "execve: Not found binary " + binary);
	return; //exit(1) not allowed?
}

void	CgiExecutor::onParentProcess( pid_t childPid )
{
	Client	*cli;

	close( this->fdToChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	cli = this->request.getClient();
	if ( cli )
		cli->setEventProc( this->fdFromChild[ FDIN ], this->fdToChild[ FDOUT ], childPid );
}

//void	CgiExecutor::onParentProcess( pid_t childPid )
//{
//	std::string	body = request.getBody();
//	// char		*reqBody = ( char * )body.c_str();
//	size_t		reqBodySize = body.length();
//	size_t		i;
//	size_t		chunkSize = 1000;
//
//
//	// Log::Info("CgiExecutor::onParentProcess start");
//	close( this->fdToChild[ FDIN ] );
//	close( this->fdFromChild[ FDOUT ] );
//	i = 0;
//	while (i < reqBodySize)
//	{
//		if ((reqBodySize - i) < chunkSize)
//			chunkSize = reqBodySize - i;
//		std::string chunk = body.substr(i, chunkSize);
//		const char * chunkStr = chunk.c_str();
//		// Log::Info("Size: " + SUtils::longToString(chunkSize));
//		// write(1, chunkStr, chunkSize);
//		if ( write( this->fdToChild[ FDOUT ], chunkStr, chunkSize ) < 0 )
//			break ;
//		// Log::Info("fd: " + SUtils::longToString(this->fdToChild[ FDOUT ]));
//		i += chunkSize;
//	}
//	// Log::Info("CgiExecutor::onParentProcess passed write");
//	close( this->fdToChild[ FDOUT ] );
//	PendingCgiTask task( childPid, request, fdFromChild[ FDIN ] );
//	CgiExecutor::pendingTasks.appendTask( task );
//	// Log::Error("CgiExecutor::onParentProcess pid " + SUtils::longToString( childPid )
//	// 	+ " timestamp: " + SUtils::longToString( std::clock() ) );
//	attendPendingCgiTasks();
//}

bool CgiExecutor::checkFileExecutable(std::string file)
{
	bool res = (access(file.c_str(), F_OK) == 0);
	res &= (access(file.c_str(), X_OK) == 0);
	return (res);
}

bool CgiExecutor::checkFileReadable(std::string file)
{
	bool res = (access(file.c_str(), F_OK) == 0);
	res &= (access(file.c_str(), R_OK) == 0);
	return (res);
}

std::string	CgiExecutor::getChildOutput( PendingCgiTask *task )
{
	if ( !task )
		return ( "" );
	return ( task->getTaskOutput() );
}

int	CgiExecutor::execute( void )
{
	pid_t	pid;

	if ( pipe( this->fdToChild ) )
		onFailToChildPipeOpen();
	if ( pipe( this->fdFromChild ) )
		onFailFromChildPipeOpen();
	fcntl(this->fdFromChild[ FDIN ], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->fdFromChild[ FDOUT ], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->fdToChild[ FDIN ], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->fdToChild[ FDOUT ], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	pid = fork();
	if ( pid < 0 )
		onFailFork();
	if ( pid == 0 )
		onChildProcess();
	onParentProcess( pid );
	return ( 0 );
}

PendingCgiTask	*CgiExecutor::getCompletedTask( void )
{
	pid_t	pid;
	
	pid = waitpid( -1, NULL, WNOHANG );
	if ( pid < 1 )
		return ( NULL );
	if ( CgiExecutor::pendingTasks.empty() )
		return ( NULL );
	PendingCgiTask& tk = CgiExecutor::pendingTasks[ pid ];
	if ( !tk.isMarkedToDelete() )
		return ( &tk );
	return ( NULL );
}

PendingCgiTask	*CgiExecutor::getTimeoutedTask( double to )
{
	PendingCgiTasks::iterator	it = CgiExecutor::pendingTasks.begin();
	PendingCgiTasks::iterator	ite = CgiExecutor::pendingTasks.end();

	if ( CgiExecutor::pendingTasks.empty() )
		return ( NULL );
	while ( it != ite )
	{
		if ( !it->second.isMarkedToDelete() && it->second.isTimeout( to, false ) )
			return ( &( it->second ) );
		it++;
	}
	return ( NULL );
}

PendingCgiTask *CgiExecutor::getMarkedToDeleteTask( void )
{
	PendingCgiTasks::iterator it = CgiExecutor::pendingTasks.begin();
	PendingCgiTasks::iterator ite = CgiExecutor::pendingTasks.end();

	if ( CgiExecutor::pendingTasks.empty() )
		return ( NULL );
	while ( it != ite )
	{
		if ( it->second.isMarkedToDelete() )
			return ( &( it->second ) );
		it++;
	}
	return ( NULL );
}

size_t	CgiExecutor::purgeTimeoutedTasks( double to, size_t max )
{
	size_t			i = 0;
	PendingCgiTask	*task = NULL;

	if ( CgiExecutor::pendingTasks.empty() )
		return 0;
	while ( i < max && ( task = getTimeoutedTask( to ) ) )
	{
		CgiExecutor::pendingTasks.eraseTask( task->getPid() );
		i++;
	}
	return ( i );
}

void	CgiExecutor::checkCompletedTasks( void )
{
	PendingCgiTask	*pTask = NULL;
	Client			*cli = NULL;
	Request			*req = NULL;

	while ( ( pTask = CgiExecutor::getCompletedTask() ) )
	{
		// Log::Info( "Cgi Task completed" );
		req = &pTask->getRequest();
		pTask->applyTaskOutputToReq();
		CgiExecutor::pendingTasks.eraseTask(pTask->getPid());
		cli = req->getClient();
		req->setError( HTTP_OK_CODE );
		req->setReadyToSend();
		if ( cli != NULL )
			cli->allowPollWrite( true );
	}
}

void	CgiExecutor::checkTimeoutedTasks( void )
{
	PendingCgiTask	*pTask = NULL;
	Client			*cli = NULL;

	if ( ( pTask = CgiExecutor::getTimeoutedTask( CGI_TO ) ) != NULL )
	{
		Request& req = pTask->getRequest();
		pTask->isTimeout( CGI_TO, true );
		// Log::Error( "Timeout of Req: " + SUtils::longToString( req.getId() )
		// 	+ " process id: " + SUtils::longToString( pTask->getPid() ) );
		pTask->killPendingTask();
		CgiExecutor::pendingTasks.eraseTask( pTask->getPid() );
		req.setError( HTTP_GATEWAY_TIMEOUT_CODE );
		Router::checkErrorRedir( req.getError(), req );
		Router::checkErrorBody( req, req.getError() );
		req.setReadyToSend();
		cli = req.getClient();		
		if ( cli != NULL )
			cli->allowPollWrite( true );
	}
}

void	CgiExecutor::checkMarkedToDeleteTasks( void )
{
	PendingCgiTask	*pTask = NULL;

	if ( CgiExecutor::pendingTasks.size() > 0 \
		&& ( pTask = CgiExecutor::getMarkedToDeleteTask() ) != NULL ) 
	{
		Log::Error( "Delete Marked pending task for pid: " \
			+ SUtils::longToString( pTask->getPid() ) );
		CgiExecutor::pendingTasks.eraseTask( pTask->getPid() );
	}
}

void	CgiExecutor::attendPendingCgiTasks( void )
{
	checkCompletedTasks();
	checkTimeoutedTasks();
	checkMarkedToDeleteTasks();
}

size_t	CgiExecutor::getPendingTasksSize( void )
{
	return ( CgiExecutor::pendingTasks.size() );
}

int CgiExecutor::purgeDiscardedRequest(Request *req)
{
	pendingTasks.eraseTask(req);
    return 0;
}

pid_t CgiExecutor::findClientPendingPid(Client *cli)
{
	pid_t pid = pendingTasks.findPid(cli);
	if (pid > 0)
		return (pid);
    return 0;
}

void	CgiExecutor::pushEnvVar( const std::string& variable, \
								const std::string& value )
{
	this->envVars.push_back( variable + "=" + value );
}
