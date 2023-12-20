/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:34 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/20 18:41:24 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CGIEXECUTOR_HPP_
# define _CGIEXECUTOR_HPP_

# include <string>
# include <vector>
# include <unistd.h>
# include <sys/wait.h>

# include <cerrno>
# include <ctime>

# include "Utils.hpp"
# include "Request.hpp"
# include "PendingCgiTask.hpp"
# include "PendingCgiTasks.hpp"
# include "Router.hpp"
# include "Client.hpp"
# include <ServerFinder.hpp>

# define FDIN 0
# define FDOUT 1
# define CGI_TO 2

class CgiExecutor
{
private:
	static PendingCgiTasks	pendingTasks;
	std::string				binary;
	std::string				argument;
	std::vector<std::string>	envVars;
	Request&				request;
	//char					*envPath;
	int						fdToChild[2];
	int						fdFromChild[2];
	char					*argv[3];
	char					**childEnv;
	static std::string		getChildOutput(PendingCgiTask *task);
	void					onFailFork(void);
	void					onFailToChildPipeOpen(void);
	void					onFailFromChildPipeOpen(void);
	char **					getEnvVarList(void);
	void 					onChildProcess(void);
	void					onParentProcess(pid_t childPid);
public:
	CgiExecutor(Request& request);
	~CgiExecutor();
	int execute(void);
	static PendingCgiTask*	getCompletedTask();
	static PendingCgiTask*	getTimeoutedTask(double to);
	static PendingCgiTask*	getMarkedToDeleteTask();
	static size_t			purgeTimeoutedTasks(double to, size_t max);
	static void				attendPendingCgiTasks(void);
	static size_t			getPendingTasksSize();
	void					pushEnvVar(const std::string& variable, const std::string& value);
};

#endif
