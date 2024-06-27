/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:21:22 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/27 20:05:08 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// int	read_is_anyone_dead(t_config *config)
// {
// 	int	value;

// 	pthread_mutex_lock(&config->is_anyone_dead_mutex);
// 	value = config->is_anyone_dead;
// 	pthread_mutex_unlock(&config->is_anyone_dead_mutex);
// 	return (value);
// }

// void	update_is_anyone_dead(t_config *config, int value)
// {
// 	pthread_mutex_lock(&config->is_anyone_dead_mutex);
// 	config->is_anyone_dead = value;
// 	pthread_mutex_unlock(&config->is_anyone_dead_mutex);
// }

// int	all_philos_eat_enough(t_config *config)
// {
// 	size_t	i;
// 	size_t	num_of_philo;
// 	t_philo	*philos;

// 	philos = config->philos;
// 	num_of_philo = config->num_of_philo;
// 	i = 0;
// 	while (i < num_of_philo)
// 	{
// 		if (philos[i].eat_count < config->required_eat_count)
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// /**
//  * @brief Checks if a philosopher has died.
//  * @param philo A pointer to the philosopher structure.
//  * @return 1 if the philosopher has died, 0 otherwise.
//  */
// int	philo_is_dead(t_philo *philo)
// {
// 	t_timeval	last_eat_time;
// 	size_t		time_to_die;
// 	long		from_last_eat;

// 	last_eat_time = philo->last_eat_timeval;
// 	from_last_eat = get_elapsed_usec(last_eat_time);
// 	time_to_die = philo->config->time_to_die;
// 	if (from_last_eat > (long)time_to_die)
// 	{
// 		update_is_anyone_dead(philo->config, DIED);
// 		mutex_print(philo, DEAD);
// 		return (1);
// 	}
// 	return (0);
// }

// int	should_stop(t_philo *philo)
// {
// 	t_config	*config;

// 	config = philo->config;
// 	if (read_is_anyone_dead(config) != CONTINUE)
// 		return (1);
// 	return (0);
// }

void	start_pthreads(pthread_t *pthreads, t_philo *philos,
		pthread_t *th_monitor, t_config *config)
{
	start_monitor(th_monitor, config);
	start_philos(pthreads, philos, config->num_of_philo);
}

void	wait_for_threads(pthread_t *pthreads, size_t num_of_philo,
		pthread_t th_monitor)
{
	size_t	i;

	i = 0;
	while (i < num_of_philo)
	{
		if (pthread_join(pthreads[i], NULL) != 0)
			printf("Error: Failed to join philo thread\n");
		i++;
	}
	if (pthread_join(th_monitor, NULL) != 0)
		printf("Error: Failed to join monitor thread\n");
}

int	is_invalid_argument(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	if (atoi(argv[1]) < 2 || atoi(argv[1]) > 200)
	{
		printf("Error: Number of philosophers must be between 2 and 200\n");
		return (1);
	}
	if (atoi(argv[2]) < 60 || atoi(argv[3]) < 60 || atoi(argv[4]) < 60)
	{
		printf("Error: Time to die, eat, and sleep must be at least 60ms\n");
		return (1);
	}
	if (argc == 6 && atoi(argv[5]) < 1)
	{
		printf("Error: Required eat count must be at least 1\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_t	*pthreads;
	pthread_t	th_monitor;
	t_config	conf;

	if (is_invalid_argument(argc, argv))
		return (EXIT_FAILURE);
	init_config(argc, argv, &conf);
	pthreads = (pthread_t *)malloc(sizeof(pthread_t) * conf.num_of_philo);
	if (pthreads == NULL)
	{
		printf("Error: Failed to allocate memory for pthreads array\n");
		free(pthreads);
		free_config(&conf);
		return (EXIT_FAILURE);
	}
	start_pthreads(pthreads, conf.philos, &th_monitor, &conf);
	wait_for_threads(pthreads, conf.num_of_philo, th_monitor);
	free(pthreads);
	free_config(&conf);
	print_result(&conf);
	return (0);
}
