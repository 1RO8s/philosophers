/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:21:22 by hnagasak          #+#    #+#             */
/*   Updated: 2024/05/14 01:26:02 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	print_config(t_config *config)
{
	printf("--- print_config ---\n");
	printf("num_of_philo: %zu\n", config->num_of_philo);
	printf("time_to_die: %zu\n", config->time_to_die);
	printf("time_to_eat: %zu\n", config->time_to_eat);
	printf("time_to_sleep: %zu\n", config->time_to_sleep);
	printf("required_eat_count: %zu\n", config->required_eat_count);
	printf("start_time: %ld usec\n", get_elapsed_usec(config->start));
	printf("\n");
}

void	print_philo(t_philo *data)
{
	printf("%ld philos[%d] is actitvated: left->%p right->%p:\n",
		get_elapsed_usec(data->config->start), data->id, data->left_fork,
		data->right_fork);
}

void	print_forks(pthread_mutex_t **forks)
{
	int	i;

	printf("--- print_forks ---\n");
	i = 0;
	while (forks[i] != NULL && i < 10)
	{
		printf("forks[%d]: %p\n", i, forks[i]);
		i++;
	}
	printf("\n");
}

void	print_philos_forks(t_philo *data, size_t num_of_philo)
{
	size_t	i;

	printf("--- print_philos_forks ---\n");
	i = 0;
	while (i < num_of_philo)
	{
		printf("philo[%d]: left->%p right->%p\n", data[i].id, data[i].left_fork,
			data[i].right_fork);
		i++;
	}
	printf("\n");
}
