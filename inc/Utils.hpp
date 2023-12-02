/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:31:42 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/02 11:57:38 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _UTILS_HPP_
# define _UTILS_HPP_

# include <string>
# include <iostream>
# include <sstream>
# include <vector>

# include <Defines.hpp>

namespace PUtils
{
	void	printInAscii( const char *str );
}

namespace SUtils
{
	std::string					longToString( long int num );
	bool						isNum( std::string num );
	std::string					trim( const std::string str );
	std::string					leftTrim( const std::string str );
	std::string					rightTrim( const std::string str );
	std::vector< std::string >&	split( std::vector<std::string>& v, \
									std::string strArr, \
									std::string delimiter );
	std::vector< std::string >	splitOnce( std::string str, \
									std::string del );
	int							compareNumbersAsStrings( \
									const std::string num1, \
									const std::string num2 );
}

namespace STLUtils
{
	template < typename T >
	std::string	vectorToString( typename T::iterator begin, typename T::iterator end );
	template < typename T >
	std::string	mapToString( typename T::iterator begin, typename T::iterator end );
}

namespace Log
{
	void	Info( const std::string str );
	void	Error( const std::string str );
	void	Success( const std::string str );
}

namespace Binary
{
	std::string	formatBits( std::string bits );
	std::string	decodeAddress( uint32_t address );
	uint32_t	codeAddress( std::string address );
}

#include "Utils.ipp"

#endif
