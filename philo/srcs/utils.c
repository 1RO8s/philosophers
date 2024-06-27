/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 20:00:31 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/27 20:01:41 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	if (pthread_mutex_init(mutex, attr) != 0)
	{
		perror("Failed to initialize mutex");
		return (1);
	}
	return (0);
}
