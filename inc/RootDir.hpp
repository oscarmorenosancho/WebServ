/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootDir.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:01:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 15:48:33 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ROOTDIR_HPP_
# define _ROOTDIR_HPP_
# include <string>

class RootDir
{
private:
	std::string	path;
public:
	RootDir();
	RootDir(std::string head, std::string body);
	RootDir(std::string	path);
	~RootDir();
	RootDir(const RootDir& b);
	RootDir& 		operator=(const RootDir& b);
};

#endif
