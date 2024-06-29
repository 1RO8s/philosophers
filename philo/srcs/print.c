/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 19:53:53 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/30 01:40:37 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	mutex_message(t_config *config, char *message)
{
	pthread_mutex_lock(&config->print_mutex);
	printf("%s", message);
	pthread_mutex_unlock(&config->print_mutex);
}

void	mutex_print(t_philo *philo, t_status status)
{
	long	elapsed_msec;
	int		philo_num;

	philo_num = philo->id + 1;
	pthread_mutex_lock(&philo->config->print_mutex);
	elapsed_msec = us2ms(get_elapsed_usec(philo->config->start));
	if (status == DEAD)
		printf("%ld\t%d died\n", elapsed_msec, philo_num);
	else if (status == EATING)
		printf("%ld\t%d is eating\n", elapsed_msec, philo_num);
	else if (status == SLEEPING)
		printf("%ld\t%d is sleeping\n", elapsed_msec, philo_num);
	else if (status == THINKING)
		printf("%ld\t%d is thinking\n", elapsed_msec, philo_num);
	else if (status == TAKE_FORK)
		printf("%ld\t%d has taken a fork\n", elapsed_msec, philo_num);
	else if (status == TEST)
		printf("%ld\t%d test\n", elapsed_msec, philo_num);
	pthread_mutex_unlock(&philo->config->print_mutex);
}
