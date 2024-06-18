/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 09:01:29 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/18 09:01:59 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


long	us2ms(long usec)
{
	return (usec);
	// return (usec / 1000);
}

t_timeval	us2timeval(long usec)
{
	struct timeval	time;

	time.tv_sec = usec / 1000000;
	time.tv_usec = usec % 1000000;
	return (time);
}