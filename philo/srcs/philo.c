/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:20:34 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/30 21:02:37 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	waiting_for_forks(t_philo *philo, pthread_mutex_t *fork1,
		pthread_mutex_t *fork2)
{
	take_1st_fork(philo, fork1);
	if (fork1 == fork2)
	{
		while (!should_stop(philo))
			ft_sleep(philo->config, 10);
		return ;
	}
	take_2nd_fork(philo, fork2);
}

static void	wait_for_forks(t_philo *philo)
{
	t_config	*config;
	int			total_philos;

	config = philo->config;
	total_philos = config->num_of_philo;
	if (philo->id % 2 == 1)
	{
		if (get_elapsed_msec(philo->config->start) < 50)
			ft_sleep(config, 10);
	}
	if (total_philos % 2 == 1 && philo->id == total_philos - 1)
	{
		if (get_elapsed_msec(philo->config->start) < 50)
			ft_sleep(config, 10);
	}
	if (philo->id % 2 == 0)
		waiting_for_forks(philo, philo->left_fork, philo->right_fork);
	else
		waiting_for_forks(philo, philo->left_fork, philo->right_fork);
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
	update_eat_count(philo);
	return (0);
}

static void	*handle_philo_actions(void *args)
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
