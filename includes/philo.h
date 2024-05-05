/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:22:03 by hnagasak          #+#    #+#             */
/*   Updated: 2024/05/06 05:18:04 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

typedef struct timeval	t_timeval;

typedef struct s_config
{
	size_t				num_of_philo;
	size_t				time_to_die;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				required_eat_count;
	t_timeval			start;
	// pthread_mutex_t	*forks;
}						t_config;

typedef struct s_philo
{
	int					id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	int					eat_count;
	long				last_eat_time;
	t_config			*config;
}						t_philo;

long					get_elapsed_time(t_timeval start);
long					us2ms(long usec);

// print.c
void					print_config(t_config *config);
void					print_philo(t_philo *data);
void					print_forks(pthread_mutex_t **forks);
void					print_philos_forks(t_philo *data, size_t num_of_philo);

#endif