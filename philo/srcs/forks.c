/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 11:13:54 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/30 01:28:42 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>

static pthread_mutex_t	*init_fork(pthread_mutex_t **forks, size_t i)
{
	forks[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (forks[i] == NULL)
	{
		printf("Error: Failed to allocate memory for mutex\n");
		free_forks(forks, i);
		return (NULL);
	}
	if (mutex_init(forks[i], NULL) != 0)
	{
		free(forks[i]);
		free_forks(forks, i);
		return (NULL);
	}
	return (forks[i]);
}

pthread_mutex_t	**init_forks(size_t num)
{
	pthread_mutex_t	**forks;
	size_t			i;

	forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * num);
	if (forks == NULL)
	{
		printf("Error: Failed to allocate memory for forks array\n");
		return (NULL);
	}
	i = 0;
	while (i < num)
	{
		if (init_fork(forks, i) == NULL)
			return (NULL);
		i++;
	}
	return (forks);
}

int	take_1st_fork(t_philo *philo, pthread_mutex_t *fork1)
{
	if (pthread_mutex_lock(fork1) != 0)
	{
		printf("Error: Failed to lock fork1[%p]\n", fork1);
		return (0);
	}
	if (should_stop(philo))
	{
		pthread_mutex_unlock(fork1);
		return (1);
	}
	mutex_print(philo, TAKE_FORK);
	return (0);
}

int	take_2nd_fork(t_philo *philo, pthread_mutex_t *fork2)
{
	if (pthread_mutex_lock(fork2) != 0)
	{
		printf("Error: Failed to lock fork2[%p]\n", fork2);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	if (should_stop(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(fork2);
		return (1);
	}
	mutex_print(philo, TAKE_FORK);
	return (0);
}

void	put_forks(pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	pthread_mutex_unlock(fork1);
	pthread_mutex_unlock(fork2);
}
