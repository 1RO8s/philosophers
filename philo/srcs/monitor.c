/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 04:18:17 by hnagasak          #+#    #+#             */
/*   Updated: 2024/07/01 21:22:35 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h>
#include <unistd.h> // for usleep

// check if philos is fullfilled
static int	is_fullfilled(t_config *config, size_t fullfilled_philos)
{
	if (fullfilled_philos == config->num_of_philo)
	{
		update_is_anyone_dead(config, FULLFILLED);
		return (1);
	}
	return (0);
}

static void	*monitor(void *args)
{
	t_config	*config;
	size_t		i;
	size_t		fullfilled_philos;
	t_philo		*philos;

	config = (t_config *)args;
	philos = config->philos;
	while (1)
	{
		fullfilled_philos = 0;
		i = 0;
		while (i < config->num_of_philo)
		{
			if (philo_is_dead(&philos[i]))
				return (NULL);
			if (read_eat_count(&philos[i]) >= config->required_eat_count)
				fullfilled_philos++;
			i++;
		}
		if (is_fullfilled(config, fullfilled_philos))
			return (NULL);
	}
	return (NULL);
}

void	start_monitor(pthread_t *monitor_thread, t_config *config)
{
	if (pthread_create(monitor_thread, NULL, monitor, config) != 0)
		mutex_message(config, "Error: failed to create monitor thread\n");
}
