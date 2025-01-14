/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventsTarget.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:04:03 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/12 14:16:35 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _EVENTSTARGET_HPP_
# define _EVENTSTARGET_HPP_

# include "Event.hpp"
# include "Events.hpp"

class EventsTarget
{
protected:
	Events	*evs;
public:
	EventsTarget( Events* evs );
	virtual ~EventsTarget( void );
	virtual int	onEvent( Event &tevent ) = 0;
};

#endif
