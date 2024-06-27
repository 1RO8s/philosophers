/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 20:04:40 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/27 20:05:47 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	read_is_anyone_dead(t_config *config)
{
	int	value;

	pthread_mutex_lock(&config->is_anyone_dead_mutex);
	value = config->is_anyone_dead;
	pthread_mutex_unlock(&config->is_anyone_dead_mutex);
	return (value);
}

void	update_is_anyone_dead(t_config *config, int value)
{
	pthread_mutex_lock(&config->is_anyone_dead_mutex);
	config->is_anyone_dead = value;
	pthread_mutex_unlock(&config->is_anyone_dead_mutex);
}

int	all_philos_eat_enough(t_config *config)
{
	size_t	i;
	size_t	num_of_philo;
	t_philo	*philos;

	philos = config->philos;
	num_of_philo = config->num_of_philo;
	i = 0;
	while (i < num_of_philo)
	{
		if (philos[i].eat_count < config->required_eat_count)
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Checks if a philosopher has died.
 * @param philo A pointer to the philosopher structure.
 * @return 1 if the philosopher has died, 0 otherwise.
 */
int	philo_is_dead(t_philo *philo)
{
	t_timeval	last_eat_time;
	size_t		time_to_die;
	long		from_last_eat;

	last_eat_time = philo->last_eat_timeval;
	from_last_eat = get_elapsed_usec(last_eat_time);
	time_to_die = philo->config->time_to_die;
	if (from_last_eat > (long)time_to_die)
	{
		update_is_anyone_dead(philo->config, DIED);
		mutex_print(philo, DEAD);
		return (1);
	}
	return (0);
}

int	should_stop(t_philo *philo)
{
	t_config	*config;

	config = philo->config;
	if (read_is_anyone_dead(config) != CONTINUE)
		return (1);
	return (0);
}
