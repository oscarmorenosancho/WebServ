/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:40:55 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 12:20:45 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERCONFIG_HPP
# define _SERVERCONFIG_HPP
# include <vector>
# include <Location.hpp>
# include <RootDir.hpp>

class ServerConfig
{
private:
	std::vector<unsigned int>	ports;
	std::vector<Location>		locations;
	RootDir						rootDir;
public:
	ServerConfig();
	~ServerConfig();
	ServerConfig(const ServerConfig& b);
	ServerConfig& operator=(const ServerConfig& b);
};

#endif