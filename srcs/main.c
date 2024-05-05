/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:21:22 by hnagasak          #+#    #+#             */
/*   Updated: 2024/05/05 23:20:33 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	print_config(t_config *config)
{
	printf("--- print_config ---\n");
	printf("num_of_philo: %zu\n", config->num_of_philo);
	printf("time_to_die: %zu\n", config->time_to_die);
	printf("time_to_eat: %zu\n", config->time_to_eat);
	printf("time_to_sleep: %zu\n", config->time_to_sleep);
	printf("required_eat_count: %zu\n", config->required_eat_count);
	printf("\n");
}

void	*handle_philo_actions(void *args)
{
	t_philo	*data;

	data = (t_philo *)args;
	// printf("philos[%d] is actitvated: left->%p right->%p:\n", data->id,
	// 	data->left_fork, data->right_fork);
	while (1)
	{
		// フォークを取る
		pthread_mutex_lock(data->left_fork);
		pthread_mutex_lock(data->right_fork);
		printf("philos[%d] is eating\n", data->id);
		usleep(data->config->time_to_eat * 1000);
		pthread_mutex_unlock(data->left_fork);
		pthread_mutex_unlock(data->right_fork);
		break;
		// 両手にフォークを持ったら食事を開始
		// 食事が終わったらフォークを置いて睡眠
	}
	return (NULL);
}

t_config	*init_config(int argc, char **argv, t_config *config)
{
	config->num_of_philo = atoi(argv[1]);
	config->time_to_die = atoi(argv[2]);
	config->time_to_eat = atoi(argv[3]);
	config->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		config->required_eat_count = atoi(argv[5]);
	else
		config->required_eat_count = -1;
	// config->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
	// 		* config->num_of_philo);
	print_config(config);
	return (config);
}

void	print_forks(pthread_mutex_t **forks)
{
	int	i;

	i = 0;
	printf("--- print_forks ---\n");
	while (forks[i] != NULL && i < 10)
	{
		printf("forks[%d]: %p\n", i, forks[i]);
		i++;
	}
	printf("\n");
}

pthread_mutex_t	**init_forks(size_t num_of_philo)
{
	pthread_mutex_t	**forks;
	size_t			i;

	forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * (num_of_philo
				+ 1));
	i = 0;
	while (i < num_of_philo)
	{
		forks[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (forks[i] == NULL || pthread_mutex_init(forks[i], NULL) != 0)
			perror("Failed to initialize mutex");
		i++;
	}
	forks[i] = (pthread_mutex_t *)NULL;
	print_forks(forks);
	return (forks);
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

void	wait_for_threads(pthread_t *pthreads, size_t num_of_philo)
{
	size_t	i;

	i = 0;
	while (i < num_of_philo)
	{
		pthread_join(pthreads[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	pthread_t		*pthreads;
	t_config		conf;
	t_philo			*data;
	pthread_mutex_t	**forks;

	if (is_invalid_argument(argc, argv))
		return (1);
	// initialize
	init_config(argc, argv, &conf);
	forks = init_forks(conf.num_of_philo);
	pthreads = (pthread_t *)malloc(sizeof(pthread_t) * conf.num_of_philo);
	data = (t_philo *)malloc(sizeof(t_philo) * conf.num_of_philo);
	for (size_t i = 0; i < conf.num_of_philo; i++)
	{
		data[i].id = i;
		data[i].left_fork = forks[i];
		data[i].right_fork = forks[(i + 1) % conf.num_of_philo];
		data[i].eat_count = 0;
		data[i].last_eat = 0;
		data[i].config = &conf;
	}
	// start threads
	for (size_t i = 0; i < conf.num_of_philo; i++)
	{
		if (pthread_create(&pthreads[i], NULL, &handle_philo_actions,
				&data[i]) != 0)
			perror("Failed to create thread");
	}
	wait_for_threads(pthreads, conf.num_of_philo);
	printf("complete\n");
	return (0);
}
