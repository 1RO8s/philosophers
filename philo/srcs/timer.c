/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 09:01:29 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/30 20:27:25 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

long	us2ms(long usec)
{
	return (usec);
}

t_timeval	us2timeval(long usec)
{
	struct timeval	time;

	time.tv_sec = usec / 1000000;
	time.tv_usec = usec % 1000000;
	return (time);
}

void	ft_sleep(t_config *config, long msec)
{
	t_timeval	tmp = config->start;
	long	start;
	long	current;

	// start = get_elapsed_msec(config->start);
	start = get_elapsed_msec(tmp);

	while (1)
	{
		// current = get_elapsed_msec(config->start);
		current = get_elapsed_msec(tmp);

		if (current - start >= msec)
			break ;
		usleep(100);
	}
}

long	get_elapsed_msec(t_timeval start)
{
	t_timeval	current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec * 1000) + (current.tv_usec / 1000) - (start.tv_sec
			* 1000) - (start.tv_usec / 1000));
}
