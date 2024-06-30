/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 20:00:31 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/30 21:04:44 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	if (pthread_mutex_init(mutex, attr) != 0)
	{
		perror("Failed to initialize mutex");
		return (1);
	}
	return (0);
}

void	update_last_eat_time(t_philo *philo)
{
		// 可能性あり
	printf("5\n");
	// 可能性あり
	pthread_mutex_lock(&philo->last_eat_time_mutex);
		printf("6\n");

	gettimeofday(&philo->last_eat_timeval, NULL);
		printf("7\n");

		// 可能性あり
	pthread_mutex_unlock(&philo->last_eat_time_mutex);
		printf("8\n");
}

void update_eat_count(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_count_mutex);
	philo->eat_count += 1;
	pthread_mutex_unlock(&philo->eat_count_mutex);
}

size_t read_eat_count(t_philo *philo)
{
	size_t eat_count;
	
	pthread_mutex_lock(&philo->eat_count_mutex);
	eat_count = philo->eat_count;
	pthread_mutex_unlock(&philo->eat_count_mutex);
	return eat_count;
}
