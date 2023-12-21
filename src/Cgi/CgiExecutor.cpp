/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:11 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/21 15:42:42 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "Router.hpp"
#include "Client.hpp"
#include "CgiExecutor.hpp"
#define FDIN 0
#define FDOUT 1

PendingCgiTasks	CgiExecutor::pendingTasks;

CgiExecutor::CgiExecutor( Request& request ): request( request )
{
	std::string		host;
	std::string		port;
	const Server	*s = NULL;

	// request.getClient()->cgis.findCgi( request.getDocExt(), binary );
	Client *cli = request.getClient();
	request.getHostPort( host, port );
	s = ServerFinder::find( cli->getServers(), host, port );
	if (!s)
		return;
	Log::Success("CgiExecutor::CgiExecutor Route Chaine: " + request.getRouteChaineString() );
	binary = s->getCgiBinary( request.getDocExt(), request.getRouteChaineString() );
	// cli->servers[]
	this->argument = "." + request.getRouteChaineString() + request.getDocument();
	Log::Info( "CgiExecutor binary: " + this->binary );
	Log::Info( "CgiExecutor argment: " + this->argument );
	// char				*envPath;
	argv[0] = (char *)this->binary.c_str();
	argv[1] = (char *)this->argument.c_str();
	argv[2] = NULL;
	childEnv = NULL;
}

CgiExecutor::~CgiExecutor()
{
	envVars.clear();
	if (childEnv)
		delete childEnv;
}

void	CgiExecutor::onFailFork( void )
{
		Log::Error( "fork: failed create child process" );
		close( this->fdToChild[ FDIN ] );
		close( this->fdToChild[ FDOUT ] );
		close( this->fdFromChild[ FDIN ] );
		close( this->fdFromChild[ FDOUT ] );
		throw ;
}

void	CgiExecutor::onFailToChildPipeOpen( void )
{
	Log::Error( "pipe: failed to open pipe to child" );
	throw ;
}

void	CgiExecutor::onFailFromChildPipeOpen( void )
{
	Log::Error( "pipe: failed to open pipe from child" );
	close( this->fdToChild[ FDIN ] );
	close( this->fdToChild[ FDOUT ] );
	throw ;
}

char** CgiExecutor::getEnvVarList( void )
{
	std::vector<std::string>::iterator it = envVars.begin();
	std::vector<std::string>::iterator ite = envVars.end();
	size_t i = 0;
	childEnv = new char*[envVars.size() + 1];
	while (it != ite)
	{
		childEnv[i] = (char*)it->c_str();
		it++;
		i++;
	}
	childEnv[i] = 0;
	return childEnv;
}

void	CgiExecutor::onChildProcess( void )
{
	//On child
	getEnvVarList();
	close(fdToChild[FDOUT]);
	close(fdFromChild[FDIN]);
	if (dup2(fdToChild[FDIN], FDIN) < 0)
		Log::Error(std::string("dup2: fdToChild[0] to 0 failed: " + SUtils::longToString(errno)));
	if (dup2(fdFromChild[FDOUT], FDOUT) < 0)
		Log::Error(std::string("dup2: fdFromChild[1] to 1 failed: " + SUtils::longToString(errno)));
	close(fdToChild[FDIN]);
	close(fdFromChild[FDOUT]);
	execve(binary.c_str(), argv, childEnv);
	close(FDIN);
	close(FDOUT);
	Log::Error(std::string("execve: Not found binary ") + binary);
	return; //exit(1) not alloed?
}

void	CgiExecutor::onParentProcess( pid_t childPid )
{
	std::string	body = request.getBody();
	// body = std::string("Dommy body");
	char *reqBody = (char *)body.c_str();
	size_t reqBodySize = body.length();
	close(fdToChild[FDIN]);
	close(fdFromChild[FDOUT]);
	write(fdToChild[FDOUT], reqBody, reqBodySize);
	close(fdToChild[FDOUT]);
	PendingCgiTask task(childPid, request, fdFromChild[FDIN]);
	pendingTasks.appendTask(task);
	CgiExecutor::attendPendingCgiTasks();
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
	// Log::Success("Pipe To Child Created on fds: " + SUtils::longToString(fdToChild[0]) + "," + SUtils::longToString(fdToChild[1]));
	if ( pipe( this->fdFromChild ) )
		onFailFromChildPipeOpen();
	// Log::Success("Pipe From Child Created on fds: " + SUtils::longToString(fdFromChild[0]) + "," + SUtils::longToString(fdFromChild[1]));
	// Log::Success("Request " + SUtils::longToString (request.getClient()->getId()));
	// Log::Success("with client " + SUtils::longToString (request.getClient()->getId()));
	// Log::Success("using socket " + SUtils::longToString (request.getClient()->getClientSocket()));
	pid = fork();
	if ( pid < 0 )
		onFailFork();
	if ( pid == 0 )
		onChildProcess();
	onParentProcess( pid );
	return ( 0 );
	// return getChildOutput();
}

PendingCgiTask	*CgiExecutor::getCompletedTask( void )
{
	pid_t	pid;
	pid = waitpid(-1, NULL, WNOHANG);
	if (pid < 1)
		return NULL;
	if (pendingTasks.empty())
		return NULL;
	PendingCgiTask& tk = pendingTasks[pid];
	if (!tk.isMarkedToDelete())
		return (&tk);
	return NULL;
}

PendingCgiTask *CgiExecutor::getTimeoutedTask(double to)
{
	PendingCgiTasks::iterator it = pendingTasks.begin();
	PendingCgiTasks::iterator ite= pendingTasks.end();
	if (pendingTasks.empty())
		return NULL;
	while (it != ite)
	{
		if(!it->second.isMarkedToDelete() && it->second.isTimeout(to, false))
			return (&(it->second));
		it++;
	}
	return ( NULL );
}

PendingCgiTask *CgiExecutor::getMarkedToDeleteTask()
{
	PendingCgiTasks::iterator it = pendingTasks.begin();
	PendingCgiTasks::iterator ite= pendingTasks.end();
	if (pendingTasks.empty())
		return NULL;
	while (it != ite)
	{
		if(it->second.isMarkedToDelete())
			return (&(it->second));
		it++;
	}
	return NULL;
}

size_t	CgiExecutor::purgeTimeoutedTasks(double to, size_t max)
{
	size_t i = 0;
	PendingCgiTask *task = NULL;
	if (pendingTasks.empty())
		return 0;
	while (i < max && (task = getTimeoutedTask(to)))
	{
		CgiExecutor::pendingTasks.eraseTask( task->getPid() );
		i++;
	}
	return ( i );
}

void	CgiExecutor::attendPendingCgiTasks( void )
{
	PendingCgiTask* pTask; 
	Client* cli = NULL;
	while ((pTask = CgiExecutor::getCompletedTask()))
	{
		Log::Info( "Cgi Task completed" );
		Request&	req = pTask->getRequest();
		pTask->applyTaskOutputToReq();
		CgiExecutor::pendingTasks.eraseTask(pTask->getPid());
		// Log::Info("CgiExecutor::attendPendingCgiTasks got:");
		// Log::Info("Request addr: " + SUtils::longToString((long)&req));
		// Log::Info("Request id: " + SUtils::longToString((long)req.
		// getId()));
		// req->getDocExt();
		cli = req.getClient();
		//req->setCgiOutput(pTask->getTaskOutput());
		// req.logStatus();
		// Log::Info("Set Cgi Request " + SUtils::longToString(req.getId()) + " ReadyToSend");
		req.setReadyToSend();
		// req.logStatus();
		if (cli != NULL)
		 	cli->allowPollWrite(true);
		// cli = NULL;
	}
	cli = NULL;
	if ((pTask = CgiExecutor::getTimeoutedTask(CGI_TO)))
	{
		Request& req = pTask->getRequest();
		pTask->isTimeout(CGI_TO, true);
		Log::Error("Timeout of Req: " + SUtils::longToString(req.getId()) + " process id: " + SUtils::longToString(pTask->getPid()));
		pTask->killPendingTask();
		CgiExecutor::pendingTasks.eraseTask(pTask->getPid());
		req.setUseCgi(false);
		req.setError(500);
		req.setReadyToSend();
		cli = req.getClient();		
		if (cli != NULL)
			cli->allowPollWrite(true);
		cli = NULL;
	}
	if ((pendingTasks.size() > 0) && (pTask = CgiExecutor::getMarkedToDeleteTask())) 
	{
		Log::Error("Delete Marked pending task for pid: " + SUtils::longToString(pTask->getPid()));
		CgiExecutor::pendingTasks.eraseTask(pTask->getPid());
	}
}

size_t CgiExecutor::getPendingTasksSize()
{
	return pendingTasks.size();
}

void CgiExecutor::pushEnvVar(const std::string& variable, const std::string& value)
{
	envVars.push_back(variable + "=" + value);
}
