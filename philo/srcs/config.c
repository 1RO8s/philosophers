/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:47:50 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/30 21:13:47 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h>

static void	set_args(int argc, char **argv, t_config *config)
{
	config->num_of_philo = atoi(argv[1]);
	config->time_to_die = atoi(argv[2]);
	config->time_to_eat = atoi(argv[3]);
	config->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		config->required_eat_count = atoi(argv[5]);
	else
		config->required_eat_count = SIZE_MAX;
}

static void	init_philos(t_philo *philos, t_config *config)
{
	size_t	i;

	i = 0;
	while (i < config->num_of_philo)
	{
		philos[i].id = i;
		philos[i].right_fork = config->forks[i];
		if (i + 1 != config->num_of_philo)
			philos[i].left_fork = config->forks[i + 1];
		else
			philos[i].left_fork = config->forks[0];
		philos[i].eat_count = 0;
		philos[i].last_eat_timeval = config->start;
		philos[i].config = config;
		mutex_init(&philos[i].eat_count_mutex,NULL);
		mutex_init(&philos[i].last_eat_time_mutex,NULL);
		i++;
	}
}

t_config	*init_config(int argc, char **argv, t_config *config)
{
	pthread_mutex_t	**forks;
	t_philo			*philos;

	set_args(argc, argv, config);
	gettimeofday(&config->start, NULL);
	config->is_anyone_dead = 0;
	if (mutex_init(&config->print_mutex, NULL) != 0)
		return (NULL);
	if (mutex_init(&config->is_anyone_dead_mutex, NULL) != 0)
		return (NULL);
	forks = init_forks(config->num_of_philo);
	config->forks = forks;
	philos = (t_philo *)malloc(sizeof(t_philo) * config->num_of_philo);
	if (philos == NULL)
		return (NULL);
	init_philos(philos, config);
	config->philos = philos;
	return (config);
}

pthread_mutex_t	**free_forks(pthread_mutex_t **forks, size_t num)
{
	size_t	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
		i++;
	}
	free(forks);
	return (NULL);
}

void	free_config(t_config *config)
{
	pthread_mutex_destroy(&config->print_mutex);
	pthread_mutex_destroy(&config->is_anyone_dead_mutex);
	free(config->philos);
	free_forks(config->forks, config->num_of_philo);
}
