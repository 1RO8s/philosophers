/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:20:34 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/27 20:38:09 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	update_last_eat_time(t_philo *philo)
{
	gettimeofday(&philo->last_eat_timeval, NULL);
}

static int	eat(t_philo *philo)
{
	wait_for_forks(philo);
	if (should_stop(philo))
	{
		put_forks(philo->right_fork, philo->left_fork);
		return (1);
	}
	mutex_print(philo, EATING);
	update_last_eat_time(philo);
	ft_sleep(philo->config, philo->config->time_to_eat);
	put_forks(philo->right_fork, philo->left_fork);
	if (should_stop(philo))
		return (1);
	philo->eat_count++;
	return (0);
}

void	*handle_philo_actions(void *args)
{
	t_philo	*philo;
	size_t	i;

	philo = (t_philo *)args;
	update_last_eat_time(philo);
	i = 0;
	while (1 || i < 100)
	{
		if (eat(philo))
			break ;
		if (should_stop(philo))
			break ;
		mutex_print(philo, SLEEPING);
		ft_sleep(philo->config, philo->config->time_to_sleep);
		if (should_stop(philo))
			break ;
		mutex_print(philo, THINKING);
		i++;
	}
	return (NULL);
}

void	start_philos(pthread_t *pthreads, t_philo *data, size_t num_of_philo)
{
	size_t	i;

	i = 0;
	while (i < num_of_philo)
	{
		if (pthread_create(&pthreads[i], NULL, &handle_philo_actions,
				&data[i]) != 0)
			mutex_message(data[0].config, "Failed to create thread\n");
		i++;
	}
}
