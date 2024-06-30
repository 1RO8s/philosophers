/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:21:22 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/30 14:23:07 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	start_pthreads(pthread_t *pthreads, t_philo *philos,
		pthread_t *th_monitor, t_config *config)
{
	start_philos(pthreads, philos, config->num_of_philo);
	start_monitor(th_monitor, config);
}

static void	wait_for_threads(pthread_t *pthreads, size_t num_of_philo,
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

static int	is_invalid_argument(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Error: number of arguments must be 4 or 5\n");
		return (1);
	}
	if (atoi(argv[1]) < 1 || atoi(argv[1]) > 200)
	{
		printf("Error: Number of philosophers must be 1 to 200\n");
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
	return (0);
}
