/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:34:13 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/31 19:40:26 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

namespace PUtils
{
	void	printInAscii( const char *str )
	{
		for ( uint32_t i = 0; str[ i ] != '\0'; i++ )
		{
			std::cout << static_cast< int >( str[ i ] );
			if ( str[ i ] != '\n' && str[ i + 1 ] != '\0' )
				std::cout << " ";
			else if ( str[ i + 1 ] != '\0' )
				std::cout << "\n";
		}
		std::cout << std::endl;
	}
}

namespace Log
{
	void	Info( const std::string str )
	{
		std::cout << YELLOW << "Info: "<< str << DEF << std::endl;
	}
	
	void	Error( const std::string str )
	{
		std::cout << RED << "Error: " << str << DEF << std::endl;
	}

	void	Success( const std::string str )
	{
		std::cout << GREEN << "Success: "<< str << DEF << std::endl;
	}	
}

namespace SUtils
{
	std::string	longToString( long int num )
	{
		std::stringstream	ss;
	
		ss << num;
		return ( ss.str() );
	}
}

namespace Binary
{
	void	printInOctets( unsigned int num )
	{
		unsigned int	iter;
		int				i;

		iter = 0;
		i = 24;
		while ( i >= 0 )
		{
			for ( int j = 7; j >= 0; j-- )
				std::cout << ( ( ( num >> i ) >> j ) & 1 );
			if ( i - 8 >= 0 )
				std::cout << ".";
			i -= 8;
		}
		std::cout << std::endl;
	}
}
