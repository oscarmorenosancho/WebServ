/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:34 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/22 13:10:54 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CGIEXECUTOR_HPP_
# define _CGIEXECUTOR_HPP_
# include <string>
# include <unistd.h>
# include <sys/wait.h>
# include <ctime>
# include "Utils.hpp"
# include "Request.hpp"
# include "PendingCgiTask.hpp"
# include "PendingCgiTasks.hpp"

class CgiExecutor
{
private:
	static PendingCgiTasks	pendingTasks;
	const std::string&		binary;
	const std::string&		argument;
	Request					request;
	//char					*envPath;
	int						fdToChild[2];
	int						fdFromChild[2];
	char					*argv[3];
	char					*childEnv[10];
	static std::string		getChildOutput(PendingCgiTask *task);
	void					onFailFork(void);
	void					onFailToChildPipeOpen(void);
	void					onFailFromChildPipeOpen(void);
	void					onChildProcess(void);
	void					onParentProcess(pid_t childPid);
public:
	CgiExecutor(const std::string& binary, const std::string& argument,
		Request& request, char **env);
	~CgiExecutor();
	int execute(void);
	static PendingCgiTask*	getCompletedTask();
	static PendingCgiTask*	getTimeoutedTask(clock_t to);
	static std::string		getCompletedTaskOutput(void);
	static size_t			purgeTimeoutedTasks(clock_t to, size_t max);
};

#endif