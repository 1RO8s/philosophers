/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 04:18:17 by hnagasak          #+#    #+#             */
/*   Updated: 2024/06/16 07:06:17 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

typedef enum e_monitor_result
{
	FULLFILLED,
	DIED,
	CONTINUE
}	t_monitor_result;

void *monitor(void *args)
{
	// int *result;
	t_config *config;
	// size_t i;
	// size_t num_of_philo;
	// size_t fullfilled_philos;
	// t_philo *philos;

	// result = (int *)malloc(sizeof(int));
	

	
	config = (t_config *)args;
	// philos = config->philos;
	mutex_message(config,"--- begin monitor thread!! ---\n");
	// num_of_philo = config->num_of_philo;
	// while (1)
	// {
	// 	// 各哲学者の状態を監視する
	// 	fullfilled_philos = 0;
	// 	i = 0;
	// 	while (i < num_of_philo)
	// 	{
	// 		return (NULL);
	// 		// i番目の哲学者が死んでいたらDIEDを返す
	// 		if (philo_is_dead(&philos[i]))
	// 		{
	// 			config->is_anyone_dead = 1;
	// 			mutex_print(&philos[i], DEAD);
	// 			*result = DIED;
	// 			return (result);
	// 		}
	// 		// i番目の哲学者が必要な回数だけ食事を終えていたらfullfilled_philosをインクリメント
	// 		if (philos[i].eat_count >= config->required_eat_count)
	// 			fullfilled_philos++;
	// 		i++;
	// 	}
	// 	if (fullfilled_philos == num_of_philo)
	// 	{
	// 		*result = FULLFILLED;
	// 		return (result);

	// 	}
	// }
	return (NULL);
}