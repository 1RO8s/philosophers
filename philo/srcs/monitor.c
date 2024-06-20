/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 04:18:17 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/21 04:51:10 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h> // for printf
#include <unistd.h> // for usleep

void *monitor(void *args)
{
	// int *result;
	t_config *config;
	size_t i;
	// size_t num_of_philo;
	size_t fullfilled_philos;
	t_philo *philos;

	// result = (int *)malloc(sizeof(int));
	

	
	config = (t_config *)args;
	philos = config->philos;
	// mutex_message(config,"--- begin monitor thread!! ---\n");
	// num_of_philo = config->num_of_philo;
	// print is_anyone_dead_mutex address of monitor
	// printf("monitor   is_anyone_dead_mutex address: %p\n", config->is_anyone_dead_mutex);
	while (1)
	{
		// 各哲学者の状態を監視する
		fullfilled_philos = 0;
		i = 0;
		while (i < config->num_of_philo)
		{
			// i番目の哲学者が死んでいたらDIEDを返す
			if (philo_is_dead(&philos[i]))
			{
				update_is_anyone_dead(config, DIED);
				mutex_print(&philos[i], DEAD);
				// *result = DIED;
				return (NULL);
			}
			// i番目の哲学者が必要な回数だけ食事を終えていたらfullfilled_philosをインクリメント
			if (philos[i].eat_count >= config->required_eat_count)
				fullfilled_philos++;
			i++;
		}
		if (fullfilled_philos == config->num_of_philo)
		{
			update_is_anyone_dead(config, FULLFILLED);
			// *result = FULLFILLED;
			// mutex_message(config, "All philos are fullfilled\n");
			return (NULL);
		}
		// usleep(100);
	}
	mutex_message(config, "monitor thread end\n");
	return (NULL);
}


void start_monitor(pthread_t	*monitor_thread, t_config *config)
{
	// monitor_thread = (pthread_t *)malloc(sizeof(pthread_t));
	if(pthread_create(monitor_thread, NULL, monitor, config) != 0)
	{
		mutex_message(config, "Error: failed to create monitor thread\n");
	}
	else
	{
		// mutex_message(config, "Success: created monitor thread\n");
	}
}